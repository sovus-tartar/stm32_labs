#pragma once

#include <stdint.h>

struct point_t
{
    unsigned int x;
    unsigned int y;
    unsigned int z;
};

void MPU6050_Init (void);
struct point_t MPU6050_Read_Accel (void);

