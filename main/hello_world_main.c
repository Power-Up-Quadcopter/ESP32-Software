
#include <stdio.h>
#include <nvs_flash.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "ESC_Control.h"
#include "I2C_General.h"

#define MAIN_LOOP_STACK_SIZE 2048

//Note: If not using OpenOCD, run using "mingw32-make.exe flash" in the terminal below

void DroneLoop(void*);


//arduino setup function equivalent
void app_main() {

    printf("Setting up\n");
    ESC_Init();
    I2C_Init();

    //TEST CODE HERE
    printf("TEST: Output 50 Percent in ESC0\n");
    ESC_Set(0,50);
    //END TEST AREA

    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", MAIN_LOOP_STACK_SIZE, NULL, 5, NULL);
}

//arduino loop function equivalent
void DroneLoop(void* ptr){
    while (1){
        //TODO put crap here

        //TEST CODE HERE
        printf("Looping\n");
        //END TEST AREA

        vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms

    }
}
