

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "ESC_Control.h"


//Note: If not using OpenOCD, run using "mingw32-make.exe flash" in the terminal below


void DroneLoop();


//arduino setup function equivalent
void app_main() {

    //TEST CODE FOR ESCs HERE:
    printf("Setting up\n");
    ESCSetup();
    printf("Output 50 Percent in ESC0\n");
    ESC_Set(0,50);
    //END TEST CODE

    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    esp_restart();
}

//arduino loop function equivalent
void DroneLoop(){
    printf("Looping\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS); //delay 1000ms
}
