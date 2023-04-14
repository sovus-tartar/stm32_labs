#include <stdint.h>
#include <stdbool.h>
#include "./led.h"
#include "./RCC.h"
#include "./GPIO.h"
#include "./systick.h"

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

#define ONE_SEC_DELAY_TIME 3692308U // 48000000 / 13
void more_precise_delay_forbidden_by_quantum_mechanics_1000ms()
{
    for (uint32_t i = 0; i < ONE_SEC_DELAY_TIME; ++i);
}

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PC8 and PC9:
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);
}

//-----------------------
// SysTick configuration
//-----------------------

void systick_init(uint32_t period_us)
{
    // (0) Read STM32F051 SysTick configuration:
    // Assumptions:
    // - There is a reference clock and it can be chosen as clock source.
    // - The SYST_CALIB SKEW bit is 1.
    // (1) Program the reload value:
    SYSTICK_RVR_RELOAD_TIME_MS(SYSTICK_RVR, period_us);

    // (2) Clear the current value:
    SYSTICK_CVR_CLEAR_VAL(SYSTICK_CVR);

    // (3) Program the CSR:
    // Watch out for the clock source!
    SYSTICK_CSR_COUNTER_ENABLE(SYSTICK_CSR);
    SYSTICK_CSR_TICKINT_ENABLE(SYSTICK_CSR);

    SYSTICK_CALIB_SKEW_EXACT(SYSTICK_CALIB);
}

void systick_handler(void)
{
    static unsigned handler_ticks = 0U;
    static unsigned tick_cntr = 0u;


    handler_ticks += 1U;

    if (handler_ticks == 10000U)
    {
        handler_ticks = 0U;
        tick_cntr  = (tick_cntr + 1) % 2;

        switch(tick_cntr)
        {
            case 0u:
                led_on(&led9);
                break;
            case 1u:
                led_off(&led9);
                break;
        }
    }
}

//------
// Main
//------

int main(void)
{

    board_clocking_init();
    board_gpio_init();

    systick_init(100U);

    led_init(&led8, GPIOC_BSRR, 8, GPIOC_MODER, GPIOC_TYPER);
    led_init(&led9, GPIOC_BSRR, 9, GPIOC_MODER, GPIOC_TYPER);

    while (1)
    {
        more_precise_delay_forbidden_by_quantum_mechanics_1000ms();

        led_on(&led8);

        more_precise_delay_forbidden_by_quantum_mechanics_1000ms();

        led_off(&led8);       
    }

}
