
#include <stdio.h>
#include <nvs_flash.h>
#include <tcpip_adapter.h>
#include <math.h>
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



    ESP_ERROR_CHECK(I2C_Init());
    Esc::init();
    Expand::init();

//    SD::init();
//    spl06.initialize();
//    GPS::init(true);
    Mag::init();
    Wifi::init();

    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", MAIN_LOOP_STACK_SIZE, NULL, 4, NULL);
    xTaskCreate(&MPU::task_display, "MPUTask", MAIN_LOOP_STACK_SIZE, NULL, 4, NULL);

    Control::init();
}

//arduino loop function equivalent
[[noreturn]] void DroneLoop(void* arg){
    int connected = 3;
    const unsigned char ping[1] = {0xF0u};
    while (true){
//        printf("Temp: %0.2f\n", spl06.getTemperature());
//        printf("Pressure: %0.2f\n", spl06.getPressure());
//        printf("Altitude: %0.2f\n", spl06.getAltitude());
//        printf("-----------------------\n");
        MagData* data = Mag::getData();

//        printf("+++++\n");
//        printf("YAW: % 4.3f\n", (MPU::ypr[0] * 180/M_PI));
//        printf("PITCH: % 4.3f\n", MPU::ypr[1] * 180/M_PI);
//        printf("ROLL: % 4.3f\n", MPU::ypr[2] * 180/M_PI);
//        printf("-----\n");
//        printf("X: %d\n", data->x);
//        printf("Y: %d\n", data->y);
//        printf("Z: %d\n", data->z);
//        printf("X: %d\n", MPU::raw[0]);
//        printf("Y: %d\n", MPU::raw[1]);
//        printf("Z: %d\n", MPU::raw[2]);
//        printf("X: %f\n", MPU::getAvgX());
//        printf("Y: %f\n", MPU::getAvgY());
//        printf("Z: %f\n", MPU::getAvgZ());
//        printf("A: %f\n", MPU::getAvgA());
//        float xhoriz = data->x * cos(-MPU::ypr[1]) + data->y * sin(-MPU::ypr[2]) * sin(-MPU::ypr[1]) - data->z * cos(-MPU::ypr[2]) * sin(-MPU::ypr[1]);
//        float yhoriz = data->y * cos(-MPU::ypr[2]) + data->z * sin(-MPU::ypr[2]);
//        printf("Heading: %f\n", atan2(yhoriz, xhoriz) * 180 / M_PI);
//        string str("Raw:0,0,0," + to_string((int) (MPU::ypr[0] * 180/M_PI)) + "," + to_string((int)(MPU::ypr[1]* 180/M_PI)) + "," + to_string((int)(MPU::ypr[2]* 180/M_PI)) + "," + to_string(data->x) + "," + to_string(data->y) + "," + to_string(data->z) + "\n");
//        printf("%s", str.c_str());

        Wifi::sendTCP((char*) ping, 1);

        vTaskDelay( 200 / portTICK_PERIOD_MS);
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
