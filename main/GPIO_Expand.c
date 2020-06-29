
#include <stdint.h>
#include "I2C_General.h"
#include "GPIO_Expand.h"

#define expandAddrW 64

void expand_Write8(uint8_t data, uint8_t regAddr){
    I2C_Write8(expandAddrW, data, regAddr);
}


void expand_Write16(uint16_t data, uint8_t regAddr){
    I2C_Write16(expandAddrW, data, regAddr);
}

uint8_t expand_Read8(uint8_t regAddr){
    uint8_t out = I2C_Read8(expandAddrW, regAddr);
    return out;
}


uint16_t expand_Read16(uint8_t regAddr){
    uint16_t out = I2C_Read16(expandAddrW, regAddr);
    return out;
}
