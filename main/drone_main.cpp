
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
#include "Control.h"
#include "Communication.h"

#define MAIN_LOOP_STACK_SIZE 4096
#define TAG "Main_Loop"

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

    Control::init();
    Wifi::init();
    ESP_ERROR_CHECK(I2C_Init());

    Expand::init();

//    SD::init();
//    spl06.initialize();
    Esc::init();
//    GPS::init(true);
//    Mag::init();

    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", MAIN_LOOP_STACK_SIZE, NULL, 4, NULL);
//    xTaskCreate(&MPU::task_display, "MPUTask", MAIN_LOOP_STACK_SIZE, NULL, 4, NULL);
}

//arduino loop function equivalent
[[noreturn]] void DroneLoop(void* arg){
    int connected = 3;
    unsigned char ping[1] = {0xF0};
    while (1){
//        printf("Temp: %0.2f\n", spl06.getTemperature());
//        printf("Pressure: %0.2f\n", spl06.getPressure());
//        printf("Altitude: %0.2f\n", spl06.getAltitude());
//        printf("-----------------------\n");

        Wifi::sendTCP((char*) ping, 1);

        vTaskDelay( 1000 / portTICK_PERIOD_MS); //delay 1000ms
        if(!Talk::receive && Wifi::TCPConnected){
            connected--;
            if(connected==0){
//                ESP_LOGW(TAG, "TIMEOUT");
                Wifi::TCPConnected = false;
            }

        }
        else{
            connected = 3;
            Talk::receive = false;
        }

    }
    vTaskDelete(NULL);
}
