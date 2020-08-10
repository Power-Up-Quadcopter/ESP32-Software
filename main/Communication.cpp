
#include <string>
#include <soc_log.h>
#include "Control.h"
#include "GPS.h"
#include "Wifi.h"
#include "Communication.h"

namespace Talk {
    bool receive;

    int parse(const uint8_t *in, uint8_t *out, int len) {
        if (len == 0) { return 0; }
        char temp[len];
        for(int i=0; i<len-1; i++){
            temp[i] = in[i+1];
        }
        temp[len-1] = 0;
        switch (in[0]) {
            case 0x01: //Joystick Axes
                Control::updateAxes(in+1);
                return 0;
            case 0x02: //Joystick Buttons
                Control::updateButtons(in+1);
                return 0;
            case 0x03: //Joystick Connected
                Control::updateStatus(1);
                break;
            case 0x04: //Joystick Disconnected
                Control::updateStatus(0);
                break;
            case 0xB0: //Motor 0Fr,1FL,2BL,3BR, speed 1 byte

                break;
            case 0xD0: //Debug variables
                GPS::sendGPS(std::string(temp));
                break;
            case 0xF1: //TCP Pong
                receive = 1;
//                ESP_LOGI("Parse", "recieve");
                break;
            case 0xF3: //UDP Pong
                receive = 1;
                break;
        }
        return 0;
    }

    //send an error msg over TCP
    void sendError(std::string msg){
        char out[msg.length()+1];
        out[0] = 0xA1; //header byte
        const char* temp = msg.c_str();

        for(int i=1; temp[i]!=0; i++){
            out[i] = temp[i];
        }

        Wifi::sendTCP(out,msg.length()+1);
    }

    //send a non-critical msg over UDP
    void sendNorm(std::string msg){
        char out[msg.length()+1];
        out[0] = 0xA1; //header byte
        const char* temp = msg.c_str();

        for(int i=1; temp[i]!=0; i++){
            out[i] = temp[i];
        }

        Wifi::sendUDP(out,msg.length()+1);
    }

}
