
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/semphr.h>
#include <esp_log.h>
#include "Control.h"
#include "MPU6500.h"
#include "ESC_Control.h"

#define CONTROL_LOOP_STACK_SIZE 8192



namespace Control{


    //PID parameters Yaw,Pitch,Roll,ZAccel
    float pPara[6] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
    float iPara[6] = {0.001f,0.001f,0.001f,0.001f,0.001f,0.001f};
    float dPara[6] = {0.001f,0.001f,0.001f,0.001f,0.001f,0.001f};

    //Set points for pid
    float SPYaw;
    float SPPitch;
    float SPRoll;
    float SPAccelZ;


    SemaphoreHandle_t xMutex;

    //Joystick Axes, make sure these are pre-adjusted on the control software
    //failing to do so will result in very jerky flight
    //x/15.0 + (x/41.3)^3 is a decent starting point for adjustment, change the constants around in desmos to see what works
    int8_t LX = 0; //strafe left/right
    int8_t LY = 0; //up/down
    int8_t RX = 0; //yaw left,right
    int8_t RY = 0; //forward/back
    //Joystick Buttons
    bool tL, tR, bL, bR, st, sel, JoyL, JoyR, a, b, x, y, dUp, dR, dDown, dL = false;
    //Joystick Status
    bool joyConnect = false;


    void updatePID(uint8_t type, const float* args){
        xSemaphoreTake( xMutex, portMAX_DELAY );
        printf("%d, %f, %f, %f\n", type, args[0], args[1], args[2]);
        pPara[type] = args[0];
        iPara[type] = args[1];
        dPara[type] = args[2];
        xSemaphoreGive( xMutex );
    }

    void updateAxes(const int8_t* in){
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

    float getI(const float* in){
        float sum = 0.0f;
        for(int i=0; i<20; i++){
            sum += in[i];
        }
        return sum;
    }

    float getD(const float* in){
        float sum = 0.0f;
        for(int i=1; i<6; i++){
            sum += in[i] - in[i-1];
        }
        return sum/5.0f;
    }

    void doThings(void* arg){

        vTaskDelay(20000 / portTICK_PERIOD_MS); //wait 20sec for MPU DMP engine to stabilize


        float toYaw    = 0;
        float toPitch  = 0;
        float toRoll   = 0;
        float throttle = 0;

        SPYaw = MPU::ypr[0];
        SPPitch = 0;
        SPRoll = 0;

        float PVYaw;
        float PVPitch;
        float PVRoll;
        float PVAccelZ;

        float yawErr;
        float pitchErr;
        float rollErr;
        float zAccelErr;

        float last20ErrYaw[20];
        float last20ErrPitch[20];
        float last20ErrRoll[20];
        float last20ErrZAccel[20];
        for(int i=0; i<20; i++){
            last20ErrYaw[i]    = 0.0f;
            last20ErrPitch[i]  = 0.0f;
            last20ErrRoll[i]   = 0.0f;
            last20ErrZAccel[i] = 0.0f;
        }
        int currentNum = 0;

        ESP_LOGI("Control", "Done!");
        while(true){
            xSemaphoreTake( xMutex, portMAX_DELAY );
            int8_t tempLX = LX;
            int8_t tempLY = LY;
            int8_t tempRX = RX;
            int8_t tempRY = RY;
            xSemaphoreGive( xMutex );

            PVYaw    = MPU::ypr[0];
            PVPitch  = MPU::ypr[1];
            PVRoll   = MPU::ypr[2];
            PVAccelZ = MPU::accelXYZ[2];

            yawErr    = SPYaw - PVYaw;
            pitchErr  = SPPitch - PVPitch;
            rollErr   = SPRoll - PVRoll;
            zAccelErr = SPAccelZ - PVAccelZ;

            last20ErrYaw[currentNum]    = yawErr;
            last20ErrPitch[currentNum]  = pitchErr;
            last20ErrRoll[currentNum]   = rollErr;
            last20ErrZAccel[currentNum] = zAccelErr;

            toYaw      = pPara[0] * yawErr    + iPara[0] * getI(last20ErrYaw)    + dPara[0] * getD(last20ErrYaw);
            toPitch    = pPara[1] * pitchErr  + iPara[1] * getI(last20ErrPitch)  + dPara[1] * getD(last20ErrPitch);
            toRoll     = pPara[2] * rollErr   + iPara[2] * getI(last20ErrRoll)   + dPara[2] * getD(last20ErrRoll);
            throttle   = pPara[3] * zAccelErr + iPara[3] * getI(last20ErrZAccel) + dPara[3] * getD(last20ErrZAccel);

            Esc::motorPID(1, toPitch, toRoll, 1);

            currentNum = (currentNum + 1) % 20;

            vTaskDelay( 50 / portTICK_PERIOD_MS);
        }
        vTaskDelete(NULL);
    }

    void init(){
        xMutex = xSemaphoreCreateMutex();
        xTaskCreate(&doThings, "DroneLoop", CONTROL_LOOP_STACK_SIZE, NULL, 5, NULL);
    }

}


