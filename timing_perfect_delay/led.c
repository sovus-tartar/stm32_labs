#include "led.h"
#include "bit_arithmetic.h"
#include "GPIO.h"

struct led_t * led_init(struct led_t * led, volatile uint32_t *bsrr_reg, unsigned bit, volatile uint32_t * moder, volatile uint32_t * typer)
{
    GPIO_MODER_PORT_SET_MODE_OUTPUT(moder, bit);
    GPIO_TYPER_PORT_SET_PUSH_PULL(typer, bit);

    led->bit = bit;
    led->bsrr_reg = bsrr_reg;

    return led;
}

void led_on(struct led_t * led)
{
    SET_BIT(led->bsrr_reg, led->bit);
}

void led_off(struct led_t * led)
{
    SET_BIT(led->bsrr_reg, led->bit + 16);
}
