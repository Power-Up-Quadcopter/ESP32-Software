
#ifndef ESP32_SOFTWARE_ESC_CONTROL_H
#define ESP32_SOFTWARE_ESC_CONTROL_H

#include <stdint.h>

#define ESC0_PIN 32
#define ESC1_PIN 33
#define ESC2_PIN 25
#define ESC3_PIN 26

namespace Esc {
    void init();
    void set(uint8_t esc, uint8_t power);
}
#endif //ESP32_SOFTWARE_ESC_CONTROL_H
