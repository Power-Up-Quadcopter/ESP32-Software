
#include <driver/ledc.h>
#include "freertos/FreeRTOS.h"
#include <freertos/semphr.h>
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "GPIO_Expand.h"
#include "ESC_Control.h"

#define ESC_STACK_SIZE 2048
#define IS_YES(pin) (powers[pin]-timer<=0)
#define NOP() asm volatile ("nop")

//Pin numbers for ESC output
//Look at the table at this link for valid pins
//https://randomnerdtutorials.com/esp32-pinout-reference-gpios/


namespace Esc {

    [[noreturn]] void loop(void* arg);

//Globals for keeping track of current ESC state
    const uint8_t escPins[] = {ESC0_PIN, ESC1_PIN, ESC2_PIN, ESC3_PIN};
    const ledc_channel_t escChannels[] = {LEDC_CHANNEL_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3, LEDC_CHANNEL_4};
    uint16_t powers[] = {100,300,500,800}; //on time in units of microseconds
    pin_t powerToEsc[] = {GPA0,GPA2,GPA1,GPA3}; //corresponding ports
    int16_t timer = 0; //timer progress, units of microseconds
    SemaphoreHandle_t xMutex;

//set up ESC pins for PWM
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

        xMutex = xSemaphoreCreateMutex();
        xTaskCreate(&loop, "DroneLoop", ESC_STACK_SIZE, NULL, 4, NULL);
    }


    unsigned long IRAM_ATTR micros()
    {
        return (unsigned long) (esp_timer_get_time());
    }

    void IRAM_ATTR delayMicroseconds(uint32_t us)
    {
        uint32_t m = micros();
        if(us){
            uint32_t e = (m + us);
            if(m > e){ //overflow
                while(micros() > e){
                    NOP();
                }
            }
            while(micros() < e){
                NOP();
            }
        }
    }

//takes in esc #0-3 and power from  0-100
//sets up PWM output to corresponding ESC
    void set(uint8_t esc, uint8_t power) {
//        assert(esc < 4);
//        assert(power <= 100);
//        float fraction = power / 100.0F;
//
//        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, escChannels[esc],
//                                      81.91 * fraction + 81.91)); //1% duty when 0, 2% when 100
//        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, escChannels[esc]));
        xSemaphoreTake( xMutex, portMAX_DELAY );

        xSemaphoreGive( xMutex );
    }

    [[noreturn]] void loop(void* arg){
        bool off = false;
        unsigned long time = micros();
        unsigned long yes;
        while(1){
            Expand::setAllData(Expand::portA| (0x0Fu), Expand::portB );
            vTaskDelay(8 / portTICK_PERIOD_MS);
            delayMicroseconds(powers[0]);
            Expand::setPinData(powerToEsc[0], 0);
            delayMicroseconds(powers[1]-powers[0]);
            Expand::setPinData(powerToEsc[1], 0);
            delayMicroseconds(powers[2]-powers[1]);
            Expand::setPinData(powerToEsc[2], 0);
            delayMicroseconds(powers[3]-powers[2]);
            Expand::setPinData(powerToEsc[3], 0);
            delayMicroseconds(1000-powers[3]);
            vTaskDelay(36 / portTICK_PERIOD_MS);
        }
    }

}
