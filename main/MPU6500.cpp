
#include <stdint.h>
#include <helper_3dmath.h>
#include "esp_log.h"
#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6500.h"
#include "I2C_General.h"
#include "Magnetometer.h"

#define mpuAddrW 208
#define NUM_AVG 500

#define YAW_DRIFT_OFFSET 0.00047524871674157234

namespace MPU{


#if GYRO_CALIBRATION
    short last20X[NUM_AVG];
    short last20Y[NUM_AVG];
    short last20Z[NUM_AVG];
    short last20A[NUM_AVG];
    int currentNumGyro = 0;
#endif

    float last20Yaw[20];
    float last20Pitch[20];
    float last20Roll[20];
    float last20AccelZ[20];
    int currentNum = 0;

    MPU6050 mpu;
    bool ready = false;
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    uint32_t accelXYZ[3];
    short raw[4];
    uint16_t packetSize = 42;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU





    [[noreturn]] void task_display(void* p){
        mpu.initialize();
        mpu.dmpInitialize();


        for(int i=0; i<20; i++){
            last20Yaw[i] = 0;
            last20Pitch[i] = 0;
            last20Roll[i] = 0;
            last20AccelZ[i] = 0;
        }

#if GYRO_CALIBRATION
        for(int i=0; i<NUM_AVG; i++){
            last20X[i] = 0;
            last20Y[i] = 0;
            last20Z[i] = 0;
            last20A[i] = 0;
        }
#endif

        mpu.setXGyroOffset(-12);
        mpu.setYGyroOffset(-35);
        mpu.setZGyroOffset(-17);
        mpu.setZAccelOffset(0);
        mpu.setDMPEnabled(true);

        while(1){
            mpuIntStatus = mpu.getIntStatus();
            // get current FIFO count
            fifoCount = mpu.getFIFOCount();

            if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                // reset so we can continue cleanly
                mpu.resetFIFO();
                // otherwise, check for DMP data ready interrupt frequently)
            } else if (mpuIntStatus & 0x02) {
                // wait for correct available data length, should be a VERY short wait
                while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

                // read a packet from FIFO
                mpu.getFIFOBytes(fifoBuffer, packetSize);
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                mpu.dmpGetGravity(&gravity, &q);
                mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
                mpu.dmpGetAccel(accelXYZ, fifoBuffer);

                float temp = ypr[1];
                ypr[1] = ypr[2];
                ypr[2] = temp;

                ready = true;

                last20Yaw[currentNum] = ypr[0];
                last20Pitch[currentNum] = ypr[1];
                last20Roll[currentNum] = ypr[2];
                last20AccelZ[currentNum] = accelXYZ[2];

                currentNum = (currentNum + 1)%20;
#if GYRO_CALIBRATION
                raw[0] = mpu.getRotationX();
                raw[1] = mpu.getRotationY();
                raw[2] = mpu.getRotationZ();
                raw[3] = mpu.getAccelerationZ();

                last20X[currentNumGyro] = raw[0];
                last20Y[currentNumGyro] = raw[1];
                last20Z[currentNumGyro] = raw[2];
                last20A[currentNumGyro] = raw[3];

                currentNumGyro = (currentNumGyro + 1)%NUM_AVG;
                if(currentNumGyro==0){
                    ESP_LOGI("MPU", "DONE!");
                }
#endif


//                mpu.dmpGetAccel()
//                data = Mag::getData();



//                printf("+++++\n");
//                printf("YAW: % 4.3f\n", (ypra[0] * 180/M_PI)+currentOff);
//                printf("PITCH: % 4.3f\n", ypra[1] * 180/M_PI);
//                printf("ROLL: % 4.3f\n", ypra[2] * 180/M_PI);

            }



            //Best result is to match with DMP refresh rate
            // Its last value in components/MPU6050/MPU6050_6Axis_MotionApps20.h file line 310
            // Now its 0x13, which means DMP is refreshed with 10Hz rate
            vTaskDelay(100/portTICK_PERIOD_MS);
        }

        vTaskDelete(NULL);
    }




    void write8(uint8_t data, uint8_t regAddr){
        I2C_Write8(mpuAddrW, data, regAddr);
    }


    void write16(uint16_t data, uint8_t regAddr){
        I2C_Write16(mpuAddrW, data, regAddr);
    }

    uint8_t read8(uint8_t regAddr){
        uint8_t out = I2C_Read8(mpuAddrW, regAddr);
        return out;
    }


    uint16_t read16(uint8_t regAddr){
        uint16_t out = I2C_Read16(mpuAddrW, regAddr);
        return out;
    }




#if GYRO_CALIBRATION
    void debuggy(const int8_t* haha){
        ESP_LOGI("MPU", "%d, %d, %d, %d", haha[0], haha[1], haha[2], haha[3]);
        mpu.setXGyroOffset(haha[0]);
        mpu.setXGyroOffset(haha[1]);
        mpu.setXGyroOffset(haha[2]);
        mpu.setXGyroOffset(haha[3]);
    }

    float getAvgX(){
        int sum = 0;
        for(int i : last20X){
            sum += i;
        }
        return sum / ((double) NUM_AVG);
    }

    float getAvgY(){
        int sum = 0;
        for(int i : last20Y){
            sum += i;
        }
        return sum / ((double) NUM_AVG);
    }

    float getAvgZ(){
        int sum = 0;
        for(int i : last20Z){
            sum += i;
        }
        return sum / ((double) NUM_AVG);
    }

    float getAvgA(){
        int sum = 0;
        for(int i : last20A){
            sum += i;
        }
        return sum / ((double) NUM_AVG);
    }
#endif
}

