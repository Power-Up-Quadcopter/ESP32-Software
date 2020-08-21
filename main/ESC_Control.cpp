
#include <cmath>
#include <driver/ledc.h>
#include "freertos/FreeRTOS.h"
#include <freertos/semphr.h>
#include <esp_log.h>
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "I2C_General.h"
#include "ESC_Control.h"


#define ESC_STACK_SIZE 2048
#define IS_YES(pin) (powers[pin]-timer<=0)
#define NOP() asm volatile ("nop")
#define NANO_ADDR 0x04


#define MOTOR_MIN 12.0F
#define BOUND(x)  fmaxf(fminf(x + MOTOR_MIN,255.0f), MOTOR_MIN);

namespace Esc {

    void loop(void* arg);

    //Globals for keeping track of current ESC state
    uint8_t powers[] = {0,0,0,0}; //Fr,FL,BL,BR
    SemaphoreHandle_t xMutex;

    //initializes ESC output in off state UNUSED
    void init() {

        xMutex = xSemaphoreCreateMutex();
        xTaskCreate(&loop, "ESCLoop", ESC_STACK_SIZE, NULL, 5, NULL);
        I2C_Write8_Nano(NANO_ADDR, powers, 4);
    }

    //takes in an array of 4 bytes, sets all ESC output values
    void setAll(const uint8_t* newPowers){
        xSemaphoreTake( xMutex, portMAX_DELAY );
        for(int i=0; i<4; i++){
            powers[i] = newPowers[i];
        }
        xSemaphoreGive( xMutex );
    }

    //takes in esc #0-3 and power from  0-255
    //sets up PWM output to corresponding ESC
    void set(ESC_PIN_t esc, uint8_t power) {
        xSemaphoreTake( xMutex, portMAX_DELAY );
            powers[esc] = power;
        xSemaphoreGive( xMutex );
    }


    void motorPID(float toYaw, float toPitch, float toRoll, float throttle){
        uint8_t tempPower[4];


        tempPower[FrontR] = BOUND(throttle + toYaw + toPitch + toRoll);
        tempPower[FrontL] = BOUND(throttle - toYaw + toPitch - toRoll);
        tempPower[BackL]  = BOUND(throttle - toYaw - toPitch - toRoll);
        tempPower[BackR]  = BOUND(throttle + toYaw - toPitch + toRoll);
        ESP_LOGI("ESC", "%f, %f, %f, %f", toYaw, toPitch, toRoll, throttle);
        ESP_LOGI("ESC", "%d, %d, %d, %d", tempPower[0], tempPower[1], tempPower[2], tempPower[3]);
        setAll(tempPower);
    }


    void loop(void* arg){
        while(1){
            xSemaphoreTake( xMutex, portMAX_DELAY );
            I2C_Write8_Nano(NANO_ADDR, powers, 4);
            xSemaphoreGive( xMutex );
            vTaskDelay( 50 / portTICK_PERIOD_MS);
        }
        vTaskDelete(NULL);
    }

}
