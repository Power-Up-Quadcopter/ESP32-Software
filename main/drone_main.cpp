
#include <stdio.h>
#include <string>
#include <nvs_flash.h>
#include <tcpip_adapter.h>
#include <sstream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "MPU6500.h"
#include "ESC_Control.h"
#include "I2C_General.h"
#include "GPS.h"
#include "Wifi.h"

#define MAIN_LOOP_STACK_SIZE 2048

using namespace std;

extern "C" {
    void app_main();
};


//Note: If not using OpenOCD, run using "mingw32-make.exe flash" in the terminal below

void DroneLoop(void*);

bool GPSready = false;

uint8_t data[1024];
char strbuffer[1024];
void task_gps(void *arg) {
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity    = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, 32, 33, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, 2048, 2048, 10, &uart_queue, 0));

    // Read data from UART.
    const int uart_num = UART_NUM_2;
    int length = 0;

    GPSready = true;
    while(1) {
        int x = uart_get_buffered_data_len(uart_num, (size_t *) &length);
        ESP_ERROR_CHECK(x);
        length = uart_read_bytes(uart_num, data, length, 1000);
        printf("%d\n", length);
        int strIndex = 0;
        for(int i = 0; i < length; i++) {
            if(data[i] == 13) continue;
            strbuffer[strIndex++] = data[i];
            printf("%c", (int8_t) data[i]);
        }
        strbuffer[strIndex++] = '\n';
        strbuffer[strIndex++] = '\n';
        Wifi_sendTCP(strbuffer, strIndex-1);
        printf("\n-----------------------------------\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void sendGPS(string msg) {
    uint8_t checksum = msg.data()[0];
    for(int i = 1; i < msg.length(); i++) {
        checksum ^= msg.data()[i];
    }
    stringstream sstream;
    sstream << hex << checksum;

    string toSend = "$";
    toSend.append(msg);
    toSend.append("*");
    toSend.append(sstream.str());
    printf("GPS SEND: %s\n", toSend.data());
    toSend.append("\r\n");

    uart_write_bytes(UART_NUM_2, toSend.data(), toSend.length());
}

//arduino setup function equivalent
void app_main() {

    ESP_LOGI("app_main", "Setting up");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);


    ESP_ERROR_CHECK(I2C_Init());
//    Esc_Init();
//    GPS_Init();
    Wifi_Init();

    //TEST CODE HERE
//    printf("TEST: Output 50 Percent in ESC0\n");
//    Esc_Set(0, 50);
    //END TEST AREA

//    Wifi_startTCPServer();

    //create the DroneLoop task
    xTaskCreate(&DroneLoop, "DroneLoop", MAIN_LOOP_STACK_SIZE, NULL, 4, NULL);
    xTaskCreate(&task_gps, "GPS", MAIN_LOOP_STACK_SIZE, NULL, 5, NULL);
}

//arduino loop function equivalent
void DroneLoop(void* ptr){

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    sendGPS("PSTMNMEAONOFF,0");
    sendGPS("PSTMCFGANTSENS,2,1,0,0,0,1,8,150,500");

    while (1){
        //TODO put crap here

        //TEST CODE HERE
        //printf("Looping\n");
        //END TEST AREA

//        string msg = "PSTMCFGANTSENS,2,1,0,0,0,1,8,150,500";



//  TCP Test message
//        char message[] = "lololol\n";
//        Wifi_sendTCP(message);

//  I2C GPIO Expander LED Blink
//        int chip_addr = 0x20;
//        I2C_Write8(chip_addr, 0x00, 0b000);  //  set port A to outputs
//        I2C_Write8(chip_addr, 0x01, 0b000);  //  set port B to outputs
//        while(1) {
//            printf("LEDR ON\n");
//            I2C_Write8(chip_addr,0b101, 0x12);
//            vTaskDelay(1000 / portTICK_RATE_MS);
//            printf("LEDR OFF\n");
//            I2C_Write8(chip_addr, 0b000, 0x12);
//            vTaskDelay(1000 / portTICK_RATE_MS);
//        }

        vTaskDelay(5000 / portTICK_PERIOD_MS); //delay 1000ms

    }
}
