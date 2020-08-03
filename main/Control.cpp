
#include <cstdint>
#include "Control.h"

namespace Control{

    //Joystick Axes
    uint8_t LX = 128;
    uint8_t LY = 128;
    uint8_t RX = 128;
    uint8_t RY = 128;
    //Joystick Buttons
    bool tL, tR, bL, bR, st, sel, JoyL, JoyR, a, b, x, y, dUp, dR, dDown, dL = false;
    //Joystick Status
    bool joyConnect = false;


    void updateAxes(const uint8_t* in){

    }

}


