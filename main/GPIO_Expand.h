
#ifndef ESP32_SOFTWARE_GPIO_EXPAND_H
#define ESP32_SOFTWARE_GPIO_EXPAND_H

//put useful defines for register addresses here

//write an 8bit value to specific register
void expand_Write8(uint8_t data, uint8_t regAddr);

//write a 16 bit value to two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
void expand_Write16(uint16_t data, uint8_t regAddr);

//read an 8bit value from specific register
uint8_t expand_Read8(uint8_t regAddr);

//read a 16 bit value from two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
uint16_t expand_Read16(uint8_t regAddr);

#endif //ESP32_SOFTWARE_GPIO_EXPAND_H
