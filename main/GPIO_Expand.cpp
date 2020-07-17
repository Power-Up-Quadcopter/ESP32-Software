
#include <stdint.h>
#include "I2C_General.h"
#include "GPIO_Expand.h"

#define expanderAddress 0x20

void GPIO_Expand::initialize() {
    // Set all pins default to output
    I2C_Write8(expanderAddress, 0x00, 0x00);
    I2C_Write8(expanderAddress, 0x01, 0x00);
    //Configure pin behavior
    I2C_Write8(expanderAddress, 0x0A, 0x00); //Check
    I2C_Write8(expanderAddress, 0x0B, 0x00); //Check
    // Set all PUR default to off
    I2C_Write8(expanderAddress, 0x0C, 0x00);
    I2C_Write8(expanderAddress, 0x0D, 0x00);
}

void GPIO_Expand::set_dir_bit(uint8_t pin, uint8_t bit){

    uint8_t port = -1;
    // Determines if Port A or Port B
    if(pin == this->A0 || pin == this->A1 || pin == this->A2 || pin == this->A3 || pin == this->A4 || pin == this->A5 || pin == this->A6 || pin == this->A7)
        port = 0x00;
    else if(pin == this->B0 || pin == this->B1 || pin == this->B2 || pin == this->B3 || pin == this->B4 || pin == this->B5 || pin == this->B6 || pin == this->B7)
        port = 0x01;

    if(port == (uint8_t)-1)
        return;

    uint8_t data = I2C_Read8(expanderAddress, port);
    // Sets the correct bit
    if(pin == this->A0 || pin == this->B0);
    else if(pin == this->A1 || pin == this->B1)
        bit = bit << 1u;
    else if(pin == this->A2 || pin == this->B2)
        bit = bit << 2u;
    else if(pin == this->A3 || pin == this->B3)
        bit = bit << 3u;
    else if(pin == this->A4 || pin == this->B4)
        bit = bit << 4u;
    else if(pin == this->A5 || pin == this->B5)
        bit = bit << 5u;
    else if(pin == this->A6 || pin == this->B6)
        bit = bit << 6u;
    else if(pin == this->A7 || pin == this->B7)
        bit = bit << 7u;
    data |= bit;
    data &= bit;
    // Writes the new value to the dir register
    I2C_Write8(expanderAddress, data, port);
}

void GPIO_Expand::set_dir_reg(uint8_t byte, uint8_t port){
    // Writes to the entire dir register
    if(port == this->A)
        I2C_Write8(expanderAddress, byte, 0x00);
    else if(port == this->B)
        I2C_Write8(expanderAddress, byte, 0x01);
}

uint8_t GPIO_Expand::get_dir_bit(uint8_t pin){

    uint8_t port = -1;
    // Determines if Port A or Port B
    if(pin == this->A0 || pin == this->A1 || pin == this->A2 || pin == this->A3 || pin == this->A4 || pin == this->A5 || pin == this->A6 || pin == this->A7)
        port = 0x00;
    else if(pin == this->B0 || pin == this->B1 || pin == this->B2 || pin == this->B3 || pin == this->B4 || pin == this->B5 || pin == this->B6 || pin == this->B7)
        port = 0x01;

    if(port == (uint8_t)-1)
        return -1;
    // reads bit
    if(pin == this->A0)
        return (I2C_Read8(expanderAddress, port) & 0x01u);
    else if(pin == this->A1)
        return (I2C_Read8(expanderAddress, port) & 0x02u) >> 1;
    else if(pin == this->A2)
        return (I2C_Read8(expanderAddress, port) & 0x04u) >> 2;
    else if(pin == this->A3)
        return (I2C_Read8(expanderAddress, port) & 0x08u) >> 3;
    else if(pin == this->A4)
        return (I2C_Read8(expanderAddress, port) & 0x10u) >> 4;
    else if(pin == this->A5)
        return (I2C_Read8(expanderAddress, port) & 0x20u) >> 5;
    else if(pin == this->A6)
        return (I2C_Read8(expanderAddress, port) & 0x40u) >> 6;
    else if(pin == this->A7)
        return (I2C_Read8(expanderAddress, port) & 0x80u) >> 7;
    else
        return -1;
}

uint8_t GPIO_Expand::get_dir_reg(uint8_t port){
    // Reads the entire dir register
    if(port == this->A)
        return I2C_Read8(expanderAddress, 0x00);
    else if(port == this->B)
        return I2C_Read8(expanderAddress, 0x01);
    return -1;
}

void GPIO_Expand::set_pur_bit(uint8_t pin, uint8_t bit){

    uint8_t port = -1;
    // Determines if Port A or Port B
    if(pin == this->A0 || pin == this->A1 || pin == this->A2 || pin == this->A3 || pin == this->A4 || pin == this->A5 || pin == this->A6 || pin == this->A7)
        port = 0x0C;
    else if(pin == this->B0 || pin == this->B1 || pin == this->B2 || pin == this->B3 || pin == this->B4 || pin == this->B5 || pin == this->B6 || pin == this->B7)
        port = 0x0D;

    if(port == (uint8_t)-1)
        return;

    uint8_t data = I2C_Read8(expanderAddress, port);
    // Sets the correct bit
    if(pin == this->A0 || pin == this->B0);
    else if(pin == this->A1 || pin == this->B1)
        bit = bit << 1u;
    else if(pin == this->A2 || pin == this->B2)
        bit = bit << 2u;
    else if(pin == this->A3 || pin == this->B3)
        bit = bit << 3u;
    else if(pin == this->A4 || pin == this->B4)
        bit = bit << 4u;
    else if(pin == this->A5 || pin == this->B5)
        bit = bit << 5u;
    else if(pin == this->A6 || pin == this->B6)
        bit = bit << 6u;
    else if(pin == this->A7 || pin == this->B7)
        bit = bit << 7u;
    data |= bit;
    data &= bit;
    //Writes the new value to the pur register
    I2C_Write8(expanderAddress, data, port);
}

