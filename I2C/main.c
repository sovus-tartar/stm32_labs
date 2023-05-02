#include <stdint.h>
#include <stdbool.h>
#include "./RCC.h"
#include "./GPIO.h"
#include "./I2C.h"

//-------------------
// RCC configuration
//-------------------

#define ONE_MILLISECOND 48000U

struct led_t led8;
struct led_t led9;

void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    REG_RCC_CR_HSEON(REG_RCC_CR);
    WAIT_FOR(REG_RCC_CR, REG_RCC_HSEON_CHECK);
    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    REG_RCC_CFGR2_CONF_PLL(REG_RCC_CFGR2, 2U);

    // (3) Select PREDIV output as PLL input (4 MHz):
    REG_RCC_CFGR_PLL_SET_SRC(REG_RCC_CFGR, HSI_SRC);

    // (4) Set PLLMUL to 12:
    // SYSCLK frequency = 48 MHz
    REG_RCC_CFGR_PLLMUL_SET_MULT(REG_RCC_CFGR, 12);

    // (5) Enable PLL:
    REG_RCC_CR_PLL_ENABLE(REG_RCC_CR);
    WAIT_FOR(REG_RCC_CR, REG_RCC_CR_PLL_ENABLE_CHECK);
    // (6) Configure AHB frequency to 48 MHz:
    REG_RCC_CFGR_SET_AHB(REG_RCC_CFGR, AHB_FREQ_48);

    // (7) Select PLL as SYSCLK source:
    REG_RCC_CFGR_SET_SYSCLK_SRC(REG_RCC_CFGR, PLL_SRC);
    WAIT_FOR(REG_RCC_CFGR, REG_RCC_CFGR_SET_SYSCLK_SRC_CHECK);
    // (8) Set APB frequency to 48 MHz
    REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_1(REG_RCC_CFGR);
}

void I2C_START()
{
    SET_BIT(I2C_CR2, 13);
}

void I2C_STOP()
{
    CLEAR_BIT(I2C_CR2, 13);
}

void I2C_WRITE(int Address, int size, char * data) //nbytes must be below 256
{
    I2C_Master_init();
    SET_BIT(I2C_CR2, 25);
    I2C_SEND_ADDRESS(Address, 0, size);
    I2C_START();

    int i = 0;
    while(1)
    {
        if (READ_BIT(I2C_ISR, 4) == 1)
            break;//???
        
        if(READ_BIT(I2C_ISR, 1) == 0)
            continue;

        WRITE_REG(I2C_TXDR, data[i]);
        ++i;
        if (i == size)
            break;
    }

    I2C_STOP();
}

void  I2C_SEND_ADDRESS(char address, int mode, int size) // mode 1 read, mode 0 write
{
    *I2C_CR2 |= address;  
    CLEAR_BIT(I2C_CR2, 11); //7 bit address
    if (mode)
        SET_BIT(I2C_CR2, 10); //master read mode
    else
        CLEAR_BIT(I2C_CR2, 10);
    
    *I2C_CR2 |= size << 16; 
}

void I2C_READ(int Address, int size, char * buffer)
{
    I2C_Master_init();
    SET_BIT(I2C_CR2, 25);
    I2C_SEND_ADDRESS(Address, 1, size);
    I2C_START();

    int i = 0;

    while(1)
    {
        if(READ_BIT(I2C_ISR, 2) == 0)
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
    SET_BIT(REG_RCC_APB1RSTR, 21); //reset
    SET_BIT(REG_RCC_CFGR3, 1); //set i2c clk src sysclk
    SET_BIT(REG_RCC_APB1ENR, 21);
    SET_BIT(I2C_CR1, 12); //analog filter off
    MODIFY_REG(I2C_CR1, 0b1111u<<8, 0b0001u); //digital filter on
    WRITE_REG(I2C_TIMINGR, 0x60201A27U); //Recomended magic value
}

void board_gpio_init()
{
    SET_BIT(REG_RCC_APB1ENR, 21); //enable I2C CLOCK
    REG_RCC_AHBENR_PORT_B_ENABLE(REG_RCC_AHBENR);
    for(unsigned i = 6; i <= 11; ++i)
    {
        GPIO_MODER_PORT_SET_MODE_ALT(GPIOB_MODER, i);
        GPIO_TYPER_PORT_SET_OPEN_DRAIN(GPIOB_TYPER, i);
        GPIO_OSPEEDR_SET_FAST(GPIOB_OSPEEDR, i);
        GPIO_PUPDR_PORT_PULL_UP_SET(GPIOB_PUPDR, i);
        GPIO_AFRH_SET_ALT(GPIOB_AFRH, i, GPIO_AF1);
    }


}



int main(void)
{
    board_clocking_init();
}

