
#ifndef ESP32_SOFTWARE_MAGNETOMETER_H
#define ESP32_SOFTWARE_MAGNETOMETER_H

//put useful defines for register addresses here
#define MAG_STATUS 0x00
#define MAG_WHOAMI 0x0D
#define MAG_XYZ_DATA_CFG 0x0E
#define MAG_CTRL_REG1 0x2A
#define MAG_CTRL_REG2 0x2B
#define MAG_M_DR_STATUS 0x32
#define MAG_OUT_X 0x33
#define MAG_OUT_Y 0x35
#define MAG_OUT_Z 0x37
#define MAG_TEMP 0x51
#define MAG_M_CTRL_REG1 0x5B
#define MAG_M_CTRL_REG2 0x5C
#define MAG_WHOAMI_VAL 0xC7


typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t temp;
} MagData;

namespace Mag{

    ///True once magnetometer is done setting up.
    extern uint8_t magReady;

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


    /** Returns magnetometer data.
     *
     * @return MagData A struct containing x,y,z field strengths and temperature reading
    */
    MagData* getData()

}

#endif //ESP32_SOFTWARE_MAGNETOMETER_H
