#include <stdint.h>
#include <stdbool.h>
#include "./led.h"
#include "./RCC.h"
#include "./GPIO.h"
#include "./systick.h"
#include "SYSCFG.h"
#include "NVIC.h"
#include "button_INT.h"

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

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PC8 and PC9:
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);
    REG_RCC_AHBENR_PORT_A_ENABLE(REG_RCC_AHBENR);
    GPIO_MODER_PORT_SET_MODE_ALT(GPIOA_MODER, 0);
    GPIO_MODER_PORT_SET_MODE_ALT(GPIOA_MODER, 1);
    GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOA_TYPER, 0);
    GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOA_TYPER, 1);
    GPIO_PUPDR_PORT_PULL_DOWN_SET(GPIOA_PUPDR, 0);
    GPIO_PUPDR_PORT_PULL_DOWN_SET(GPIOA_PUPDR, 1);

}

void EXTI_init()
{
    SET_BIT(REG_RCC_APB2ENR, 0);
    SET_BIT(REG_RCC_APB2RSTR, 0);
    SYSCFGR_EXTICR1_ENABLE(SYSCFG_EXTICR1, 0, 'A');
    SYSCFGR_EXTICR1_ENABLE(SYSCFG_EXTICR1, 1, 'A');
    //*SYSCFG_EXTICR1 |= 0b00000000u;

    EXTI_IMR_UNMASK_IRQ(EXTI_IMR, 0);
    EXTI_IMR_UNMASK_IRQ(EXTI_IMR, 1);
    EXTI_RTSR_ENABLE(EXTI_RTSR, 0);
    EXTI_RTSR_ENABLE(EXTI_RTSR, 1);
    EXTI_FTSR_ENABLE(EXTI_RTSR, 0);
    EXTI_FTSR_ENABLE(EXTI_RTSR, 1);

    SET_BIT(NVIC_ISER, 5);
    *NVIC_IPR0 = 0b00000000U;
}

void IRQ0_handler(void)
{
    if (READ_BIT(EXTI_PR, 0) == 1)
    {
        led_on(&led8);
        SET_BIT(EXTI_PR, 0);
    }
    else
    {
        led_off(&led8);
        SET_BIT(EXTI_PR, 1);
    }
}


//------
// Main
//------

int main(void)
{

    board_clocking_init();
    board_gpio_init();
    EXTI_init();

    led_init(&led8, GPIOC_BSRR, 8, GPIOC_MODER, GPIOC_TYPER);
    led_init(&led9, GPIOC_BSRR, 9, GPIOC_MODER, GPIOC_TYPER);

    led_on(&led9);
    while(1)
    {
        led_on(&led9);
        timing_perfect_delay(1000);
        led_off(&led9);
        timing_perfect_delay(1000);
    }

}

