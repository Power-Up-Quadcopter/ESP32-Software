
#include <stdint.h>
#include "I2C_General.h"
#include "Magnetometer.h"

#define magAddrW 60

void Mag_Write8(uint8_t data, uint8_t regAddr){
    I2C_Write8(magAddrW, data, regAddr);
}


void Mag_Write16(uint16_t data, uint8_t regAddr){
    I2C_Write16(magAddrW, data, regAddr);
}

uint8_t Mag_Read8(uint8_t regAddr){
    uint8_t out = I2C_Read8(magAddrW, regAddr);
    return out;
}


uint16_t Mag_Read16(uint8_t regAddr){
    uint16_t out = I2C_Read16(magAddrW, regAddr);
    return out;
}
