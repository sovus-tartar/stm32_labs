#include "button.h"

struct button_t * button_init(struct button_t * btn)
{
    btn->state = 0u;
    return btn;
}

// No need to eliminate contact bounce due to Schmidt trigger, see page 155 on stm32f0xx_rm.pdf
void button_INT_update_state(struct button_t * btn)
{
    btn->state = !btn->state;
}

int button_INT_get_state(struct button_t * btn)
{
    return btn->state;
}
