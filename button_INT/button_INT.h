#pragma once
#include <stdint.h>

struct button_t
{
    unsigned state;
};

void button_INT_update_state(struct button_t * btn);
int button_INT_get_state(struct button_t * btn);
