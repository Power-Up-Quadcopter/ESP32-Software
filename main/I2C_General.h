
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

void Write_Byte(uint8_t addr, uint8_t byte);
void Write_n_Bytes(uint8_t addr, uint8_t* bytes, int n);
uint8_t Read_Byte(uint8_t addr);
void Read_n_Bytes(uint8_t addr, uint8_t* bytes, int n);

#endif //ESP32_SOFTWARE_I2C_GENERAL_H
