#include <stdint.h>
#include "I2C.h"
#include "MPU60XX.h"

#define MPU6050_ADDR (uint8_t)0b1101000U
#define WHO_AM_I_REG (uint8_t)0x75U
#define PWR_MGMT_1_REG (uint8_t)0x6BU
#define SMPLRT_DIV_REG (uint8_t)0x19U
#define ACCEL_CONFIG_REG (uint8_t)0x1CU
#define GYRO_CONFIG_REG (uint8_t)0x1BU
#define ACCEL_XOUT_H_REG (uint8_t)0x3BU

void MPU_read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
    
    I2C_SEND_ADDRESS(Address, 0, 1);
    I2C_START();
    I2C_WRITE(Address, 1, &Reg);
    I2C_STOP();
    I2C_SEND_ADDRESS(Address, 1, size);
    I2C_START();
    I2C_READ(Address + 0b1u, size, buffer);
    I2C_STOP();
}

void MPU_write(uint8_t Address, uint8_t Reg, uint8_t Data)
{
    unsigned char temp[2] = {Reg, Data};
    I2C_SEND_ADDRESS(Address, 0, 2);
    I2C_START();
    I2C_WRITE(Address, 2, temp);
    I2C_STOP();

}

void MPU6050_Init(void)
{
    uint8_t check[10];
    uint8_t Data;

    // check device ID WHO_AM_I

    MPU_read(MPU6050_ADDR, 117u, check, 1);
    MPU_read(MPU6050_ADDR, 117u, check, 1);
    MPU_read(MPU6050_ADDR, 117u, check, 1);
    MPU_read(MPU6050_ADDR, 117u, check, 1);
    if (*check == 104) // 0x68 will be returned by the sensor if everything goes well
    {
        // power management register 0X6B we should write all 0's to wake the sensor up
        Data = 0;

        MPU_write(MPU6050_ADDR, PWR_MGMT_1_REG, Data);
        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
        Data = 0x07;
        MPU_write(MPU6050_ADDR, SMPLRT_DIV_REG, Data);

        // Set accelerometer configuration in ACCEL_CONFIG Register
        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ? 2g
        Data = 0x00;
        MPU_write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);
       
        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s
        Data = 0x00;
        MPU_write(MPU6050_ADDR, GYRO_CONFIG_REG, Data);
       
    }
}

struct point_t MPU6050_Read_Accel(void)
{
    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    uint8_t Rx_data[6];
    struct point_t temp;

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register
    MPU_read(MPU6050_ADDR, ACCEL_XOUT_H_REG, Rx_data, 6);

    Accel_X_RAW = (int16_t)(Rx_data[1] << 8 | Rx_data[0]);
    Accel_Y_RAW = (int16_t)(Rx_data[3] << 8 | Rx_data[2]);
    Accel_Z_RAW = (int16_t)(Rx_data[5] << 8 | Rx_data[4]);

    /*** convert the RAW values into acceleration in 'g'
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 16384.0
         for more details check ACCEL_CONFIG Register              ****/

    temp.x = (Accel_X_RAW/16384.0) * 1000;
    temp.y = (Accel_Y_RAW/16384.0) * 1000;
    temp.z = (Accel_Z_RAW/16384.0) * 1000;

    return temp;
}