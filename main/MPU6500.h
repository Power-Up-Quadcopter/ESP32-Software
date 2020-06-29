
#ifndef ESP32_SOFTWARE_MPU6500_H
#define ESP32_SOFTWARE_MPU6500_H

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

//write an 8bit value to specific register
void MPU_Write8(uint8_t data, uint8_t regAddr);

//write a 16 bit value to two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
void MPU_Write16(uint16_t data, uint8_t regAddr);

//read an 8bit value from specific register
uint8_t MPU_Read8(uint8_t regAddr);

//read a 16 bit value from two sequential registers
//takes the high bit register b/c mpu auto increments register after r/w
uint16_t MPU_Read16(uint8_t regAddr);

#endif //ESP32_SOFTWARE_MPU6500_H
