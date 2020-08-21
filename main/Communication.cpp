
#include <string>
#include <soc_log.h>
#include "Control.h"
#include "GPS.h"
#include "Wifi.h"
#include "ESC_Control.h"
#include "MPU6500.h"
#include "Communication.h"


namespace Talk {
    bool receive;

    uint8_t allOff[] = {0,0,0,0};
    uint8_t allOn[] = {255,255,255,255};

    int parse(const uint8_t *in, uint8_t *out, int len) {
        if (len == 0) { return 0; }
        char temp[len];
        for(int i=0; i<len-1; i++){
            temp[i] = in[i+1];
        }
        temp[len-1] = 0;
        switch (in[0]) {
            case 0x01: //Joystick Axes
                Control::updateAxes((int8_t*) (in+1));
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
                Esc::set((ESC_PIN_t) in[1], in[2]);
                break;
            case 0xD0: //Debug functions
                switch(in[1]){
                    case 0x0:   //GPS message send
                        GPS::sendGPS(std::string(temp));
                        break;
                    case 0x1:   //MPU gyro offsets
#if GYRO_CALIBRATION
                        MPU::debuggy((int8_t*) in+2);
#endif
                        break;
                    case 0x2:   //turn off all motors
                        Esc::setAll(allOff);
                        break;
                    case 0x3:   //turn on all motors
                        Esc::setAll(allOn);
                        break;
                    case 0x4:
                        printf("%f\n", *((float*) (in + 2)) );
                        Control::updatePID(in[2], (float*) (in+3));
                        break;
                }
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
