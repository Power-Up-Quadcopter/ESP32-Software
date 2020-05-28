
#ifndef ESP32_SOFTWARE_ESC_CONTROL_H
#define ESP32_SOFTWARE_ESC_CONTROL_H

#include <stdint.h>

void ESCSetup();
void ESC_Set(uint8_t esc, uint8_t power);

#endif //ESP32_SOFTWARE_ESC_CONTROL_H
