
#include "ESC_Control.h"
#include <stdio.h>
#include <driver/ledc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define ESC_STACK_SIZE 2048

//Pin numbers for ESC output
//Look at the table at this link for valid pins
//https://randomnerdtutorials.com/esp32-pinout-reference-gpios/



//Globals for keeping track of current ESC state
const uint8_t escPins[] = {ESC0_PIN,ESC1_PIN,ESC2_PIN,ESC3_PIN};
const uint8_t escChannels[] = {LEDC_CHANNEL_1,LEDC_CHANNEL_2,LEDC_CHANNEL_3,LEDC_CHANNEL_4};

//set up ESC pins for PWM
void Esc_Init(){

    ledc_channel_config_t ledc_channel_ESC0 = {
        .gpio_num   = ESC0_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel    = LEDC_CHANNEL_1,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER_1,
        .duty = 0
    };

    ledc_channel_config_t ledc_channel_ESC1 = {
            .gpio_num   = ESC1_PIN,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel    = LEDC_CHANNEL_2,
            .intr_type  = LEDC_INTR_DISABLE,
            .timer_sel  = LEDC_TIMER_1,
            .duty = 0
    };

    ledc_channel_config_t ledc_channel_ESC2 = {
            .gpio_num   = ESC2_PIN,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel    = LEDC_CHANNEL_3,
            .intr_type  = LEDC_INTR_DISABLE,
            .timer_sel  = LEDC_TIMER_1,
            .duty = 0
    };

    ledc_channel_config_t ledc_channel_ESC3 = {
            .gpio_num   = ESC3_PIN,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel    = LEDC_CHANNEL_4,
            .intr_type  = LEDC_INTR_DISABLE,
            .timer_sel  = LEDC_TIMER_1,
            .duty = 0
    };


    ledc_timer_config_t ledc_timer = {
            .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
            .freq_hz = 5000,                      // frequency of PWM signal
            .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
            .timer_num = LEDC_TIMER_1,            // timer index
            .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };

    ESP_ERROR_CHECK( ledc_channel_config(&ledc_channel_ESC0) );
    ESP_ERROR_CHECK( ledc_channel_config(&ledc_channel_ESC1) );
    ESP_ERROR_CHECK( ledc_channel_config(&ledc_channel_ESC2) );
    ESP_ERROR_CHECK( ledc_channel_config(&ledc_channel_ESC3) );
    ESP_ERROR_CHECK( ledc_timer_config(&ledc_timer) );

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

}

//takes in esc #0-3 and power from  0-100
//sets up PWM output to corresponding ESC
void Esc_Set(uint8_t esc, uint8_t power){
    assert(esc<4);
    assert(power<=100);
    float fraction  = power/100.0F;

    ESP_ERROR_CHECK( ledc_set_duty(LEDC_HIGH_SPEED_MODE, escChannels[esc], 81.91*fraction + 81.91) ); //1% duty when 0, 2% when 100
    ESP_ERROR_CHECK( ledc_update_duty(LEDC_HIGH_SPEED_MODE, escChannels[esc]) );
}

