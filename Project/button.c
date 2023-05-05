#include "button.h"
#include "GPIO.h"
#include "bit_arithmetic.h"


struct button_t * button_init(struct button_t * btn, volatile uint32_t *reg, unsigned bit, volatile uint32_t * typer, volatile uint32_t * moder, volatile uint32_t * pupdr)
{
    moder = moder;
    GPIO_TYPER_PORT_SET_PUSH_PULL(typer, bit);
    //GPIO_MODER_PORT_SET_MODE_INPUT(moder, bit);
    GPIO_PUPDR_PORT_PULL_DOWN_SET(pupdr, bit);


    btn->state = 0u;
    btn->bit = bit;
    btn->reg = reg;

    return btn;
}

unsigned button_get_immediate_state(struct button_t * btn)
{
    if (btn->reg == 0U)
        return -1;

    return READ_BIT(btn->reg, btn->bit);
}

