#include "bit_arithmetic.h"
#include "button.h"
#include "display.h"
#include "GPIO.h"
#include "RCC.h"
#include "led.h"
#include "wait.h"

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

void board_gpio_init(struct button_t * btn1, struct button_t * btn2, struct led_t * led1, struct led_t * led2)
{
    REG_RCC_AHBENR_PORT_A_ENABLE(REG_RCC_AHBENR);
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);

    //Enabling display pins
    for (unsigned i = 1; i < 13u; ++i)
        GPIO_MODER_PORT_SET_MODE_OUTPUT(GPIOA_MODER, i);
    for (unsigned i = 1; i < 13u; ++i)
        GPIO_TYPER_PORT_SET_PUSH_PULL(GPIOA_TYPER, i);

    button_init(btn1, GPIOC_IDR, 0, GPIOC_MODER, GPIOC_PUPDR);
    button_init(btn2, GPIOC_IDR, 1, GPIOC_MODER, GPIOC_PUPDR);
    led_init(led1, GPIOC_BSRR, 8, GPIOC_MODER, GPIOC_TYPER);
    led_init(led2, GPIOC_BSRR, 9, GPIOC_MODER, GPIOC_TYPER);
}


void blink(struct led_t * led1, struct led_t * led2, unsigned display_num, struct Seg7Display * display) //led 1 high freq, led2 low freq
{
    for(int i = 0; i < 10; ++i)
    {
        led_on(led1);
        SEG7_set_and_push(display, display_num);
        led_off(led1);
        SEG7_set_and_push(display, display_num);

        if(i % 5 == 0)
        {
            led_on(led2);
            SEG7_set_and_push(display, display_num);
            led_off(led2);
            SEG7_set_and_push(display, display_num);
        }

        SEG7_set_and_push(display, display_num);
    }
}

int main()
{
    struct button_t player_btn[2];
    struct led_t player_led[2];
    struct Seg7Display display;

    unsigned display_count = 0;
    unsigned player[2] = {};

    board_clocking_init();
    board_gpio_init(player_btn, player_btn + 1, player_led, player_led + 1);

    unsigned btn_old_state[2];
    btn_old_state[0] = button_get_state(player_btn + 0);
    btn_old_state[1] = button_get_state(player_btn + 1);

    SEG7_set_and_push(&display, 0);

    while(1)
    {
        unsigned btn_new_state[2];

        btn_new_state[0] = button_get_state(player_btn + 0);
        btn_new_state[1] = button_get_state(player_btn + 1);

        if ((btn_old_state[0] == 1) && (btn_new_state[0] == 1) && (btn_old_state[1] == 0) && (btn_new_state[1] == 1))
        {
            player[1] += 1;
            display_count = 100 * (player[0] % 100) + (player[1] % 100);
            
            blink(player_led +  1, player_led + 0, display_count, &display);
        }

        if ((btn_old_state[1] == 1) && (btn_new_state[1] == 1) && (btn_old_state[0] == 0) && (btn_new_state[0] == 1))
        {
            player[0] += 1;
            display_count = 100 * (player[0] % 100) + (player[1] % 100);
            

            blink(player_led + 0, player_led + 1, display_count, &display);
        }

        

        SEG7_set_and_push(&display, display_count);

        btn_old_state[0] = btn_new_state[0];
        btn_old_state[1] = btn_new_state[1];
    }
}