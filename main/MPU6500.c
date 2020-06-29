
#include <stdint.h>
#include <driver/i2c.h>
#include "MPU6500.h"
#include "I2C_General.h"

#define mpuAddrW 208
#define mpuAddrR 209



void MPU_Write8(uint8_t data, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t toSend[3] = {mpuAddrW, regAddr, data};

    i2c_master_start(cmd);
    i2c_master_write(cmd, toSend, 3, 1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));

}


void MPU_Write16(uint16_t data, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t toSend[4] = {mpuAddrW, regAddr, *(((uint8_t*) &data)+1), *(((uint8_t*) &data))};

    i2c_master_start(cmd);
    i2c_master_write(cmd, toSend, 4, 1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
}

uint8_t MPU_Read8(uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t out;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,mpuAddrW,1);
    i2c_master_write_byte(cmd,regAddr,1);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,mpuAddrR,1);
    i2c_master_read_byte(cmd,&out,I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));

    i2c_master_stop(cmd);
    return out;
}


uint16_t MPU_Read16(uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint16_t out;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,mpuAddrW,1);
    i2c_master_write_byte(cmd,regAddr,1);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,mpuAddrR,1);
    i2c_master_read(cmd, (((uint8_t*) &out)+1), 2, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));

    i2c_master_stop(cmd);
    return out;
}
