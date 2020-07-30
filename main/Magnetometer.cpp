
#include <stdint.h>
#include "I2C_General.h"
#include "Magnetometer.h"

#define magAddrW 0x1E
#define MAG_LOOP_STACK_SIZE 2048

namespace Mag {
    MAGDATA_t magData;
    uint8_t magReady;

    void Mag_Loop(void *ptr);

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

        magData = MAGDATA_t{0, 0, 0, 0};
        magReady = 1;
        xTaskCreate(&Mag_Loop, "DroneLoop", MAG_LOOP_STACK_SIZE, NULL, 4, NULL);
    }

    void loop(void *ptr) {
        while (true) {
//        while((Mag_Read8(MAG_M_DR_STATUS) & (0b111) ) == 0){vTaskDelay(100 / portTICK_PERIOD_MS);} //delay until ready
//        magReady = 1;
            magData.x = read16(MAG_OUT_X);
            magData.y = read16(MAG_OUT_Y);
            magData.z = read16(MAG_OUT_Z);
            magData.temp = read8(MAG_TEMP);

            printf("X: %d\n", magData.x);
            printf("Y: %d\n", magData.y);
            printf("Z: %d\n", magData.z);
            printf("Temp: %d\n", magData.temp);
            printf("-----\n");

            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

}