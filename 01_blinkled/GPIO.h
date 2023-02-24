#pragma once

#include "bit_arithmetic.h"

//----------------
// GPIO Registers
//----------------

#define GPIOC_MODER (volatile uint32_t*)(uintptr_t)0x48000800U      // GPIO port mode register
#define GPIOC_TYPER (volatile uint32_t*)(uintptr_t)0x48000804U      // GPIO port output type register
#define GPIOC_ODR (volatile uint32_t*)(uintptr_t) 0x48000814U       // GPIO port output register
#define GPIOC_BSRR (volatile uint32_t*)(uintptr_t) 0x48000818U      // GPIO port bit set/reset register

//--------------
// GPIO macro
//--------------
#define GPIO_MODER_PORT_SET_MODE_OUTPUT(REG, BIT) (SET_BIT((REG), 2*(BIT)))
#define GPIO_TYPER_PORT_SET_PUSH_PULL(REG, BIT) (CLEAR_BIT((REG), (BIT)))
#define GPIO_BSRR_BIT_SET(REG, BIT) (SET_BIT(REG, BIT))
#define GPIO_BSRR_BIT_RESET(REG, BIT) (SET_BIT(REG, (BIT + 16)))
