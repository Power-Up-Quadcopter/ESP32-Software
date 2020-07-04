
#include <stdio.h>
#include <nvs_flash.h>
#include <tcpip_adapter.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "MPU6500.h"
#include "ESC_Control.h"
#include "I2C_General.h"
#include "GPS.h"
#include "Wifi.h"

#define MAIN_LOOP_STACK_SIZE 2048

extern "C" {
    void app_main();
};


//Note: If not using OpenOCD, run using "mingw32-make.exe flash" in the terminal below

void DroneLoop(void*);


//arduino setup function equivalent
void app_main() {

    ESP_LOGI("app_main", "Setting up");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    Esc_Init();
    I2C_Init();
    GPS_Init();
    Wifi_Init();

    //TEST CODE HERE
    printf("TEST: Output 50 Percent in ESC0\n");
    Esc_Set(0,50);
    //END TEST AREA

    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", MAIN_LOOP_STACK_SIZE, NULL, 5, NULL);
}

//arduino loop function equivalent
void DroneLoop(void* ptr){
    while (1){
        //TODO put crap here

        //TEST CODE HERE
        //printf("Looping\n");
        //END TEST AREA

        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms

    }
}
