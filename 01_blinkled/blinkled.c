#include <stdint.h>
#include "GPIO.h"
#include "RCC.h"
#include "bit_arithmetic.h"

//------
// Main
//------
#define ONE_MILLISECOND (CPU_FREQENCY / 1000U)

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
    // (8) Set APB frequency to 24 MHz
    REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_2(REG_RCC_CFGR);
}


void board_gpio_init()
{
    // (1) Enable GPIOC clocking:
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);

    // (2) Configure PC8 and PC9 mode:
    GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, 8);
    GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, 9);


    // (3) Configure PC8 and PC9 type output push-pull set:
    GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOC_TYPER, 8);
    GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOC_TYPER, 9);

}

void totally_accurate_quantum_femtosecond_precise_super_delay_3000_10ms()
{
    for (uint32_t i = 0; i < 10U * ONE_MILLISECOND; ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}

int main()
{
#ifndef INSIDE_QEMU
    board_clocking_init();
#endif

    board_gpio_init();

    while (1)
    {
        GPIO_BSRR_BIT_SET(GPIOC_BSRR, 8);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_10ms();
        GPIO_BSRR_BIT_RESET(GPIOC_BSRR, 8);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_10ms();
        GPIO_BSRR_BIT_SET(GPIOC_BSRR, 9);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_10ms();
        GPIO_BSRR_BIT_RESET(GPIOC_BSRR, 9);
        totally_accurate_quantum_femtosecond_precise_super_delay_3000_10ms();
    }
}

