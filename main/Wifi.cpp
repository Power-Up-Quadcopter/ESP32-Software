

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


#include "lwip/err.h"
#include "lwip/sys.h"

#define ESP_WIFI_SSID      "PowerUp"
#define ESP_WIFI_PASS      ""
#define ESP_WIFI_CHANNEL   1
#define LISTENQ 2

const int CLIENT_CONNECTED_BIT = BIT0;
const int CLIENT_DISCONNECTED_BIT = BIT1;
const int AP_STARTED_BIT = BIT2;

static EventGroupHandle_t wifi_event_group;
static const char *TAG = "wifi softAP";

int clientSocket; //client socket
int socketHandle;
bool TCPConnected = false;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
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

void Wifi_startTCPServer() {
    struct sockaddr_in tcpServerAddr;
    tcpServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcpServerAddr.sin_family = AF_INET;
    tcpServerAddr.sin_port = htons( 5414 );
    int r;
    char recv_buf[64];
    static struct sockaddr_in remote_addr;
    static unsigned int socklen;
    socklen = sizeof(remote_addr);
    xEventGroupWaitBits(wifi_event_group,AP_STARTED_BIT,false,true,portMAX_DELAY);

        socketHandle = socket(AF_INET, SOCK_STREAM, 0);
        if(socketHandle < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            return;
        }
        ESP_LOGI(TAG, "... allocated socket\n");
        if(bind(socketHandle, (struct sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr)) != 0) {
            ESP_LOGE(TAG, "... socket bind failed errno=%d \n", errno);
            close(socketHandle);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            return;
        }
        ESP_LOGI(TAG, "... socket bind done \n");
        if(listen (socketHandle, LISTENQ) != 0) {
            ESP_LOGE(TAG, "... socket listen failed errno=%d \n", errno);
            close(socketHandle);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            return;
        }

        clientSocket = accept(socketHandle,(struct sockaddr *)&remote_addr, &socklen);
        ESP_LOGI(TAG,"New TCP connection request");
    TCPConnected = true;
//            //set O_NONBLOCK so that recv will return, otherwise we need to impliment message end
//            //detection logic. If know the client message format you should instead impliment logic
//            //detect the end of message
//            fcntl(cs,F_SETFL,O_NONBLOCK);
//            do {
//                bzero(recv_buf, sizeof(recv_buf));
//                r = recv(cs, recv_buf, sizeof(recv_buf)-1,0);
//                for(int i = 0; i < r; i++) {
//                    putchar(recv_buf[i]);
//                }
//            } while(r > 0);
//
//            ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
//
//            char message[] = "asdf\n";
//        }
//        ESP_LOGI(TAG, "... server will be opened in 5 seconds");
//        vTaskDelay(5000 / portTICK_PERIOD_MS);
//    }
//    ESP_LOGI(TAG, "...tcp_client task closed\n");
}

void Wifi_sendTCP(char* message, int len) {
    if(!TCPConnected) return;
    if( write(clientSocket , message , len) < 0)
    {
        ESP_LOGE(TAG, "... Send failed \n");
        close(socketHandle);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "... socket send success");
}

void Wifi_Init(){
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_event_group = xEventGroupCreate();

    // initialize the tcp stack
    tcpip_adapter_init();
    // stop DHCP server
    ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
    // assign a static IP to the network interface
    tcpip_adapter_ip_info_t info;
    memset(&info, 0, sizeof(info));
    IP4_ADDR(&info.ip, 192, 168, 1, 1);
    IP4_ADDR(&info.gw, 192, 168, 1, 1);//ESP acts as router, so gw addr will be its own addr
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
    esp_wifi_set_max_tx_power(127);

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             ESP_WIFI_SSID, ESP_WIFI_PASS, ESP_WIFI_CHANNEL);
}
