#include "display.h"
#include "GPIO.h"
#include "bit_arithmetic.h"
#include "wait.h"

void SEG7_set_number_quarter(struct Seg7Display* seg7, unsigned tick)
{
    uint32_t divisors[4] = {1, 10, 100, 1000};

    unsigned quarter = tick % 4;
    unsigned divisor = divisors[quarter];

    seg7->display = DIGITS[(seg7->number / divisor) % 10] | POSITIONS[quarter];
}

// Write changes to microcontroller:
void SEG7_push_display_state_to_mc(struct Seg7Display* seg7)
{
    uint32_t surrounding_state = ~PINS_USED & *GPIOA_ODR;
    uint32_t to_write = PINS_USED & seg7->display;

    WRITE_REG(GPIOA_ODR, (surrounding_state | ~to_write));
}

void SEG7_set_and_push(struct Seg7Display * seg7, unsigned num)
{
    seg7->number = num;

    for(int i = 0; i < 4; ++i)
    {
        SEG7_set_number_quarter(seg7, i);
        SEG7_push_display_state_to_mc(seg7);
        wait_1ms();
        
        
    }
}

