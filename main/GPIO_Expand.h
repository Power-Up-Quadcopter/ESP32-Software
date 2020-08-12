
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


    extern uint8_t portA; /*!< Current levels off all portA pins */
    extern uint8_t portB; /*!< Current levels off all portB pins */



    /** Component initialization function. Call before using this component.
    */
    void init();

    /** Component specific I2C function for reading 8 bit registers.
     *
     * @param data The 8 bit data value to write
     * @param regAddr The address of the 8 bit register you want to access
    */
    void write8(uint8_t data, uint8_t regAddr);

    /** Component specific I2C function for writing to 16 bit registers.
     *
     * @param data The 16 bit data value to write
     * @param regAddr The address of the 16 bit register you want to access
     *
    */
    void write16(uint16_t data, uint8_t regAddr);

    /** Component specific I2C function for reading 8 bit registers.
     *
     * @param regAddr The address of the 8 bit register you want to access
     *
     * @return The 8 bit value contained within the register.
    */
    uint8_t read8(uint8_t regAddr);

    /** Component specific I2C function for reading 16 bit registers.
     *
     * @param regAddr The address of the 16 bit register you want to access
     *
     * @return The 16 bit value contained within the register.
    */
    uint16_t read16(uint8_t regAddr);


    /** Set the direction of a single pin.
     *
     * @param pin The pin you want to change
     * @param dir Pin direction, 0 for output, 1 for input
    */
    void setPinDir(pin_t pin, bool dir);

    /** Set a single output pin to high or low.
     *
     * @param pin The pin you want to change
     * @param on Pin level, 0 for low, 1 for high
    */
    void setPinData(pin_t pin, bool on);

    /** Set the direction of a all pins.
     *
     * @param newPortA The direction of all port A pins.
     * @param newPortB The direction of all port B pins.
    */
    void setAllDir(uint8_t newPortA, uint8_t newPortB);

    /** Set the output level of all pins
     *
     * @param newPortA The levels of all port A pins.
     * @param newPortB The levels of all port B pins.
    */
    void setAllData(uint8_t newPortA, uint8_t newPortB);

}
#endif //ESP32_SOFTWARE_GPIO_EXPAND_H
