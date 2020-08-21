
#include <stdint.h>
#include <cmath>
#include "I2C_General.h"
#include "Magnetometer.h"

#define magAddrW 0x1E
#define MAG_LOOP_STACK_SIZE 2048

#define offsetX 494.5
#define offsetY 937.5
#define offsetZ 508.5

#define scaleX 1.0845315904139432
#define scaleY 0.9374764595103577
#define scaleZ 0.988875645609853

//#define offsetX 49.19
//#define offsetY 98.04
//#define offsetZ 55.14
//
//#define C1 0.982
//#define C2 -0.062
//#define C3 -0.004
//#define C4 -0.062
//#define C5 1.009
//#define C6 0.009
//#define C7 -0.004
//#define C8 0.009
//#define C9 1.014


//#define scaleX 1
//#define scaleY 1
//#define scaleZ 1

//#define offsetX 0
//#define offsetY 0
//#define offsetZ 0
//
//#define scaleX 1
//#define scaleY 1
//#define scaleZ 1

namespace Mag {
    uint8_t magReady;

    [[noreturn]] void loop(void* arg);

    void write8(uint8_t data, uint8_t regAddr) {
        I2C_Write8(magAddrW, data, regAddr);
    }

    void write16(uint16_t data, uint8_t regAddr) {
        I2C_Write16(magAddrW, data, regAddr);
    }

    uint8_t read8(uint8_t regAddr) {
        uint8_t out = I2C_Read8(magAddrW, regAddr);
        return out;
    }

    uint16_t read16(uint8_t regAddr) {
        uint16_t out = I2C_Read16(magAddrW, regAddr);
        return out;
    }

    MagData* getData(){
        auto* data = new MagData;
        float tempX = read16(MAG_OUT_X) - offsetX;
        float tempY = read16(MAG_OUT_Y) - offsetY;
        float tempZ = read16(MAG_OUT_Z) - offsetZ;
        data->x = tempX * scaleX;
        data->y = tempY * scaleY;
        data->z = tempZ * scaleZ;
        data->temp = read8(MAG_TEMP);
        return data;
    }


    int getHeading(float yaw, float pitch, float roll, MagData* data){
        return data->x;
    }

    void init() {
        if (uint8_t i = read8(MAG_WHOAMI) != MAG_WHOAMI_VAL) {
            printf("Error for Mag device ID: Expected %X, Got %X\n", MAG_WHOAMI_VAL, i);
            return;
        }

        //standby mode
        write8(0b00000000, MAG_CTRL_REG1);

        //auto-cal disabled
        //no one-shot mag reset
        //no one-shot mag measurement
        //8x oversampling
        //hybrid mode with accel and mag active
        write8(0b00011111, MAG_M_CTRL_REG1);

        //hyb_autoinc_mode=1 mag registers follow accel registers
        //m_maxmin dis=0 default max/min latching
        //m_maxmin_dis_ths=0
        //m_maxmin_rst=0
        //m_rst_cnt=0 enable magnetic reset each cycle
        write8(0b00100000, MAG_M_CTRL_REG2);

        //hpf_out=0
        //fs=01 accel range of +/- 4g
        write8(0x01, MAG_XYZ_DATA_CFG);

        //aslp_rate=00
        //dr=001 200hz data rate
        //lnoise=1 low noise mode
        //f_read=0 16 bit reads
        //active=1 take part out of standby
        write8(0b00101101, MAG_CTRL_REG1);

        magReady = 1;
        xTaskCreate(&loop, "DroneLoop", MAG_LOOP_STACK_SIZE, NULL, 4, NULL);
    }

    [[noreturn]] void loop(void* arg) {
        while (true) {
//        while((Mag_Read8(MAG_M_DR_STATUS) & (0b111) ) == 0){vTaskDelay(100 / portTICK_PERIOD_MS);} //delay until ready
//        magReady = 1;
//            MagData* data = getData();
//
//            printf("-----\n");
//            printf("X: %d\n", data->x);
//            printf("Y: %d\n", data->y);
//            printf("Z: %d\n", data->z);
//            printf("Heading: %f\n", atan2(data->y, data->x));
//            printf("Temp: %d\n", data->temp);


            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

}