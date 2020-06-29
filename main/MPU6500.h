
#ifndef ESP32_SOFTWARE_MPU6500_H
#define ESP32_SOFTWARE_MPU6500_H

//put useful defines for register addresses here

//write an 8bit value to specific register
void MPU_Write8(uint8_t data, uint8_t regAddr);

//write a 16 bit value to two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
void MPU_Write16(uint16_t data, uint8_t regAddr);

//read an 8bit value from specific register
uint8_t MPU_Read8(uint8_t regAddr);

//read a 16 bit value from two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
uint16_t MPU_Read16(uint8_t regAddr);

#endif //ESP32_SOFTWARE_MPU6500_H
