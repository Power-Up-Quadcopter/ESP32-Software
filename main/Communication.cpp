
#include "Control.h"
#include "Communication.h"

namespace Talk {
    int parse(const uint8_t *in, uint8_t *out, int len) {
        if (len == 0) { return 0; }
        switch (in[0]) {
            case 0x01: //Joystick Axes

                break;
            case 0x02: //Joystick Buttons

                break;
            case 0x03: //Joystick Connected

                break;
            case 0x04: //Joystick Disconnected

                break;
            case 0xD0: //Debug variables

                break;
            case 0xF1: //TCP Pong

                break;
            case 0xF3: //UDP Pong

                break;
        }
    }

}
