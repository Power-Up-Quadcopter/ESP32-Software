
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




//  NOTE: dont use these functions outside of drivers, use driver specific functions


/** I2C initialization function. Call this before using any I2C devices.
 *
 * @return Error code returned by the driver initialization
*/
esp_err_t I2C_Init();



/** Test function for higher i2c frequency writing.
 *
 * @param regAddr The address of the 8 bit register you want to access
*/
void I2C_Write8_fast(uint8_t addrW, uint8_t data, uint8_t regAddr);

/** Generic i2c function for writing to 8 bit registers.
 *
 * @param addrW The I2C address of the device being accessed
 * @param data 8 bit data value to write
 * @param regAddr The address of the 8 bit register you want to access
*/
void I2C_Write8(uint8_t addrW, uint8_t data, uint8_t regAddr);

/** Arduino specific i2c function for sending several 8 bit values.
 *
 * @param addrW The I2C address of the Arduino being accessed
 * @param data A pointer to array of 8 bit numbers
 * @param dataLen The length of the data array
*/
void I2C_Write8_Nano(uint8_t addrW, uint8_t* data, uint8_t dataLen);

/** Generic I2C function for writing to 16 bit registers.
 *
 * @param addrW The I2C address of the device being accessed
 * @param data 16 bit data value to write
 * @param regAddr The address of the 16 bit register you want to access
*/
void I2C_Write16(uint8_t addrW, uint16_t data, uint8_t regAddr);

/** Generic I2C function for reading 8 bit registers.
 *
 * @param addrW The I2C address of the device being accessed
 * @param regAddr The address of the 8 bit register you want to access
 *
 * @return The 8 bit value contained within the register.
*/
uint8_t I2C_Read8(uint8_t addrW, uint8_t regAddr);

/** Generic I2C function for reading 16 bit registers.
 *
 * @param addrW The I2C address of the device being accessed
 * @param regAddr The address of the 16 bit register you want to access
 *
 * @return The 16 bit value contained within the register.
*/
uint16_t I2C_Read16(uint8_t addrW, uint8_t regAddr);

#endif //ESP32_SOFTWARE_I2C_GENERAL_H
