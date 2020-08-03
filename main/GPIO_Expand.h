
#ifndef ESP32_SOFTWARE_GPIO_EXPAND_H
#define ESP32_SOFTWARE_GPIO_EXPAND_H

typedef enum{
    GPA0 = 0,
    GPA1 = 1,
    GPA2 = 2,
    GPA3 = 3,
    GPA4 = 4,
    GPA5 = 5,
    GPA6 = 6,
    GPA7 = 7,
    GPB0 = 8,
    GPB1 = 9,
    GPB2 = 10,
    GPB3 = 11,
    GPB4 = 12,
    GPB5 = 13,
    GPB6 = 14,
    GPB7 = 15
}pin_t;


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
