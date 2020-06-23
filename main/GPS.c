
#include <driver/uart.h>
#include "GPS.h"

#define GPSRX 16
#define GPSTX 17

//see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html

void GPS_Init(){

    const uart_port_t uart_num = UART_NUM_2;
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // Set rx and tx pins
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, GPSTX, GPSRX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, \
                                        uart_buffer_size, 10, &uart_queue, 0));
}

