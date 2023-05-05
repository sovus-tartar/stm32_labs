#pragma once

#include <stdint.h>

#define MPU6050_ADDR (uint8_t)0b1101000U
#define WHO_AM_I_REG (uint8_t)0x75U
#define PWR_MGMT_1_REG (uint8_t)0x6BU
#define SMPLRT_DIV_REG (uint8_t)0x19U
#define ACCEL_CONFIG_REG (uint8_t)0x1CU
#define GYRO_CONFIG_REG (uint8_t)0x1BU
#define ACCEL_XOUT_H_REG (uint8_t)0x3BU
#define ACCEL_XOUT_L_REG (uint8_t)0x3CU
#define ACCEL_YOUT_H_REG (uint8_t)0x3DU
#define ACCEL_YOUT_L_REG (uint8_t)0x3EU
#define ACCEL_ZOUT_H_REG (uint8_t)0x3FU
#define ACCEL_ZOUT_L_REG (uint8_t)0x40U
#define GYRO_XOUT_H_REG (uint8_t)67U


enum device_t
{
    ACCEL,
    GYRO
};

struct point_t
{
    double x;
    double y;
    double z;
};

struct RAW_point_t
{
    int16_t x;
    int16_t y;
    int16_t z;
};

void MPU6050_Init(void);
struct point_t MPU6050_Read(int D);
struct RAW_point_t MPU6050_Read_RAW(int D);
void MPU6050_calibration(void);