
#ifndef ESP32_SOFTWARE_MAGNETOMETER_H
#define ESP32_SOFTWARE_MAGNETOMETER_H

//put useful defines for register addresses here
#define FXOS8700CQ_STATUS 0x00
#define FXOS8700CQ_WHOAMI 0x0D
#define FXOS8700CQ_XYZ_DATA_CFG 0x0E
#define FXOS8700CQ_CTRL_REG1 0x2A
#define FXOS8700CQ_M_CTRL_REG1 0x5B
#define FXOS8700CQ_M_CTRL_REG2 0x5C
#define FXOS8700CQ_WHOAMI_VAL 0xC7

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
