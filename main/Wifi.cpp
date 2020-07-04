

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <string.h>
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



static const char *TAG = "wifi softAP";

void wifi_event_handler(void* arg, esp_event_base_t event_base,
                        int32_t event_id, void* event_data)
{
    printf("%d\n", event_id);
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station " MACSTR" join, AID=%d",
                MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station " MACSTR" leave, AID=%d",
                MAC2STR(event->mac), event->aid);
    }
}

void Wifi_Init(){
//    tcpip_adapter_init();
//    tcpip_adapter_ip_info_t info;
//    IP4_ADDR(&info.ip, 192, 168, 1, 1);
//    IP4_ADDR(&info.gw, 192, 168, 1, 1);
//    IP4_ADDR(&info.netmask, 255, 255, 255, 0);
//    ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
//    memset(&info, 0, sizeof(info));

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

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
    /*
    wifi_ap_config_t ap = {
            {.ssid = ESP_WIFI_SSID},
            {.password = ESP_WIFI_PASS,
            .ssid_len = static_cast<uint8_t>(strlen(ESP_WIFI_SSID)),
            .channel = ESP_WIFI_CHANNEL,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .ssid_hidden = 0,
            .max_connection = 4,
            .beacon_interval = 100
    };
    wifi_config.ap = ap;
     */
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
