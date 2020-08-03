
#include <stdint.h>
#include "I2C_General.h"
#include "GPIO_Expand.h"

#define expandAddrW 0x20

namespace Expand {

    uint8_t portA;
    uint8_t portB;

    void init() {
        // Set all pins default to output
        I2C_Write8(expandAddrW, 0x00, IODIRA);
        I2C_Write8(expandAddrW, 0x00, IODIRB);
        //Configure pin behavior
        I2C_Write8(expandAddrW, 0x00, 0x0A); //Check
        I2C_Write8(expandAddrW, 0x00, 0x0B); //Check
        // Set all PUR default to off
        I2C_Write8(expandAddrW, 0x00, GPPUA);
        I2C_Write8(expandAddrW, 0x00, GPPUB);
        //all pins low
        I2C_Write8(expandAddrW, 0x00, GPIOA);
        I2C_Write8(expandAddrW, 0x00, GPIOB);

        portA = 0;
        portB = 0;
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

    void setPinDir(pin_t pin, bool dir) {
        uint8_t addr = (pin < 8) ? IODIRA : IODIRB;
        uint8_t offset = (pin < 8) ? pin : pin - 8;
        uint8_t *port = (pin < 8) ? &portA : &portB;

        *port = (*port & ~(0x1u << offset)) | (dir << offset);
        write8(*port, addr);
    }

    void setPinData(pin_t pin, bool on){
        uint8_t addr = (pin < 8) ? GPIOA : GPIOB;
        uint8_t offset = (pin < 8) ? pin : pin-8;
        uint8_t *port = (pin < 8) ? &portA : &portB;

        *port = (*port & ~(0x1u << offset)) | (on << offset);
        write8(*port, addr);
    }

    void setAllDir(uint8_t newPortA, uint8_t newPortB){
        portA = newPortA;
        portB = newPortB;
        write8(portA, IODIRA);
        write8(portB, IODIRB);
    }

    void setAllData(uint8_t newPortA, uint8_t newPortB){
        portA = newPortA;
        portB = newPortB;
        write8(portA, GPIOA);
        write8(portB, GPIOB);
    }

}