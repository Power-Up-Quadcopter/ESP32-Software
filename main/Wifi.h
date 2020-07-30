

#ifndef ESP32_SOFTWARE_WIFI_H
#define ESP32_SOFTWARE_WIFI_H

namespace Wifi{

    void init();

    void startTCPServer();
    void sendTCP(char* message, int len);

}


#endif //ESP32_SOFTWARE_WIFI_H
