#include "I2C.h"
#include "RCC.h"
#include "bit_arithmetic.h"


void I2C_START()
{
    SET_BIT(I2C_CR2, 13);
}

void I2C_STOP()
{
    SET_BIT(I2C_CR2, 14);
}

void I2C_WRITE(int Address, int size, unsigned char *data) // nbytes must be below 256
{
    Address = Address;

    int i = 0;

    while (1)
    {
        if (READ_BIT(I2C_ISR, 4) == 1)
            break;   

        if (READ_BIT(I2C_ISR, 0) == 0)
            continue;
        if (i == size)
            break;
        
        WRITE_REG(I2C_TXDR, data[i]);
        ++i;
        
    }



    while(!READ_BIT(I2C_ISR, 0))
    {
        continue;
    }

}

void I2C_SEND_ADDRESS(unsigned char address, int mode, int size) // mode 1 read, mode 0 write
{
    //SET_BIT(I2C_CR2, 25);
    *I2C_CR2 |= address << 1;
    CLEAR_BIT(I2C_CR2, 11); // 7 bit address
    if (mode)
        SET_BIT(I2C_CR2, 10); // master read mode
    else
        CLEAR_BIT(I2C_CR2, 10); // master write mode

    *I2C_CR2 |= size << 16;
}

void I2C_READ(int Address, int size, unsigned char *buffer)
{
    Address = Address;
    

    int i = 0;

    while (1)
    {
        if (READ_BIT(I2C_ISR, 2) == 0)
            continue;

        buffer[i] = READ_REG(I2C_RXDR);
        ++i;

        if (i == size)
            break;
    }

}

void I2C_Master_init()
{
    CLEAR_BIT(I2C_CR1, 0);
    SET_BIT(REG_RCC_APB1RSTR, 21); // reset
    CLEAR_BIT(REG_RCC_APB1RSTR, 21);
    SET_BIT(REG_RCC_CFGR3, 1);     // set i2c clk src sysclk
    SET_BIT(REG_RCC_APB1ENR, 21);
    SET_BIT(I2C_CR1, 12);                       // analog filter off
    MODIFY_REG(I2C_CR1, 0b1111u << 8, 0b0001u << 8); // digital filter on
    WRITE_REG(I2C_TIMINGR, 0xC0416DFDU);        // Recomended magic value
    //SET_BIT(I2C_CR1, 17); //NOSTRETCH
    SET_BIT(I2C_CR1, 0);
}