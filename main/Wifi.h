

#ifndef ESP32_SOFTWARE_WIFI_H
#define ESP32_SOFTWARE_WIFI_H

namespace Wifi{

    extern bool TCPConnected;

    void init();
    void sendTCP(char* message, int len, bool isTask=false);
    void sendUDP(char* message, int len, bool isTask=false);
}


#endif //ESP32_SOFTWARE_WIFI_H
