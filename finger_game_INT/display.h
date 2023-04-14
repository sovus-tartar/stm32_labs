#pragma once
#include <stdint.h>

//-------------------
// 7-segment display
//-------------------

// Actually trustworthy Pin Mapping:
#define A ((1U << 11))
#define B ((1U << 7))
#define C ((1U << 4))
#define D ((1U << 2))
#define E ((1U << 1))
#define F ((1U << 10)) 
#define G ((1U << 5))
#define DP ((1U << 3))

#define POS0 (1U << 12)
#define POS1 (1U << 9)
#define POS2 (1U << 8)
#define POS3 (1U << 6)

static const uint32_t PINS_USED = A|B|C|D|E|F|G|DP|POS0|POS1|POS2|POS3;

// Exactly correct digit composition:
static const uint32_t DIGITS[10] =
{
    A|B|C|D|E|F,   // 0
    B|C,           // 1
    A|B|G|E|D,    // 2
    A|B|C|D|G,     // 3
    B|C|F|G,       // 4
    A|F|G|C|D,     // 5
    A|C|D|E|F|G,   // 6
    A|B|C,         // 7
    A|B|C|D|E|F|G, // 8
    A|B|C|D|F|G    // 9
};

static const uint32_t POSITIONS[4] =
{
         POS1|POS2|POS3, // 0
    POS0     |POS2|POS3, // 1
    POS0|POS1     |POS3, // 2
    POS0|POS1|POS2       // 3
};

// Display state:
struct Seg7Display
{
    uint32_t display;
    uint16_t number;
};

void SEG7_set_number_quarter(struct Seg7Display* seg7, unsigned tick);
void SEG7_push_display_state_to_mc(struct Seg7Display* seg7);
void SEG7_set_and_push(struct Seg7Display * seg7, unsigned num);
