#include <stdint.h>
#include <stdbool.h>

#include "bit_arithmetic.h"
#include "RCC.h"
#include "GPIO.h"
#include "display.h"
#include "button.h"
#include "led.h"
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
    // (1) Enable GPIOC clocking:
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);

    // (2) Configure PC8 and PC9 mode:
    GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, 8);
    GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, 9);


    // (3) Configure PC8 and PC9 type output push-pull set:
    GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOC_TYPER, 8);
    GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOC_TYPER, 9);

}



void timing_perfect_delay(uint32_t millis)
{
    unsigned ticks;
    ticks = millis * (ONE_MILLISECOND) / 10;

    __asm__(
        "1: ldr r3, [r7, #12] \n\t"//2
        "cmp r3, #0 \n\t"//1
        "beq 2f \n\t" //1
        "sub r3, #1 \n\t" //1
        "str r3, [r7, #12] \n\t" //2
        "b 1b \n\t" //3
        "2: \n\t"
    );

    ticks += 0;

}

//------
// Main
//------

int main()
{
    struct led_t led1;
    led_init(&led1, GPIOC_BSRR, 8, GPIOC_MODER, GPIOC_TYPER);
    board_clocking_init();

    board_gpio_init();

    led_on(&led1);
    timing_perfect_delay(1000 * 10); //30 seconds for measure accuracy
    led_off(&led1);
} 

