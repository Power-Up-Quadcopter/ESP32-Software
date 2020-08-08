
#ifndef ESP32_SOFTWARE_I2C_GENERAL_H
#define ESP32_SOFTWARE_I2C_GENERAL_H

#include <driver/i2c.h>
#include "stdint.h"

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define WRITE_BIT I2C_MASTER_WRITE  /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ    /*!< I2C master read */
#define ACK_CHECK_EN 0x1            /*!< I2C master will check ack from slave*/
#define ACK_VAL 0x0                 /*!< I2C ack value */
#define NACK_VAL 0x1                /*!< I2C nack value */
#define CONFIG_I2C_MASTER_SCL GPIO_NUM_22
#define CONFIG_I2C_MASTER_SDA GPIO_NUM_21
#define CONFIG_I2C_MASTER_FREQUENCY 400000
#define WAIT_TIME 50
#define I2Cport I2C_NUM_0
#define GPIOport I2C_NUM_1

esp_err_t I2C_Init();

//  ---dont use these functions outside of drivers, use driver specific functions---

void I2C_Write8_fast(uint8_t addrW, uint8_t data, uint8_t regAddr);
//write an 8bit value to specific register
void I2C_Write8(uint8_t addrW, uint8_t data, uint8_t regAddr);

//write a 16 bit value to two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
void I2C_Write16(uint8_t addrW, uint16_t data, uint8_t regAddr);

//read an 8bit value from specific register
uint8_t I2C_Read8(uint8_t addrW, uint8_t regAddr);

//read a 16 bit value from two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
uint16_t I2C_Read16(uint8_t addrW, uint8_t regAddr);

#endif //ESP32_SOFTWARE_I2C_GENERAL_H
