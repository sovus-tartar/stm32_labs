#pragma once

#include "bit_arithmetic.h"

//----------------
// GPIO Registers
//----------------

#define GPIOA_MODER (volatile uint32_t*)(uintptr_t)0x48000000U // GPIO port mode register
#define GPIOA_TYPER (volatile uint32_t*)(uintptr_t)0x48000004U // GPIO port output type register
#define GPIOA_PUPDR (volatile uint32_t*)(uintptr_t)0x4800000CU // GPIO port pull-up/pull-down register
#define GPIOA_IDR   (volatile uint32_t*)(uintptr_t)0x48000010U // GPIO port input  data register
#define GPIOA_ODR   (volatile uint32_t*)(uintptr_t)0x48000014U // GPIO port output data register

#define GPIOC_MODER (volatile uint32_t*)(uintptr_t)0x48000800U      // GPIO port mode register
#define GPIOC_TYPER (volatile uint32_t*)(uintptr_t)0x48000804U      // GPIO port output type register
#define GPIOC_ODR (volatile uint32_t*)(uintptr_t) 0x48000814U       // GPIO port output register
#define GPIOC_BSRR (volatile uint32_t*)(uintptr_t) 0x48000818U      // GPIO port bit set/reset register


//--------------
// GPIO macro
//--------------
#define GPIO_MODER_PORT_SET_MODE_OUTPUT(REG, BIT) (SET_BIT((REG), 2u * (BIT)))
#define GPIO_MODER_PORT_SET_MODE_INPUT(REG, BIT) (*(REG) = *(REG) & (~(0b11u << (BIT))))
#define GPIO_TYPER_PORT_SET_PUSH_PULL(REG, BIT) (CLEAR_BIT((REG), (BIT)))
#define GPIO_TYPER_PORT_SET_OPEN_DRAIN(REG, BIT) (SET_BIT((REG), (BIT)))
#define GPIO_BSRR_BIT_SET(REG, BIT) (SET_BIT(REG, BIT))
#define GPIO_BSRR_BIT_RESET(REG, BIT) (SET_BIT(REG, (BIT + 16)))
//to debug
#define GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT) (*(REG) = *(REG) & (~(0b11u << (2 * BIT))))
#define GPIO_PUPDR_PORT_PULL_UP_SET(REG, BIT) ({GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT); SET_BIT((REG), 2u * (BIT));})
#define GPIO_PUPDR_PORT_PULL_DOWN_SET(REG, BIT) ({GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT); SET_BIT((REG), (2u  * (BIT) + 1u));})


