
#include <stdint.h>
#include "I2C_General.h"
#include "Utils.h"
#include "math.h"
#include "SPL06.h"

void SPL06::initialize() {
    I2C_Write8(SPL06_ADDRESS, 0b00000100, CFG_REG);  //  set result bit-shift
    I2C_Write8(SPL06_ADDRESS, 0b00000111, MEAS_CFG); //  sensor config, continuous background
    I2C_Write8(SPL06_ADDRESS, 0b10100010, TMP_CFG);  //  temp, 4 mps, 4x oversampling
    I2C_Write8(SPL06_ADDRESS, 0b00100110, PRS_CFG); //  pressure, 4 mps, 64x oversampling

    uint8_t c_a = I2C_Read8(SPL06_ADDRESS, COEF_A);
    uint8_t c_b = I2C_Read8(SPL06_ADDRESS, COEF_B);
    uint8_t c_c = I2C_Read8(SPL06_ADDRESS, COEF_C);
    uint8_t c_d = I2C_Read8(SPL06_ADDRESS, COEF_D);
    uint8_t c_e = I2C_Read8(SPL06_ADDRESS, COEF_E);
    uint8_t c_f = I2C_Read8(SPL06_ADDRESS, COEF_F);
    uint8_t c_g = I2C_Read8(SPL06_ADDRESS, COEF_G);
    uint8_t c_h = I2C_Read8(SPL06_ADDRESS, COEF_H);
    uint8_t c_i = I2C_Read8(SPL06_ADDRESS, COEF_I);
    uint8_t c_j = I2C_Read8(SPL06_ADDRESS, COEF_J);
    uint8_t c_k = I2C_Read8(SPL06_ADDRESS, COEF_K);
    uint8_t c_l = I2C_Read8(SPL06_ADDRESS, COEF_L);
    uint8_t c_m = I2C_Read8(SPL06_ADDRESS, COEF_M);
    uint8_t c_n = I2C_Read8(SPL06_ADDRESS, COEF_N);
    uint8_t c_o = I2C_Read8(SPL06_ADDRESS, COEF_O);
    uint8_t c_p = I2C_Read8(SPL06_ADDRESS, COEF_P);
    uint8_t c_q = I2C_Read8(SPL06_ADDRESS, COEF_Q);
    uint8_t c_r = I2C_Read8(SPL06_ADDRESS, COEF_R);

    c0 = (c_a << 4) | (c_b >> 4);
    c1 = ((c_b & 0x0F) << 8) | c_c;
    c00 = (c_d << 12) | (c_e << 4) | (c_f >> 4);
    c10 = ( (c_f & 0x0F) << 16 ) | (c_g << 8) | c_h;
    c01 = (c_i << 8) | c_j;
    c11 = (c_k << 8) | c_l;
    c20 = (c_m << 8) | c_n;
    c21 = (c_o << 8) | c_p;
    c30 = (c_q << 8) | c_r;

    c0 = unsignedToSigned(c0, 12);
    c1 = unsignedToSigned(c1, 12);
    c00 = unsignedToSigned(c00, 20);
    c10 = unsignedToSigned(c10, 20);
    c01 = unsignedToSigned(c01, 16);
    c11 = unsignedToSigned(c11, 16);
    c20 = unsignedToSigned(c20, 16);
    c21 = unsignedToSigned(c21, 16);
    c30 = unsignedToSigned(c30, 16);
}

double SPL06::get_trawsc() {
    uint8_t high_temp = I2C_Read8(SPL06_ADDRESS, TMP_B2);
    uint8_t med_temp = I2C_Read8(SPL06_ADDRESS, TMP_B1);
    uint8_t low_temp = I2C_Read8(SPL06_ADDRESS, TMP_B0);
    int tempraw = (high_temp << 16u) | (med_temp << 8u) | low_temp;
    tempraw = unsignedToSigned(tempraw, 24);

    double trawsc = tempraw / kt;
    return trawsc;
}

double SPL06::getPressure() {
    uint8_t high_pressure = I2C_Read8(SPL06_ADDRESS, PSR_B2);
    uint8_t med_pressure = I2C_Read8(SPL06_ADDRESS, PSR_B1);
    uint8_t low_pressure = I2C_Read8(SPL06_ADDRESS, PSR_B0);
    int pressureraw = (high_pressure << 16u) | (med_pressure << 8u) | low_pressure;
    pressureraw = unsignedToSigned(pressureraw, 24);

    double prawsc = pressureraw / kp;
    double trawsc = this->get_trawsc();

    double pcomp = c00 + prawsc * (c10 + prawsc * (c20 + prawsc * c30))
                   + trawsc * c01 + trawsc * prawsc * (c11 + prawsc * c21);
    return pcomp;
}

double SPL06::getAltitude() {
    double pressure = this->getPressure();
    double altitude = 44330 * ( 1 - pow(pressure / 101325., 1/5.255) );
    return altitude;
}

double SPL06::getTemperature() {
    double trawsc = this->get_trawsc();
    double tcomp = c0 * 0.5 + c1 * trawsc;

    return tcomp;
}
