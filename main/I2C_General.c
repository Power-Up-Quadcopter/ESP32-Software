
#include "I2C_General.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"




//common I2C functions that we will use
//Look here for documentation:
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html#



//Initialize the I2C port we will be using
void I2C_Init(){
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_Freq;
    ESP_ERROR_CHECK(i2c_param_config(I2Cport,&conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2Cport,I2C_MODE_MASTER,0,0,0));
}

//write a byte to a device
void Write_Byte(uint8_t addr, uint8_t byte){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addr,1);
    i2c_master_write(cmd,&byte,1,1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

//write n bytes to a device
void Write_n_Bytes(uint8_t addr, uint8_t* bytes, int n){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addr,1);
    i2c_master_write(cmd,bytes,n,1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

//read one byte from a device
uint8_t Read_Byte(uint8_t addr){
    uint8_t byte;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addr,1);
    i2c_master_read(cmd,&byte,1,0);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);

    return byte;
}


//read n bytes from a device
void Read_n_Bytes(uint8_t addr, uint8_t* bytes, int n){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, addr, 1);
    i2c_master_read(cmd,bytes, n-1, 1);
    i2c_master_read(cmd,bytes + n-1, 1, 0);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);

}

void I2C_Write8(uint8_t addrW, uint8_t data, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t toSend[3] = {addrW, regAddr, data};

    i2c_master_start(cmd);
    i2c_master_write(cmd, toSend, 3, 1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));

}


void I2C_Write16(uint8_t addrW, uint16_t data, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t toSend[4] = {addrW, regAddr, *(((uint8_t*) &data)+1), *(((uint8_t*) &data))};

    i2c_master_start(cmd);
    i2c_master_write(cmd, toSend, 4, 1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
}

uint8_t I2C_Read8(uint8_t addrW, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint8_t out;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addrW, 1);
    i2c_master_write_byte(cmd,regAddr, 1);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addrW + 1, 1);
    i2c_master_read_byte(cmd,&out,I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));

    i2c_master_stop(cmd);
    return out;
}


uint16_t I2C_Read16(uint8_t addrW, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    uint16_t out;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addrW, 1);
    i2c_master_write_byte(cmd,regAddr, 1);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addrW + 1, 1);
    i2c_master_read(cmd, (((uint8_t*) &out)+1), 2, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));

    i2c_master_stop(cmd);
    return out;
}
