

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <string.h>
#include <lwip/sockets.h>
#include <freertos/event_groups.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "Communication.h"


#include "lwip/err.h"
#include "lwip/sys.h"

#define SERVER_TASK_STACK_SIZE 4096

#define ESP_WIFI_SSID      "PowerUp"
#define ESP_WIFI_PASS      ""
#define ESP_WIFI_CHANNEL   1
#define LISTENQ 2

namespace Wifi {

    SemaphoreHandle_t xMutexU;
    SemaphoreHandle_t xMutexT;


    void sendTCP(char* message, int len, bool isTask);
    void sendUDP(char* message, int len, bool isTask);

    const int CLIENT_CONNECTED_BIT = BIT0;
    const int CLIENT_DISCONNECTED_BIT = BIT1;
    const int AP_STARTED_BIT = BIT2;

    static EventGroupHandle_t wifi_event_group;
    static const char *TAG = "wifi softAP";

    int TCPClientSocket;
    int UDPClientSocket;
    int TCPsocketHandle;
    int UDPsocketHandle;
    bool TCPConnected = false;
    bool UDPConnected = false;


    static esp_err_t event_handler(void *ctx, system_event_t *event) {
        switch (event->event_id) {
            case SYSTEM_EVENT_AP_START:
                printf("Event:ESP32 is started in AP mode\n");
                xEventGroupSetBits(wifi_event_group, AP_STARTED_BIT);
                break;

            case SYSTEM_EVENT_AP_STACONNECTED:
                xEventGroupSetBits(wifi_event_group, CLIENT_CONNECTED_BIT);
                break;

            case SYSTEM_EVENT_AP_STADISCONNECTED:
                xEventGroupSetBits(wifi_event_group, CLIENT_DISCONNECTED_BIT);
                break;
            default:
                break;
        }
        return ESP_OK;
    }


    void TCPServer(void* arg) {
        struct sockaddr_in tcpServerAddr;
        tcpServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        tcpServerAddr.sin_family = AF_INET;
        tcpServerAddr.sin_port = htons(5414);
        int r;
        char recv_buf[64];
        char send_buf[64];
        static struct sockaddr_in remote_addr;
        static unsigned int socklen;
        socklen = sizeof(remote_addr);
        xEventGroupWaitBits(wifi_event_group, AP_STARTED_BIT, false, true, portMAX_DELAY);

        while(1) {
            TCPsocketHandle = socket(AF_INET, SOCK_STREAM, 0);
            if (TCPsocketHandle < 0) {
                ESP_LOGE(TAG, "... Failed to allocate socket. Ernno=%d\n",TCPsocketHandle);

                vTaskDelay(1000 / portTICK_PERIOD_MS);
                vTaskDelete(xTaskGetCurrentTaskHandle());
                return;
            }
            ESP_LOGI(TAG, "... allocated socket\n");
            if (bind(TCPsocketHandle, (struct sockaddr *) &tcpServerAddr, sizeof(tcpServerAddr)) != 0) {
                ESP_LOGE(TAG, "... socket bind failed errno=%d \n", errno);
                close(TCPsocketHandle);
                vTaskDelay(4000 / portTICK_PERIOD_MS);
                vTaskDelete(xTaskGetCurrentTaskHandle());
                return;
            }
            ESP_LOGI(TAG, "... socket bind done \n");
            if (listen(TCPsocketHandle, LISTENQ) != 0) {
                ESP_LOGE(TAG, "... socket listen failed errno=%d \n", errno);
                close(TCPsocketHandle);
                vTaskDelay(4000 / portTICK_PERIOD_MS);
                vTaskDelete(xTaskGetCurrentTaskHandle());
                return;
            }
            TCPConnected = false;
            while (1) {
                TCPClientSocket = accept(TCPsocketHandle, (struct sockaddr *) &remote_addr, &socklen);

                if (TCPsocketHandle < 0) {
                    ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
                    close(TCPsocketHandle);
                    break;
                }
                ESP_LOGI(TAG, "New TCP connection request");
                TCPConnected = true;
                while (int len = recv(TCPClientSocket, recv_buf, sizeof(recv_buf) - 1, 0)) {
                    if (len < 0) {
                        ESP_LOGE(TAG, "recv failed: errno %d", errno);
                        vTaskDelay(2000 / portTICK_PERIOD_MS);
                        close(TCPClientSocket);
                        break;
                    } else {
                        len = Talk::parse((uint8_t *) recv_buf, (uint8_t *) send_buf, len);
                        if (len > 0) {
                            sendTCP(send_buf, len, false);
                        }
                    }
                }
                TCPConnected = false;
            }
        }
        vTaskDelete(xTaskGetCurrentTaskHandle());
    }

    struct sockaddr_in source_addr;

