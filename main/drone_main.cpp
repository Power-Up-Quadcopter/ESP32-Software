
#include <stdio.h>
#include <nvs_flash.h>
#include <tcpip_adapter.h>
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
#include "SPL06.h"
#include "Wifi.h"
#include "Magnetometer.h"
#include "GPIO_Expand.h"
#include "SD.h"

#define MAIN_LOOP_STACK_SIZE 4096

using namespace std;

extern "C" {
    void app_main();
};

//Note: If not using OpenOCD, run using "mingw32-make.exe flash" in the terminal below

SPL06 spl06;

[[noreturn]] void DroneLoop(void*);

//arduino setup function equivalent
void app_main() {

    ESP_LOGI("app_main", "Setting up");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    Wifi::init();
    ESP_ERROR_CHECK(I2C_Init());
    SD::init();
    spl06.initialize();
//    Esc::init();
//    GPS::init(true);
    Expand::init();


    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", MAIN_LOOP_STACK_SIZE, NULL, 4, NULL);
}

//arduino loop function equivalent
[[noreturn]] void DroneLoop(void* arg){

    while (1){
//        printf("Temp: %0.2f\n", spl06.getTemperature());
//        printf("Pressure: %0.2f\n", spl06.getPressure());
//        printf("Altitude: %0.2f\n", spl06.getAltitude());
//        printf("-----------------------\n");

//        if(magReady){
//            printf("-------------\n"
//                   "Temp:%d\n"
//                   "Mag X,Y,Z: %d, %d, %d"
//                   ,magData.temp,magData.x,magData.y,magData.z);
//        }

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
//        printf("%s", GPS::send(ALL).c_str());


//        Expand::setPinData(GPA0,1);
//        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms
//        Expand::setPinData(GPA1,1);
//        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms
//        Expand::setPinData(GPA2,1);
//        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms
//        Expand::setPinData(GPA0,0);
//        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms
//        Expand::setPinData(GPA1,0);
//        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms
//        Expand::setPinData(GPA2,0);
        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms
    }
}
