#include <stdint.h>
#include <stdbool.h>

#include "bit_arithmetic.h"
#include "RCC.h"
#include "GPIO.h"
#include "display.h"
#include "button.h"
//-------------------
// RCC configuration
//-------------------

#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY / 1000U

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

void wait_1_ms()
{
    for (uint32_t i = 0; i < (ONE_MILLISECOND / 6U); ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PA1-PA12 as output:
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);

    for (unsigned i = 1; i < 13; ++i)
        GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, i);

    for (unsigned i = 1; i < 13; ++i)
        GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOC_TYPER, i);
}

//------
// Main
//------

int main()
{
    board_clocking_init();

    board_gpio_init();

    // Init display rendering:
    struct Seg7Display seg7 =
        {
            .number = 0
        };

    struct button_t btn;
    button_init(&btn, GPIOC_IDR, 0, GPIOC_TYPER, GPIOC_MODER, GPIOC_PUPDR);
    unsigned i = 0;

    for (i = 0; i < 9999u; ++i)
    {
        if (button_get_state(&btn) != 1)
            SEG7_set_and_push(&seg7, i);
        else 
        {
            break;
        }
        wait_1_ms();
    }

    while(1)
    {
        SEG7_set_and_push(&seg7, i);
    }
} 

