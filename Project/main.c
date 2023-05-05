#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "RCC.h"
#include "GPIO.h"
#include "I2C.h"
#include "uart.h"
#include "GPIO.h"
#include "MPU60XX.h"
#include "systick.h"
#include "button.h"
#include "bit_arithmetic.h"
#include "SYSCFG.h"
#include "NVIC.h"

volatile struct point_t state_accel;
volatile struct point_t state_gyro;

volatile struct point_t angles = {}; // only x, y
struct button_t btn[2];


#define PI 3.1415
//-------------------
// RCC configuration
//-------------------

#define ONE_MILLISECOND 48000U

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

void board_gpio_init()
{
    SET_BIT(REG_RCC_APB1ENR, 21); // enable I2C CLOCK
    REG_RCC_AHBENR_PORT_B_ENABLE(REG_RCC_AHBENR);
    for (unsigned i = 6; i <= 9; ++i)
    {
        GPIO_MODER_PORT_SET_MODE_ALT(GPIOB_MODER, i);
        GPIO_TYPER_PORT_SET_OPEN_DRAIN(GPIOB_TYPER, i);
        GPIO_OSPEEDR_SET_FAST(GPIOB_OSPEEDR, i);
        GPIO_PUPDR_PORT_PULL_UP_SET(GPIOB_PUPDR, i);
        GPIO_AFRL_SET_ALT(GPIOB_AFRL, i, GPIO_AF1);
    }

    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);
    REG_RCC_AHBENR_PORT_A_ENABLE(REG_RCC_AHBENR);

    // Set alternate functions:
    GPIO_AFRH_SET_ALT(GPIOA_AFRH, 9, GPIO_AF1);
    GPIO_AFRH_SET_ALT(GPIOA_AFRH, 10, GPIO_AF1);

    // Configure pin operating speed:
    GPIO_OSPEEDR_SET_FAST(GPIOA_OSPEEDR, 9);
    GPIO_OSPEEDR_SET_FAST(GPIOA_OSPEEDR, 10);

    GPIO_MODER_PORT_SET_MODE_ALT(GPIOA_MODER, 9);
    GPIO_MODER_PORT_SET_MODE_ALT(GPIOA_MODER, 10);
}

void totally_accurate_quantum_femtosecond_precise_super_delay_3000_1ms()
{
    for (uint32_t i = 0; i < 1U * ONE_MILLISECOND; ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}

void systick_handler(void)
{
    static unsigned counter = 1U;
    static unsigned old_state[2];
    state_accel = MPU6050_Read(ACCEL);
    state_gyro = MPU6050_Read(GYRO);

    angles.y = atan(state_accel.y / state_accel.x) * 180 / (PI);
    angles.z = atan(state_accel.z / state_accel.x) * 180 / (PI);
    if (fabs(state_gyro.x) >= 1.5)
        angles.x += state_gyro.x * 1 / 54.7;

    if (counter == 1)
    {
        old_state[0] = btn[0].state;

    }
    else if (counter < 4)
    {
        if (old_state[0] == button_get_immediate_state(btn))
            counter = 0;
    }
    else
    {
        btn[0].state = !old_state[0];

        counter = 0;
    }

    uart_send_byte((char)angles.y);
    uart_send_byte((char)angles.z);
    uart_send_byte(btn[0].state);
    uart_send_byte(btn[1].state);

    counter++;
}

int main()
{
    board_clocking_init();
    board_gpio_init();

    button_init(btn, GPIOA_IDR, 0, GPIOA_TYPER, GPIOA_MODER, GPIOA_PUPDR);
    button_init(btn + 1, GPIOA_IDR, 1, GPIOA_TYPER, GPIOA_MODER, GPIOA_PUPDR);
    uart_init(7200U, 48000000U); // why i have 1200*6?
    uart_send_byte(31);

    I2C_Master_init();
    MPU6050_Init();
    MPU6050_calibration();
    systick_init(10000U);

    while (1)
    {
        __asm__ volatile("nop");
        /*uart_print_int(angles.x);
        uart_print_int(angles.y);
        uart_send_byte('\n');
        uart_send_byte('\r');*/
    }
}
