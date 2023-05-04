#pragma once

#include <stdint.h>

struct point_t
{
    float x;
    float y;
    float z;
};

void MPU6050_Init (void);
struct point_t MPU6050_Read_Accel (void);

