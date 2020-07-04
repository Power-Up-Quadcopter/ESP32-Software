
#include <stdint.h>
#include "I2C_General.h"
#include "Pressure.h"

#define pressureAddrW 238

void Pressure_Write8(uint8_t data, uint8_t regAddr){
    I2C_Write8(pressureAddrW, data, regAddr);
}


void Pressure_Write16(uint16_t data, uint8_t regAddr){
    I2C_Write16(pressureAddrW, data, regAddr);
}

uint8_t Pressure_Read8(uint8_t regAddr){
    uint8_t out = I2C_Read8(pressureAddrW, regAddr);
    return out;
}


uint16_t Pressure_Read16(uint8_t regAddr){
    uint16_t out = I2C_Read16(pressureAddrW, regAddr);
    return out;
}
