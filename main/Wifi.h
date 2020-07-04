

#ifndef ESP32_SOFTWARE_WIFI_H
#define ESP32_SOFTWARE_WIFI_H

void Wifi_Init();

void Wifi_startTCPServer();
void Wifi_sendTCP(char* message, int len);

#endif //ESP32_SOFTWARE_WIFI_H
