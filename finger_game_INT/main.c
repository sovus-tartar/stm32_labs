#include <stdint.h>
#include <stdbool.h>
#include "./led.h"
#include "./RCC.h"
#include "./GPIO.h"
#include "./systick.h"
#include "./display.h"
#include "./wait.h"
#include "./button.h"
#include "./led.h"
//-------------------
// RCC configuration
//-------------------

#define ONE_MILLISECOND 48000U

struct led_t player_led[2];
struct button_t player_btn[2];
struct Seg7Display display;

unsigned player_won[2] = {};

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
    REG_RCC_AHBENR_PORT_A_ENABLE(REG_RCC_AHBENR);

    //Enabling display pins
    for (unsigned i = 1; i < 13u; ++i)
        GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOA_MODER, i);
    for (unsigned i = 1; i < 13u; ++i)
        GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOA_TYPER, i);

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


}

void blink(struct led_t * led1, struct led_t * led2) //led 1 high freq, led2 low freq
{
    led_on(led1);

    for(int i = 0; i < 5; ++i)
    {
        led_on(led2);
        wait_nms(5);
        led_off(led2);
        wait_nms(5);
    }

    led_off(led1);
}

void systick_handler(void)
{
    static unsigned handler_ticks = 0U;
    
    handler_ticks = (handler_ticks + 1) % 4;
    SEG7_set_number_quarter(&display, handler_ticks);
    SEG7_push_display_state_to_mc(&display);
    button_update_state(player_btn);
    button_update_state(player_btn + 1);

}   

//------
// Main
//------

int main(void)
{

    unsigned player[2] = {};


    board_clocking_init();
    board_gpio_init();

    button_init(player_btn, GPIOC_IDR, 0, GPIOC_MODER, GPIOC_PUPDR);
    button_init(player_btn + 1, GPIOC_IDR, 1, GPIOC_MODER, GPIOC_PUPDR);

    unsigned btn_old_state[2];
    btn_old_state[0] = button_get_state(player_btn + 0);
    btn_old_state[1] = button_get_state(player_btn + 1);

    led_init(player_led, GPIOC_BSRR, 8, GPIOC_MODER, GPIOC_TYPER);
    led_init(player_led + 1, GPIOC_BSRR, 9, GPIOC_MODER, GPIOC_TYPER);
    display.number = 0;
    systick_init(1000U);

    

while(1)
    {
        unsigned btn_new_state[2];

        btn_new_state[0] = button_get_state(player_btn + 0);
        btn_new_state[1] = button_get_state(player_btn + 1);

        if ((btn_old_state[0] == 1) && (btn_new_state[0] == 1) && (btn_old_state[1] == 0) && (btn_new_state[1] == 1))
        {
            player[1] += 1;
            display.number = 100 * (player[0] % 100) + (player[1] % 100);
            
            blink(player_led +  1, player_led + 0);
        }

        if ((btn_old_state[1] == 1) && (btn_new_state[1] == 1) && (btn_old_state[0] == 0) && (btn_new_state[0] == 1))
        {
            player[0] += 1;
            display.number = 100 * (player[0] % 100) + (player[1] % 100);
            

            blink(player_led + 0, player_led + 1);
        }

        



        btn_old_state[0] = btn_new_state[0];
        btn_old_state[1] = btn_new_state[1];
    }
}