    void UDPServer(void* arg) {
        while(1){
            struct sockaddr_in udpServerAddr;
            udpServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
            udpServerAddr.sin_family = AF_INET;
            udpServerAddr.sin_port = htons(5414);
            int r;
            char recv_buf[64];
            char send_buf[64];
            static struct sockaddr_in remote_addr;
            static unsigned int socklenOut;
            socklenOut = sizeof(remote_addr);
            xEventGroupWaitBits(wifi_event_group, AP_STARTED_BIT, false, true, portMAX_DELAY);

            UDPsocketHandle = socket(AF_INET, SOCK_DGRAM, 0);
            if (UDPsocketHandle < 0) {
                ESP_LOGE(TAG, "... Failed to allocate UDP socket.\n");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                break;
            }
            ESP_LOGI(TAG, "... allocated UDP socket\n");
            if (bind(UDPsocketHandle, (struct sockaddr *) &udpServerAddr, sizeof(udpServerAddr)) != 0) {
                ESP_LOGE(TAG, "... UDP socket bind failed errno=%d \n", errno);
                close(UDPsocketHandle);
                vTaskDelay(4000 / portTICK_PERIOD_MS);
                break;
            }
            UDPConnected = true;
            while (1) {
                socklen_t socklenIn = sizeof(source_addr);
                int len = recvfrom(UDPsocketHandle, recv_buf, sizeof(recv_buf) - 1, 0, (struct sockaddr *) &source_addr,
                                   &socklenIn);

                source_addr.sin_port = 5414;
                xSemaphoreTake( xMutexU, portMAX_DELAY );
                if (len < 0) {
                    ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                    break;
                }

                len = Talk::parse((uint8_t*) recv_buf, (uint8_t*) send_buf, len);
                if(len>0){

                    sendUDP(send_buf, len, true);
                }
                xSemaphoreGive( xMutexU );
            }
        }
        vTaskDelete(xTaskGetCurrentTaskHandle());
    }


    void sendTCP(char* message, int len, bool isTask=false) {
        if (!TCPConnected) return;
        if(!isTask) xSemaphoreTake( xMutexT, portMAX_DELAY );
        if (write(TCPClientSocket, message, len) < 0) {
            ESP_LOGE(TAG, "... Send failed \n");
            if(!isTask) xSemaphoreGive( xMutexT );
            return;
        }
        if(!isTask) xSemaphoreGive( xMutexT );
        ESP_LOGI(TAG, "... socket send success");
    }

    void sendUDP(char* message, int len, bool isTask=false) {
        if(!UDPConnected) return;
        if(!isTask) xSemaphoreTake( xMutexU, portMAX_DELAY );
        if ( sendto(UDPsocketHandle, message, len, 0, (const struct sockaddr*) &source_addr, sizeof(source_addr)) < 0 ) {
            ESP_LOGE(TAG, "... Send failed \n");
            if(!isTask) xSemaphoreGive( xMutexU );
            return;
        }
        if(!isTask) xSemaphoreGive( xMutexU );
        ESP_LOGI(TAG, "... socket send success");
    }

    void init() {
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
        wifi_event_group = xEventGroupCreate();

        // initialize the tcp stack
        tcpip_adapter_init();
        // stop DHCP server
        ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
        // assign a static IP to the network interface
        tcpip_adapter_ip_info_t info;
        memset(&info, 0, sizeof(info));
        IP4_ADDR(&info.ip, 10, 10, 10, 10);
        IP4_ADDR(&info.gw, 10, 10, 10, 10);//ESP acts as router, so gw addr will be its own addr
        IP4_ADDR(&info.netmask, 255, 255, 255, 0);
        ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
        // start the DHCP server
        ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

        wifi_config_t wifi_config = {};
        wifi_config.ap.ssid[0] = 'P';
        wifi_config.ap.ssid[1] = 'U';
        wifi_config.ap.ssid[2] = 10;
        wifi_config.ap.password[0] = '1';
        wifi_config.ap.password[1] = '2';
        wifi_config.ap.password[2] = '3';
        wifi_config.ap.password[3] = '4';
        wifi_config.ap.password[4] = '5';
        wifi_config.ap.password[5] = '6';
        wifi_config.ap.password[6] = '7';
        wifi_config.ap.password[7] = '8';
        wifi_config.ap.ssid_len = 2;
        wifi_config.ap.channel = ESP_WIFI_CHANNEL;
        wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
        wifi_config.ap.ssid_hidden = 0;
        wifi_config.ap.max_connection = 4;
        wifi_config.ap.beacon_interval = 100;

        if (strlen(ESP_WIFI_PASS) == 0) {
            wifi_config.ap.authmode = WIFI_AUTH_OPEN;
        }

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());
        esp_wifi_set_max_tx_power(78);

        ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
                 ESP_WIFI_SSID, ESP_WIFI_PASS, ESP_WIFI_CHANNEL);

        xMutexU = xSemaphoreCreateMutex();
        xMutexT = xSemaphoreCreateMutex();
        xTaskCreate(&TCPServer, "TCPServer", SERVER_TASK_STACK_SIZE, NULL, 4, NULL);
        xTaskCreate(&UDPServer, "UDPServer", SERVER_TASK_STACK_SIZE, NULL, 4, NULL);
    }

}