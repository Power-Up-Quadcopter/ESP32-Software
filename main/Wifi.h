

#ifndef ESP32_SOFTWARE_WIFI_H
#define ESP32_SOFTWARE_WIFI_H

namespace Wifi{

    extern bool TCPConnected; /*!< < True if a device is communicating over TCP. */

    /** Wifi initialization function. Call to use network capabilities.
     *
     */
    void init();

    /** Send a TCP message to a connected device
     *
     * @param message Packet data to be sent.
     * @param len Length of packet data
     * @param isTask True unless the caller is the TCP server task.
     */
    void sendTCP(char* message, int len, bool isTask=false);

    /** Send a UDP message to a connected device
     *
     * @param message Packet data to be sent.
     * @param len Length of packet data
     * @param isTask True unless the caller is the UDP server task.
     */
    void sendUDP(char* message, int len, bool isTask=false);
}


#endif //ESP32_SOFTWARE_WIFI_H