void GPIO_Expand::set_pur_reg(uint8_t byte, uint8_t port){
    // Reads entire pur register
    if(port == this->A)
        I2C_Write8(expanderAddress, byte, 0x0C);
    else if(port == this->B)
        I2C_Write8(expanderAddress, byte, 0x0D);
}

uint8_t GPIO_Expand::read_bit(uint8_t pin) {
    uint8_t port = -1;
    // Determines if Port A or Port B
    if(pin == this->A0 || pin == this->A1 || pin == this->A2 || pin == this->A3 || pin == this->A4 || pin == this->A5 || pin == this->A6 || pin == this->A7)
        port = 0x13;
    else if(pin == this->B0 || pin == this->B1 || pin == this->B2 || pin == this->B3 || pin == this->B4 || pin == this->B5 || pin == this->B6 || pin == this->B7)
        port = 0x14;

    if(port == (uint8_t)-1)
        return -1;
    // reads bit
    if(pin == this->A0 || pin == this->B0)
        return (I2C_Read8(expanderAddress, port) & 0x01);
    else if(pin == this->A1 || pin == this->B1)
        return (I2C_Read8(expanderAddress, port) & 0x02) >> 1;
    else if(pin == this->A2 || pin == this->B2)
        return (I2C_Read8(expanderAddress, port) & 0x04) >> 2;
    else if(pin == this->A3 || pin == this->B3)
        return (I2C_Read8(expanderAddress, port) & 0x08) >> 3;
    else if(pin == this->A4 || pin == this->B4)
        return (I2C_Read8(expanderAddress, port) & 0x10) >> 4;
    else if(pin == this->A5 || pin == this->B5)
        return (I2C_Read8(expanderAddress, port) & 0x20) >> 5;
    else if(pin == this->A6 || pin == this->B6)
        return (I2C_Read8(expanderAddress, port) & 0x40) >> 6;
    else if(pin == this->A7 || pin == this->B7)
        return (I2C_Read8(expanderAddress, port) & 0x80) >> 7;
    else
        return -1;
}

uint8_t GPIO_Expand::read_port(uint8_t port){
    // Reads entire port
    if(port == this->A)
        return I2C_Read8(expanderAddress, 0x13);
    else if(port == this->B)
        return I2C_Read8(expanderAddress, 0x14);
    return -1;
}

void GPIO_Expand::set_bit(uint8_t pin, uint8_t bit){

    uint8_t port = -1;
    // Determines if Port A or Port B
    if(pin == this->A0 || pin == this->A1 || pin == this->A2 || pin == this->A3 || pin == this->A4 || pin == this->A5 || pin == this->A6 || pin == this->A7)
        port = 0x12;
    else if(pin == this->B0 || pin == this->B1 || pin == this->B2 || pin == this->B3 || pin == this->B4 || pin == this->B5 || pin == this->B6 || pin == this->B7)
        port = 0x13;

    if(port == (uint8_t)-1)
        return;

    uint8_t data = I2C_Read8(expanderAddress, port);
    // Sets the correct bit and checks the direction
    if(pin == this->A0) {
        if (this->ADIR & 0x01)
            return;
    }
    else if(pin == this->A1){
        if(this->ADIR & 0x02)
            return;
        bit = bit << 1u;
    }
    else if(pin == this->A2){
        if(this->ADIR & 0x04)
            return;
        bit = bit << 2u;
    }
    else if(pin == this->A3){
        if(this->ADIR & 0x08)
            return;
        bit = bit << 3u;
    }
    else if(pin == this->A4){
        if(this->ADIR & 0x10)
            return;
        bit = bit << 4u;
    }
    else if(pin == this->A5){
        if(this->ADIR & 0x20)
            return;
        bit = bit << 5u;
    }
    else if(pin == this->A6){
        if(this->ADIR & 0x40)
            return;
        bit = bit << 6u;
    }
    else if(pin == this->A7){
        if(this->ADIR & 0x80)
            return;
        bit = bit << 7u;
    }
    if(pin == this->B0) {
        if (this->BDIR & 0x01)
            return;
    }
    else if(pin == this->B1){
        if(this->BDIR & 0x02)
            return;
        bit = bit << 1u;
    }
    else if(pin == this->B2){
        if(this->BDIR & 0x04)
            return;
        bit = bit << 2u;
    }
    else if(pin == this->B3){
        if(this->BDIR & 0x08)
            return;
        bit = bit << 3u;
    }
    else if(pin == this->B4){
        if(this->BDIR & 0x10)
            return;
        bit = bit << 4u;
    }
    else if(pin == this->B5){
        if(this->BDIR & 0x20)
            return;
        bit = bit << 5u;
    }
    else if(pin == this->B6){
        if(this->BDIR & 0x40)
            return;
        bit = bit << 6u;
    }
    else if(pin == this->B7){
        if(this->BDIR & 0x80)
            return;
        bit = bit << 7u;
    }

    data |= bit;
    data &= bit;
    // Writes the new value to the GPIO port register
    I2C_Write8(expanderAddress, data, port);
}
