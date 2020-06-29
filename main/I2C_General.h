
#ifndef ESP32_SOFTWARE_I2C_GENERAL_H
#define ESP32_SOFTWARE_I2C_GENERAL_H

#include <driver/i2c.h>
#include "stdint.h"

#define SCL_PIN 36
#define SDA_PIN 33
#define WAIT_TIME 50
#define I2C_Freq 100000
#define I2Cport I2C_NUM_0


void I2C_Init();


//  ---dont use these functions outside of drivers, use driver specific functions---

//write an 8bit value to specific register
void I2C_Write8(uint8_t addrW, uint8_t data, uint8_t regAddr);

//write a 16 bit value to two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
void I2C_Write16(uint8_t addrW, uint16_t data, uint8_t regAddr);

//read an 8bit value from specific register
uint8_t I2C_Read8(uint8_t addrW, uint8_t regAddr);

//read a 16 bit value from two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
uint16_t I2C_Read16(uint8_t addrW, uint8_t regAddr);

#endif //ESP32_SOFTWARE_I2C_GENERAL_H
