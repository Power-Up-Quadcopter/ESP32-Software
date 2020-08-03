
#ifndef ESP32_SOFTWARE_PRESSURE_H
#define ESP32_SOFTWARE_PRESSURE_H

#define SPL06_ADDRESS 0x77

#define PSR_B2      0x00
#define PSR_B1      0x01
#define PSR_B0      0x02
#define TMP_B2      0x03
#define TMP_B1      0x04
#define TMP_B0      0x05

#define PRS_CFG     0x06
#define TMP_CFG     0x07
#define MEAS_CFG    0x08
#define CFG_REG     0x09
#define INT_STS     0x0A
#define FIFO_STS    0x0B
#define SPL06RESET  0x0C
#define SPL06ID     0x0D

#define COEF_A      0x10
#define COEF_B      0x11
#define COEF_C      0x12
#define COEF_D      0x13
#define COEF_E      0x14
#define COEF_F      0x15
#define COEF_G      0x16
#define COEF_H      0x17
#define COEF_I      0x18
#define COEF_J      0x19
#define COEF_K      0x1A
#define COEF_L      0x1B
#define COEF_M      0x1C
#define COEF_N      0x1D
#define COEF_O      0x1E
#define COEF_P      0x1F
#define COEF_Q      0x20
#define COEF_R      0x21

class SPL06 {

protected:
    int c0{}, c1{}, c00{}, c10{}, c01{}, c11{}, c20{}, c21{}, c30{};
    const double kt = 3670016;
    const double kp = 1040384;

    double get_trawsc();

public:
    //  configures SPL06
    void initialize();

    //  gets temperature in celsius
    double getTemperature();

    //  gets pressure in pascals
    double getPressure();

    //  gets altitude in meters
    double getAltitude();
};


#endif //ESP32_SOFTWARE_PRESSURE_H
