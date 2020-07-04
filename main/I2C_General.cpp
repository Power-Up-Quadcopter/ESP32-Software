
#include "I2C_General.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define WRITE_BIT I2C_MASTER_WRITE  /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ    /*!< I2C master read */
#define ACK_CHECK_EN 0x1            /*!< I2C master will check ack from slave*/
#define ACK_VAL 0x0                 /*!< I2C ack value */
#define NACK_VAL 0x1                /*!< I2C nack value */
#define CONFIG_I2C_MASTER_SCL 22
#define CONFIG_I2C_MASTER_SDA 21
#define CONFIG_I2C_MASTER_FREQUENCY 100000

static i2c_port_t i2c_port = I2C_NUM_0;

//common I2C functions that we will use
//Look here for documentation:
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html#

//Initialize the I2C port we will be using
esp_err_t I2C_Init(){
    int i2c_master_port = i2c_port;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = CONFIG_I2C_MASTER_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = CONFIG_I2C_MASTER_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = CONFIG_I2C_MASTER_FREQUENCY;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

////write a byte to a device
//void Write_Byte(uint8_t addr, uint8_t byte){
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd,addr,1);
//    i2c_master_write(cmd,&byte,1,1);
//    i2c_master_stop(cmd);
//
//    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
//    i2c_cmd_link_delete(cmd);
//}
//
////write n bytes to a device
//void Write_n_Bytes(uint8_t addr, uint8_t* bytes, int n){
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd,addr,1);
//    i2c_master_write(cmd,bytes,n,1);
//    i2c_master_stop(cmd);
//
//    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
//    i2c_cmd_link_delete(cmd);
//}
//
////read one byte from a device
//uint8_t Read_Byte(uint8_t addr){
//    uint8_t byte;
//
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd,addr,1);
//    i2c_master_read(cmd,&byte,1,0);
//    i2c_master_stop(cmd);
//
//    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
//    i2c_cmd_link_delete(cmd);
//
//    return byte;
//}
//
//
////read n bytes from a device
//void Read_n_Bytes(uint8_t addr, uint8_t* bytes, int n){
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, addr, 1);
//    i2c_master_read(cmd,bytes, n-1, 1);
//    i2c_master_read(cmd,bytes + n-1, 1, 0);
//    i2c_master_stop(cmd);
//
//    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2Cport,cmd, WAIT_TIME/portTICK_PERIOD_MS));
//    i2c_cmd_link_delete(cmd);
//
//}

void I2C_Write8(uint8_t addrW, uint8_t data, uint8_t regAddr){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, addrW << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, regAddr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    int ret = i2c_master_cmd_begin(i2c_port, cmd, 50 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if(ret != 0) printf("I2C error\n");

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


