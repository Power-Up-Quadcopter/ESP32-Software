
#include <stdint.h>
#include "I2C_General.h"
#include "GPIO_Expand.h"

#define expandAddrW 0x20

namespace Expand {

    void init() {
        // Set all pins default to output
        I2C_Write8(expandAddrW, 0x00, 0x00);
        I2C_Write8(expandAddrW, 0x00, 0x01);
        //Configure pin behavior
        I2C_Write8(expandAddrW, 0x00, 0x0A); //Check
        I2C_Write8(expandAddrW, 0x00, 0x0B); //Check
        // Set all PUR default to off
        I2C_Write8(expandAddrW, 0x00, 0x0C);
        I2C_Write8(expandAddrW, 0x00, 0x0D);
    }

    void write8(uint8_t data, uint8_t regAddr) {
        I2C_Write8(expandAddrW, data, regAddr);
    }


    void write16(uint16_t data, uint8_t regAddr) {
        I2C_Write16(expandAddrW, data, regAddr);
    }

    uint8_t read8(uint8_t regAddr) {
        uint8_t out = I2C_Read8(expandAddrW, regAddr);
        return out;
    }


    uint16_t read16(uint8_t regAddr) {
        uint16_t out = I2C_Read16(expandAddrW, regAddr);
        return out;
    }

}