
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
} MAGDATA_t;

extern MAGDATA_t magData;
extern uint8_t magReady;

void Mag_Init();

//write an 8bit value to specific register
void Mag_Write8(uint8_t data, uint8_t regAddr);

//write a 16 bit value to two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
void Mag_Write16(uint16_t data, uint8_t regAddr);

//read an 8bit value from specific register
uint8_t Mag_Read8(uint8_t regAddr);

//read a 16 bit value from two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
uint16_t Mag_Read16(uint8_t regAddr);

#endif //ESP32_SOFTWARE_MAGNETOMETER_H
