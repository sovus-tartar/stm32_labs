#pragma once

#include "bit_arithmetic.h"

#define USART1_CR1 (volatile uint32_t*)(uintptr_t)0x40013800U // Control register 1
#define USART1_CR2 (volatile uint32_t*)(uintptr_t)0x40013804U // Control register 2
#define USART1_BRR (volatile uint32_t*)(uintptr_t)0x4001380CU // Baud rate register
#define USART1_ISR (volatile uint32_t*)(uintptr_t)0x4001381CU // Interrupt and status register
#define USART1_TDR (volatile uint32_t*)(uintptr_t)0x40013828U // Transmit data register
#define USART1_RDR (volatile uint32_t*)(uintptr_t)0x40013824U
#define USART1_CR3 (volatile uint32_t*)(uintptr_t)0x40013808U