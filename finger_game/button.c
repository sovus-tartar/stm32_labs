#include "button.h"
#include "GPIO.h"
#include "bit_arithmetic.h"
#include "wait.h"

struct button_t * button_init(struct button_t * btn, volatile uint32_t *reg, unsigned bit, volatile uint32_t * typer, volatile uint32_t * moder, volatile uint32_t * pupdr)
{
    GPIO_MODER_PORT_SET_MODE_INPUT(moder, bit);
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

int button_get_state(struct button_t * btn)
{
    unsigned old_state = btn->state;
    for(int i = 0; i < 50; ++i)
    {
        
        if (old_state == button_get_immediate_state(btn))
            return old_state;
    }

    return (btn->state = !old_state);    
}