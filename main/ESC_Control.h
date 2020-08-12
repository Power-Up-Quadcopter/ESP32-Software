
#ifndef ESP32_SOFTWARE_ESC_CONTROL_H
#define ESP32_SOFTWARE_ESC_CONTROL_H

#include <stdint.h>


#define ESC0_PIN 32
#define ESC1_PIN 33
#define ESC2_PIN 25
#define ESC3_PIN 26

typedef enum{
    FrontR = 0,
    FrontL = 1,
    BackL = 2,
    BackR = 3
}ESC_PIN_t;


namespace Esc {


    extern uint8_t powers[];

    /** Initializes ESC output in an off state
    */
    void init();

    /** Sets all ESC output values. Details follow
     *  here.
     *  @param newPowers Pointer to array of 4 bytes which will replace the current throttle values
    */
    void setAll(const uint8_t* newPowers);


    /** Sets up PWM output to corresponding ESC
     *  @param esc ESC to modify
     *  @param power Power from 0-255 to set ESC throttle to
    */
    void set(ESC_PIN_t esc, uint8_t power);
}
#endif //ESP32_SOFTWARE_ESC_CONTROL_H
