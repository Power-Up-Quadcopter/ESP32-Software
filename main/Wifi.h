

#ifndef ESP32_SOFTWARE_WIFI_H
#define ESP32_SOFTWARE_WIFI_H

namespace Wifi{

    void init();
    void sendTCP(char* message, int len);
    void sendUDP(char* message, int len);
}


#endif //ESP32_SOFTWARE_WIFI_H
