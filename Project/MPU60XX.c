#include <stdint.h>
#include "I2C.h"
#include "MPU60XX.h"
#include "uart.h"


int16_t MPU_Accel_Offset[3] = {1, 1, 1}; //X, Y, Z offfset
int16_t MPU_Gyro_Offset[3] = {1, 1, 1}; //X, Y, Z offfset

void MPU_read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
    
    I2C_SEND_ADDRESS(Address, 0, 1);
    I2C_START();
    I2C_WRITE(1, &Reg);
    I2C_STOP();
    I2C_SEND_ADDRESS(Address, 1, size);
    I2C_START();
    I2C_READ(size, buffer);
    I2C_STOP();
}

void MPU_write(uint8_t Address, uint8_t Reg, uint8_t Data)
{
    unsigned char temp[2] = {Reg, Data};
    I2C_SEND_ADDRESS(Address, 0, 2);
    I2C_START();
    I2C_WRITE(2, temp);
    I2C_STOP();

}

void MPU6050_Init(void)
{
    uint8_t check[10];
    uint8_t Data;

    // check device ID WHO_AM_I

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
        Data = 0b0u;
        MPU_write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);
        
        //Set Gyroscopic configuration in GYRO_CONFIG Register
        //XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s
        Data = 0x00;
        MPU_write(MPU6050_ADDR, GYRO_CONFIG_REG, Data);
       
    }
}

void MPU6050_calibration(void) 
{
    int16_t temp_offset_ACCEL[3] = {0};
    int16_t temp_offset_GYRO[3] = {0};
    //We want x = 16384, y = 0, z = 0 for accel
    //and x=0 y=0 z=0 for gyro
    MPU6050_Read_RAW(ACCEL);
    MPU6050_Read_RAW(GYRO);
    for (int i = 0; i < 10; ++i)
    {
        struct RAW_point_t state_a = MPU6050_Read_RAW(ACCEL);
        struct RAW_point_t state_g = MPU6050_Read_RAW(GYRO);

        temp_offset_ACCEL[0] += (16384 - state_a.x);
        temp_offset_ACCEL[1] += (0 - state_a.y);
        temp_offset_ACCEL[2] += (0 - state_a.z);
        temp_offset_GYRO[0] += (0 - state_g.x);
        temp_offset_GYRO[1] += (0 - state_g.y);
        temp_offset_GYRO[2] += (0 - state_g.z);
    
    }
    for(int i = 0; i < 3; ++i)
    {
        MPU_Accel_Offset[i] = temp_offset_ACCEL[i] / 10;
        MPU_Gyro_Offset[i] = temp_offset_GYRO[i] / 10;
    }
}

struct point_t MPU6050_Read(int D)
{
    struct RAW_point_t state = MPU6050_Read_RAW(D);
    double divider = 32768.0;
    if(D == GYRO)
        divider /= 250;
    else
        divider /= 2;

    struct point_t temp =
    {
        state.x/divider,
        state.y/divider,
        state.z/divider
    };

    return temp;
}

struct RAW_point_t MPU6050_Read_RAW(int D)
{
    uint8_t Rx_data[6] = {};
    uint8_t addr;

    if (D == GYRO)
        addr = GYRO_XOUT_H_REG;
    else
        addr = ACCEL_XOUT_H_REG;

    struct RAW_point_t temp = {};

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register
    MPU_read(MPU6050_ADDR, addr, Rx_data, 6);
    
    temp.x = ((int16_t)(Rx_data[1] << 8 | Rx_data[0])) + ((D == ACCEL)?(MPU_Accel_Offset[0]):(MPU_Gyro_Offset[0]));
    temp.y = (int16_t)(Rx_data[3] << 8 | Rx_data[2]) + ((D == ACCEL)?(MPU_Accel_Offset[1]):(MPU_Gyro_Offset[1]));
    temp.z = (int16_t)(Rx_data[5] << 8 | Rx_data[4]) + ((D == ACCEL)?(MPU_Accel_Offset[2]):(MPU_Gyro_Offset[2]));

    return temp;
}

