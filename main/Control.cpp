
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include <freertos/semphr.h>
#include "Control.h"



namespace Control{

    SemaphoreHandle_t xMutex;

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
        xSemaphoreTake( xMutex, portMAX_DELAY );
        LX = in[0];
        LY = in[1];
        RX = in[2];
        RY = in[3];
        xSemaphoreGive( xMutex );
    }

    void updateButtons(const uint8_t* in){
        bool buttons[16];
        for(int i=0; i<8; i++){
            buttons[i] = (in[0]>>i)&~1;
            buttons[i+8] = (in[1]>>i)&~1;
        }
        xSemaphoreTake( xMutex, portMAX_DELAY );
        tL = buttons[0];   tR = buttons[1];  bL = buttons[2];     bR = buttons[3];
        st = buttons[4];   sel = buttons[5]; JoyL = buttons[6];   JoyR = buttons[7];
        a = buttons[8];    b = buttons[9];   x = buttons[10];     y = buttons[11];
        dUp = buttons[12]; dR = buttons[13]; dDown = buttons[14]; dL = buttons[15];
        xSemaphoreGive( xMutex );
    }

    void updateStatus(bool connected){
        xSemaphoreTake( xMutex, portMAX_DELAY );
        joyConnect = connected;
        xSemaphoreGive( xMutex );
    }

    void doThings(){
        xSemaphoreTake( xMutex, portMAX_DELAY );
        //stuff
        xSemaphoreGive( xMutex );
    }

    void init(){
        xMutex = xSemaphoreCreateMutex();
    }

}


