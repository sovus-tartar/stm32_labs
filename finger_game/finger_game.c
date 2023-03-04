#include "bit_arithmetic.h"
#include "button.h"
#include "display.h"
#include "GPIO.h"
#include "RCC.h"


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

void board_gpio_init(struct button_t * btn1, struct button_t * btn2)
{
    REG_RCC_AHBENR_PORT_A_ENABLE(REG_RCC_AHBENR);
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);


    for (unsigned i = 1; i < 13u; ++i)
        GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOA_MODER, i);

    for(unsigned i = 8; i <= 0; ++i)
        GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOC_MODER, i);

    for(unsigned i = 0; i < 2u; ++i)
        GPIO_MODER_PORT_SET_MODE_INPUT(GPIOC_MODER, i);

    for (unsigned i = 1; i < 12u; ++i)
        GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOA_TYPER, i);

    button_init(btn1, GPIOC_IDR, 0, GPIOC_TYPER, GPIOC_MODER, GPIOC_PUPDR);
    button_init(btn2, 1, GPIOC_TYPER, GPIOC_MODER, GPIOC_PUPDR);

}

void led_on(volatile uint32_t * bsrr, unsigned bit)
{
    GPIO_BSRR_BIT_SET(GPIOC_BSRR, bit);
}

void led_off(volatile uint32_t * bsrr, unsigned bit)
{
    GPIO_BSRR_BIT_RESET(GPIOC_BSRR, bit);
}

int main()
{
    struct button_t player1_btn, player2_btn;
    
    board_clocking_init(&player1_btn, &player2_btn);

    
}