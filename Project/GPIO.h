#pragma once

#include "bit_arithmetic.h"

//------------------
// Alternative modes
//------------------

#define GPIO_AF0 0b0000u
#define GPIO_AF1 0b0001u
#define GPIO_AF2 0b0010u
#define GPIO_AF3 0b0011u
#define GPIO_AF4 0b0100u
#define GPIO_AF5 0b0101u
#define GPIO_AF6 0b0110u
#define GPIO_AF7 0b0111u

//----------------
// GPIO Registers
//----------------

#define GPIOA_MODER (volatile uint32_t*)(uintptr_t)0x48000000U // GPIO port mode register
#define GPIOA_TYPER (volatile uint32_t*)(uintptr_t)0x48000004U // GPIO port output type register
#define GPIOA_PUPDR (volatile uint32_t*)(uintptr_t)0x4800000CU // GPIO port pull-up/pull-down register
#define GPIOA_IDR   (volatile uint32_t*)(uintptr_t)0x48000010U // GPIO port input  data register
#define GPIOA_ODR   (volatile uint32_t*)(uintptr_t)0x48000014U // GPIO port output data register
#define GPIOA_OSPEEDR (volatile uint32_t*)(uintptr_t)0x48000008U // GPIO port output speed register
#define GPIOA_AFRH    (volatile uint32_t*)(uintptr_t)0x48000024U // GPIO alternate function high register
#define GPIOA_AFRL    (volatile uint32_t*)(uintptr_t)0x48000020U

#define GPIOB_MODER (volatile uint32_t*)(uintptr_t)0x48000400U
#define GPIOB_TYPER (volatile uint32_t*)(uintptr_t)0x48000404U
#define GPIOB_OSPEEDR (volatile uint32_t*)(uintptr_t)0x48000408U
#define GPIOB_PUPDR (volatile uint32_t*)(uintptr_t)0x4800040CU
#define GPIOB_IDR (volatile uint32_t*)(uintptr_t)0x48000410U
#define GPIOB_ODR (volatile uint32_t*)(uintptr_t)0x48000414U
#define GPIOB_BSRR (volatile uint32_t*)(uintptr_t)0x48000418U
#define GPIOB_AFRL (volatile uint32_t*)(uintptr_t)0x48000420U
#define GPIOB_AFRH (volatile uint32_t*)(uintptr_t)0x48000424U
#define GPIOB_BRR (volatile uint32_t*)(uintptr_t)0x48000428U

#define GPIOC_MODER (volatile uint32_t*)(uintptr_t)0x48000800U      // GPIO port mode register
#define GPIOC_TYPER (volatile uint32_t*)(uintptr_t)0x48000804U      // GPIO port output type register
#define GPIOC_ODR (volatile uint32_t*)(uintptr_t) 0x48000814U       // GPIO port output register
#define GPIOC_BSRR (volatile uint32_t*)(uintptr_t) 0x48000818U      // GPIO port bit set/reset register
#define GPIOC_IDR (volatile uint32_t*)(uintptr_t) 0x48000810U    
#define GPIOC_PUPDR (volatile uint32_t*)(uintptr_t) 0x4800080CU

//--------------
// GPIO macro
//--------------

#define GPIO_MODER_PORT_SET_MODE_OUTPUT(REG, BIT) (SET_BIT((REG), 2u * (BIT)))
#define GPIO_MODER_PORT_SET_MODE_INPUT(REG, BIT) (MODIFY_REG(REG, (0b11u << (2 * BIT)), (0b11u << (2 * BIT))))
#define GPIO_MODER_PORT_SET_MODE_ALT(REG, BIT) (MODIFY_REG(REG, (0b11u << (2 * BIT)), (0b10u << (2 * BIT))))
#define GPIO_TYPER_PORT_SET_PUSH_PULL(REG, BIT) (CLEAR_BIT((REG), (BIT)))
#define GPIO_TYPER_PORT_SET_OPEN_DRAIN(REG, BIT) (SET_BIT((REG), (BIT)))
#define GPIO_BSRR_BIT_SET(REG, BIT) (SET_BIT(REG, BIT))
#define GPIO_BSRR_BIT_RESET(REG, BIT) (SET_BIT(REG, (BIT + 16)))
#define GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT) (*(REG) = *(REG) & (~(0b11u << (2 * BIT))))
#define GPIO_PUPDR_PORT_PULL_UP_SET(REG, BIT) ({GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT); SET_BIT((REG), 2u * (BIT));})
#define GPIO_PUPDR_PORT_PULL_DOWN_SET(REG, BIT) ({GPIO_PUPDR_PORT_NO_PUPD_SET(REG, BIT); SET_BIT((REG), (2u  * (BIT) + 1u));})
/// pins 8...15
#define GPIO_AFRH_SET_ALT(REG, PIN, MODE) (MODIFY_REG(REG, (0b1111u << (4u * (PIN - 8u))), (MODE << (4u * (PIN - 8u)))))
/// pins 0...7
#define GPIO_AFRL_SET_ALT(REG, PIN, MODE) (MODIFY_REG(REG, (MODE << (4u * (PIN))), (MODE << (4u * (PIN)))))
#define GPIO_OSPEEDR_SET_FAST(REG, PIN) (MODIFY_REG(REG, (0b11u << (2u * (PIN))), (0b11u << (2u * (PIN)))))
#define GPIO_OSPEEDR_SET_MEDIUM(REG, PIN) (MODIFY_REG(REG, (0b11u << (2u * (PIN))), (0b01u << (2u * (PIN)))))
#define GPIO_OSPEEDR_SET_LOW(REG, PIN) (MODIFY_REG(REG, (0b11u << (2u * (PIN))), (0b00u << (2u * (PIN)))))
