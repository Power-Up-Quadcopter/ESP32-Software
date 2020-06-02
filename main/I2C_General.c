
#include "I2C_General.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"


#define WAIT_TIME 50

//common I2C functions that we will use
//Look here for documentation:
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html#

static i2c_port_t I2Cport = I2C_NUM_0;

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

//send a byte to a device
void Send_Byte(uint8_t addr, uint8_t byte){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,addr,1);
    i2c_master_write(cmd,&byte,1,1);
    i2c_master_stop(cmd);

    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

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

