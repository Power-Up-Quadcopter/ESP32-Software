
#include <stdio.h>
#include <string>
#include <nvs_flash.h>
#include <tcpip_adapter.h>
#include <sstream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "MPU6500.h"
#include "ESC_Control.h"
#include "I2C_General.h"
#include "GPS.h"
#include "Wifi.h"
#include "Magnetometer.h"

#define MAIN_LOOP_STACK_SIZE 2048

using namespace std;

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


    ESP_ERROR_CHECK(I2C_Init());
//    Esc_Init();
//    GPS_Init();
    Wifi_Init();

    //TEST CODE HERE
//    printf("TEST: Output 50 Percent in ESC0\n");
//    Esc_Set(0, 50);
    //END TEST AREA

//    Wifi_startTCPServer();

    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", MAIN_LOOP_STACK_SIZE, NULL, 4, NULL);
}

//arduino loop function equivalent
void DroneLoop(void* ptr){

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    while (1){
        //TODO put crap here
        if(magReady){
            printf("-------------\n"
                   "Temp:%d\n"
                   "Mag X,Y,Z: %d, %d, %d"
                   ,magData.temp,magData.x,magData.y,magData.z);
        }
        //TEST CODE HERE
        //printf("Looping\n");
        //END TEST AREA

//        string msg = "PSTMCFGANTSENS,2,1,0,0,0,1,8,150,500";



//  TCP Test message
//        char message[] = "lololol\n";
//        Wifi_sendTCP(message);

//  I2C GPIO Expander LED Blink
//        int chip_addr = 0x20;
//        I2C_Write8(chip_addr, 0x00, 0b000);  //  set port A to outputs
//        I2C_Write8(chip_addr, 0x01, 0b000);  //  set port B to outputs
//        while(1) {
//            printf("LEDR ON\n");
//            I2C_Write8(chip_addr,0b101, 0x12);
//            vTaskDelay(1000 / portTICK_RATE_MS);
//            printf("LEDR OFF\n");
//            I2C_Write8(chip_addr, 0b000, 0x12);
//            vTaskDelay(1000 / portTICK_RATE_MS);
//        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms

    }
}
