
#ifndef ESP32_SOFTWARE_GPIO_EXPAND_H
#define ESP32_SOFTWARE_GPIO_EXPAND_H

namespace Expand{
//put useful defines for register addresses here
#define IODIRA 0x00
#define IODIRB 0x01
#define GPPUA 0x0C
#define GPPUB 0x0D
#define GPIOA 0x12
#define GPIOB 0x13

//Initialize all to output, PUR off
void init();

//write an 8bit value to specific register
void write8(uint8_t data, uint8_t regAddr);

//write a 16 bit value to two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
void write16(uint16_t data, uint8_t regAddr);

//read an 8bit value from specific register
uint8_t read8(uint8_t regAddr);

//read a 16 bit value from two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
uint16_t read16(uint8_t regAddr);

}
#endif //ESP32_SOFTWARE_GPIO_EXPAND_H
