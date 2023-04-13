#pragma once
#include <stdint.h>

struct button_t
{   unsigned state;
    volatile uint32_t * reg;
    unsigned bit;
};

struct button_t * button_init(struct button_t * btn, volatile uint32_t *reg, unsigned bit, volatile uint32_t * typer, volatile uint32_t * moder, volatile uint32_t * pupdr);
int button_get_state(struct button_t * btn);