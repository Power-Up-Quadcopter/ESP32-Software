
#ifndef ESP32_SOFTWARE_MPU6500_H
#define ESP32_SOFTWARE_MPU6500_H

#define GYRO_CALIBRATION false

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02
#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG_2 0x1D

#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define TEMP_OUT_H 0x41
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47

#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B

#define WHO_AM_I 0x75

namespace MPU{

    extern float ypr[];
    extern uint32_t accelXYZ[];
    extern short raw[4];

    extern float last20Yaw[20];
    extern float last20Pitch[20];
    extern float last20Roll[20];
    extern float last20AccelZ[20];
    /** Task that gets called by app_main to initialize component.
     *
     * @param p Unused, set to NULL
     */
    [[noreturn]] void task_display(void* p);

    /**Sends new gyro offsets to the MPU6500 for calibration purposes.
     *
     * @param haha An array of 4 bytes representing the new offsets to be sent
     */
    void debuggy(const int8_t* haha);

    /** Component specific I2C function for reading 8 bit registers.
     *
     * @param data The 8 bit data value to write
     * @param regAddr The address of the 8 bit register you want to access
    */
    void write8(uint8_t data, uint8_t regAddr);

    /** Component specific I2C function for writing to 16 bit registers.
     *
     * @param data The 16 bit data value to write
     * @param regAddr The address of the 16 bit register you want to access
     *
    */
    void write16(uint16_t data, uint8_t regAddr);

    /** Component specific I2C function for reading 8 bit registers.
     *
     * @param regAddr The address of the 8 bit register you want to access
     *
     * @return The 8 bit value contained within the register.
    */
    uint8_t read8(uint8_t regAddr);

    /** Component specific I2C function for reading 16 bit registers.
     *
     * @param regAddr The address of the 16 bit register you want to access
     *
     * @return The 16 bit value contained within the register.
    */
    uint16_t read16(uint8_t regAddr);



//    ///Functions to get the integral and derivative of recent MPU values
//    float getIntegralYaw();
//    float getIntegralPitch();
//    float getIntegralRoll();
//    float getIntegralAccel();
//    float getDerivativeYaw();
//    float getDerivativePitch();
//    float getDerivativeRoll();
//    float getDerivativeAccel();


#if GYRO_CALIBRATION
    /** Calculates the average of the last NUM_AVG X gyro values.
     *
     * @return The average calculated
     */
    float getAvgX();

    /** Calculates the average of the last NUM_AVG Y gyro values.
     *
     * @return The average calculated
     */
    float getAvgY();

    /** Calculates the average of the last NUM_AVG Z gyro values.
     *
     * @return The average calculated
     */
    float getAvgZ();

    /** Calculates the average of the last NUM_AVG Z acceleration values.
     *
     * @return The average calculated
     */
    float getAvgA();
#endif
}

#endif //ESP32_SOFTWARE_MPU6500_H
