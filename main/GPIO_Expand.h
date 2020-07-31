
#ifndef ESP32_SOFTWARE_GPIO_EXPAND_H
#define ESP32_SOFTWARE_GPIO_EXPAND_H

namespace Expand{

    //put useful defines for register addresses here
    #define IODIRA 0x00
    #define IODIRB 0x01
    #define GPPUA 0x0C
    #define GPPUB 0x0D
    #define GPIOA 0x12
    #define GPIOB 0x13

    extern uint8_t portA;
    extern uint8_t portB;

    typedef enum{
        PIN0_A = 0,
        PIN1_A = 1,
        PIN2_A = 2,
        PIN3_A = 3,
        PIN4_A = 4,
        PIN5_A = 5,
        PIN6_A = 6,
        PIN7_A = 7,
        PIN0_B = 8,
        PIN1_B = 9,
        PIN2_B = 10,
        PIN3_B = 11,
        PIN4_B = 12,
        PIN5_B = 13,
        PIN6_B = 14,
        PIN7_B = 15
    }pin_t;


    //Initialize all to output, PUR off, level low
    void init();

    //write an 8bit value to specific register
    void write8(uint8_t data, uint8_t regAddr);

    //write a 16 bit value to two sequential registers
    //takes the high bit register b/c mpu auto increments register after r/w
    void write16(uint16_t data, uint8_t regAddr);

    //read an 8bit value from specific register
    uint8_t read8(uint8_t regAddr);

    //read a 16 bit value from two sequential registers
    //takes the high bit register b/c mpu auto increments register after r/w
    uint16_t read16(uint8_t regAddr);

    //set a single pin's direction
    void setPinDir(pin_t pin, bool dir);

    //set a single pin to high or low
    void setPinData(pin_t pin, bool on);

    //set all pins' directions
    void setAllDir(uint8_t newPortA, uint8_t newPortB);

    //set all pins' level
    void setAllData(uint8_t newPortA, uint8_t newPortB);

}
#endif //ESP32_SOFTWARE_GPIO_EXPAND_H
