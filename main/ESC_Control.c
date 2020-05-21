
#include "ESC_Control.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"


//Pin numbers for ESC output
#define ESC0_PIN 0
#define ESC1_PIN 1
#define ESC2_PIN 2
#define ESC3_PIN 3

//Globals for keeping track of current ESC state
int escPins[] = {ESC0_PIN,ESC1_PIN,ESC2_PIN,ESC3_PIN};
int escOut[]  = {0,0,0,0}; //all off initially

//start interrupts and set up ESC pins
void ESCSetup(){
    //Configure pads for GPIO
    gpio_pad_select_gpio(ESC0_PIN);
    gpio_pad_select_gpio(ESC1_PIN);
    gpio_pad_select_gpio(ESC2_PIN);
    gpio_pad_select_gpio(ESC3_PIN);
    //Set as Output
    gpio_set_direction(ESC0_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESC1_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESC2_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ESC3_PIN, GPIO_MODE_OUTPUT);
    //Initially 0
    gpio_set_level(ESC0_PIN, 0);
    gpio_set_level(ESC1_PIN, 0);
    gpio_set_level(ESC2_PIN, 0);
    gpio_set_level(ESC3_PIN, 0);


}

//takes in esc #0-3 and power from  0-100
//sets up PPM output to corresponding ESC
void ESC_Out(int esc, int power){

}
