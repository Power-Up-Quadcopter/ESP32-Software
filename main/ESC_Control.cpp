
#include <driver/ledc.h>
#include "freertos/FreeRTOS.h"
#include <freertos/semphr.h>
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "GPIO_Expand.h"
#include "I2C_General.h"
#include "ESC_Control.h"


#define ESC_STACK_SIZE 2048
#define IS_YES(pin) (powers[pin]-timer<=0)
#define NOP() asm volatile ("nop")
#define NANO_ADDR 0x04

//Pin numbers for ESC output
//Look at the table at this link for valid pins
//https://randomnerdtutorials.com/esp32-pinout-reference-gpios/


namespace Esc {



    //Globals for keeping track of current ESC state
    uint8_t powers[] = {0,0,0,0};
    SemaphoreHandle_t xMutex;

    //initializes ESC output in off state UNUSED
    void init() {

//        ledc_channel_config_t ledc_channel_ESC0 = {
//                .gpio_num   = ESC0_PIN,
//                .speed_mode = LEDC_HIGH_SPEED_MODE,
//                .channel    = LEDC_CHANNEL_1,
//                .intr_type  = LEDC_INTR_DISABLE,
//                .timer_sel  = LEDC_TIMER_1,
//                .duty = 0
//        };
//
//        ledc_channel_config_t ledc_channel_ESC1 = {
//                .gpio_num   = ESC1_PIN,
//                .speed_mode = LEDC_HIGH_SPEED_MODE,
//                .channel    = LEDC_CHANNEL_2,
//                .intr_type  = LEDC_INTR_DISABLE,
//                .timer_sel  = LEDC_TIMER_1,
//                .duty = 0
//        };
//
//        ledc_channel_config_t ledc_channel_ESC2 = {
//                .gpio_num   = ESC2_PIN,
//                .speed_mode = LEDC_HIGH_SPEED_MODE,
//                .channel    = LEDC_CHANNEL_3,
//                .intr_type  = LEDC_INTR_DISABLE,
//                .timer_sel  = LEDC_TIMER_1,
//                .duty = 0
//        };
//
//        ledc_channel_config_t ledc_channel_ESC3 = {
//                .gpio_num   = ESC3_PIN,
//                .speed_mode = LEDC_HIGH_SPEED_MODE,
//                .channel    = LEDC_CHANNEL_4,
//                .intr_type  = LEDC_INTR_DISABLE,
//                .timer_sel  = LEDC_TIMER_1,
//                .duty = 0
//        };
//
//
//        ledc_timer_config_t ledc_timer = {
//                .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
//                .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
//                .timer_num = LEDC_TIMER_1,            // timer index
//                .freq_hz = 5000,                      // frequency of PWM signal
//                .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
//        };
//
//        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ESC0));
//        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ESC1));
//        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ESC2));
//        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ESC3));
//        ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

//    //Configure pins for GPIO
//    gpio_pad_select_gpio(ESC0_PIN);
//    gpio_pad_select_gpio(ESC1_PIN);
//    gpio_pad_select_gpio(ESC2_PIN);
//    gpio_pad_select_gpio(ESC3_PIN);
//
//    gpio_set_direction(ESC0_PIN, GPIO_MODE_OUTPUT);
//    gpio_set_direction(ESC1_PIN, GPIO_MODE_OUTPUT);
//    gpio_set_direction(ESC2_PIN, GPIO_MODE_OUTPUT);
//    gpio_set_direction(ESC3_PIN, GPIO_MODE_OUTPUT);
//
//    //All pins off initially
//    gpio_set_level(ESC0_PIN, 0);
//    gpio_set_level(ESC1_PIN, 0);
//    gpio_set_level(ESC2_PIN, 0);
//    gpio_set_level(ESC3_PIN, 0);

  //      xMutex = xSemaphoreCreateMutex();
 //       xTaskCreate(&loop, "ESCLoop", ESC_STACK_SIZE, NULL, 4, NULL);
        I2C_Write8_Nano(NANO_ADDR, powers, 4);
    }

    //takes in an array of 4 bytes, sets all ESC output values
    void setAll(const uint8_t* newPowers){
        for(int i=0; i<4; i++){
            powers[i] = newPowers[i];
        }
        I2C_Write8_Nano(NANO_ADDR, powers, 4);
    }

    //takes in esc #0-3 and power from  0-255
    //sets up PWM output to corresponding ESC
    void set(ESC_PIN_t esc, uint8_t power) {
        powers[esc] = power;
        I2C_Write8_Nano(NANO_ADDR, powers, 4);
    }


    //UNUSED
    [[noreturn]] void loop(void* arg){
        while(1){
            I2C_Write8_Nano(NANO_ADDR, powers, 4);
            vTaskDelay( 1000 / portTICK_PERIOD_MS);
        }
        vTaskDelete(NULL);
    }

}
