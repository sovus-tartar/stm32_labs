#include "wait.h"
#include <stdint.h>

void wait_1ms()
{

    for (uint32_t i = 0; i < (48000 / 3U); ++i)
    {
        // Insert NOP for power consumption:
        __asm__ volatile("nop");
    }
}