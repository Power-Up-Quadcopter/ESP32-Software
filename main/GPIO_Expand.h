
#ifndef ESP32_SOFTWARE_GPIO_EXPAND_H
#define ESP32_SOFTWARE_GPIO_EXPAND_H

//put useful defines for register addresses here
#define IODIRA 0x00
#define IODIRB 0x01
#define GPPUA 0x0C
#define GPPUB 0x0D
#define GPIOA 0x12
#define GPIOB 0x13


class GPIO_Expand {

public:
    uint8_t A0 = 0, A1 = 1, A2 = 2, A3 = 3, A4 = 4, A5 = 5, A6 = 6, A7 = 7;
    uint8_t B0 = 8, B1 = 9, B2 = 10, B3 = 11, B4 = 12, B5 = 13, B6 = 14, B7 = 15;
    uint8_t A = 16, B = 17;
    uint8_t ADIR = 0, BDIR = 0;

    // Initializes registers to their initial state
    void initialize();

    // Sets the I/O direction of a specific pin to the given bit value
    void set_dir_bit(uint8_t pin, uint8_t bit);

    // Sets the entire Direction register of the specified port (A or B)
    void set_dir_reg(uint8_t byte, uint8_t port);

    // Gets a specific direction bit
    uint8_t get_dir_bit(uint8_t pin);

    // Gets the entire bit pattern of the Direction register
    uint8_t get_dir_reg(uint8_t port);

    // Sets the Pull Up Resistor bit of a specific pin to the given bit value
    void set_pur_bit(uint8_t pin, uint8_t bit);

    // Sets the entire bit pattern of the Pull Up Resistor Register
    void set_pur_reg(uint8_t byte, uint8_t port);

    // Reads a specific GPIO bit
    uint8_t read_bit(uint8_t pin);

    // Reads the entire bit pattern of the GPIO register data
    uint8_t read_port(uint8_t port);

    // Sets the specified output bit of the GPIO register data
    void set_bit(uint8_t pin, uint8_t bit);

};



#endif //ESP32_SOFTWARE_GPIO_EXPAND_H
