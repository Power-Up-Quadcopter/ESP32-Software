
#include <stdint.h>
#include <helper_3dmath.h>
#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6500.h"
#include "I2C_General.h"

#define mpuAddrW 208


namespace MPU{

    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    uint16_t packetSize = 42;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU

    [[noreturn]] void task_display(void* p){
        MPU6050 mpu = MPU6050();
        mpu.initialize();
        mpu.dmpInitialize();

        // This need to be setup individually
        mpu.setXGyroOffset(220);
        mpu.setYGyroOffset(76);
        mpu.setZGyroOffset(-85);
        mpu.setZAccelOffset(1788);

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
                printf("YAW: %3.1f, ", ypr[0] * 180/M_PI);
                printf("PITCH: %3.1f, ", ypr[1] * 180/M_PI);
                printf("ROLL: %3.1f \n", ypr[2] * 180/M_PI);
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

}

