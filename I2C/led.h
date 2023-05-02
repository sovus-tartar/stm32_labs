#pragma once
#include <stdint.h>

struct led_t 
{
    volatile uint32_t * bsrr_reg;
    unsigned bit;
};

struct led_t * led_init(struct led_t * led, volatile uint32_t *bsrr_reg, unsigned bit, volatile uint32_t * moder, volatile uint32_t * typer);
void led_on(struct led_t * led);
void led_off(struct led_t * led);
