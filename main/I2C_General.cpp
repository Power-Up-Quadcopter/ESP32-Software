
#include "I2C_General.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"



static i2c_port_t i2c_port = I2C_NUM_0;

//common I2C functions that we will use
//Look here for documentation:
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html#

//Initialize the I2C port we will be using
esp_err_t I2C_Init(){
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = CONFIG_I2C_MASTER_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = CONFIG_I2C_MASTER_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = CONFIG_I2C_MASTER_FREQUENCY;
    i2c_param_config(i2c_port, &conf);
    return i2c_driver_install(i2c_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);

//    i2c_config_t conf2;
//    conf2.mode = I2C_MODE_MASTER;
//    conf2.sda_io_num = CONFIG_I2C_MASTER_SDA;
//    conf2.sda_pullup_en = GPIO_PULLUP_ENABLE;
//    conf2.scl_io_num = CONFIG_I2C_MASTER_SCL;
//    conf2.scl_pullup_en = GPIO_PULLUP_ENABLE;
//    conf2.master.clk_speed = 1000000;
//    i2c_param_config(I2C_NUM_1, &conf2);
//    return i2c_driver_install(I2C_NUM_1, conf2.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void I2C_Write8_fast(uint8_t addrW, uint8_t data, uint8_t regAddr){
    I2C_Write8(addrW, data, regAddr);
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, addrW << 1 | WRITE_BIT, ACK_CHECK_EN);
//    i2c_master_write_byte(cmd, regAddr, ACK_CHECK_EN);
//    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
//    i2c_master_stop(cmd);
//
//    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 50 / portTICK_RATE_MS);
//    i2c_cmd_link_delete(cmd);
//
//    if(ret != 0) printf("I2C write8_fast error: w %s\n", esp_err_to_name(ret));
}

void I2C_Write8(uint8_t addrW, uint8_t data, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, addrW << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, regAddr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 50 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if(ret != 0) printf("I2C error: %s\n", esp_err_to_name(ret));

}


void I2C_Write8_Nano(uint8_t addrW, uint8_t* data, uint8_t dataLen){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, addrW << 1 | WRITE_BIT, ACK_CHECK_EN);
    for(int i=0; i<dataLen; i++){
        i2c_master_write_byte(cmd, data[i], ACK_CHECK_EN);
    }
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 50 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if(ret != 0) printf("I2C error: %s\n", esp_err_to_name(ret));

}


void I2C_Write16(uint8_t addrW, uint16_t data, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t toSend[4] = {addrW, regAddr, *(((uint8_t*) &data)+1), *(((uint8_t*) &data))};

    i2c_master_start(cmd);
    i2c_master_write(cmd, toSend, 4, 1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
}

uint8_t I2C_Read8(uint8_t deviceAddress, uint8_t regAddr){
    uint8_t data;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, deviceAddress << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, regAddr, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, deviceAddress << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read(cmd, &data, 1, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_port, cmd, 50 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if(ret != 0) printf("I2C error: %s\n", esp_err_to_name(ret));

    return data;
}


uint16_t I2C_Read16(uint8_t addrW, uint8_t regAddr){
    return (I2C_Read8(addrW, regAddr) << 8u) | (I2C_Read8(addrW, regAddr+1));
}


