
#include <stdint.h>
#include "MPU6500.h"
#include "I2C_General.h"

#define mpuAddrW 208

void MPU_Write8(uint8_t data, uint8_t regAddr){
    I2C_Write8(mpuAddrW, data, regAddr);
}


void MPU_Write16(uint16_t data, uint8_t regAddr){
    I2C_Write16(mpuAddrW, data, regAddr);
}

uint8_t MPU_Read8(uint8_t regAddr){
    uint8_t out = I2C_Read8(mpuAddrW, regAddr);
    return out;
}


uint16_t MPU_Read16(uint8_t regAddr){
    uint16_t out = I2C_Read16(mpuAddrW, regAddr);
    return out;
}
