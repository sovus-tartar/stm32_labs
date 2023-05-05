#pragma once
#include <stddef.h>
#include "bit_arithmetic.h"

//----------
// Registers
//----------
#define USART1_CR1 (volatile uint32_t*)(uintptr_t)0x40013800U // Control register 1
#define USART1_CR2 (volatile uint32_t*)(uintptr_t)0x40013804U // Control register 2
#define USART1_BRR (volatile uint32_t*)(uintptr_t)0x4001380CU // Baud rate register
#define USART1_ISR (volatile uint32_t*)(uintptr_t)0x4001381CU // Interrupt and status register
#define USART1_TDR (volatile uint32_t*)(uintptr_t)0x40013828U // Transmit data register
#define USART1_RDR (volatile uint32_t*)(uintptr_t)0x40013824U
#define USART1_CR3 (volatile uint32_t*)(uintptr_t)0x40013808U

//------
// Masks
//------

#define USART_CR2_STOP_BIT_MASK (0b11 << 12)
#define USART_ISR_WAIT_TX (1U << 21U)
#define USART_ISR_WAIT_RX (1U << 22U)
#define USART_ISR_WAIT_RCV (1U << 5U)
#define USART_ISR_WAIT_SND (1U << 7U)

//-------
// Macro
//-------

#define USART_CR1_SET_DATA_LENGTH_SET_8(REG) ({CLEAR_BIT(REG, 28), CLEAR_BIT(REG, 12);})
#define USART_CR1_SET_DATA_LENGTH_SET_7(REG) ({SET_BIT(REG, 28), CLEAR_BIT(REG, 12);})
#define USART_CR1_SET_DATA_LENGTH_SET_9(REG) ({CLEAR_BIT(REG, 28), SET_BIT(REG, 12);})
#define USART_CR1_SET_OVERSAMPLLING_16(REG) (CLEAR_BIT(REG, 15))
#define USART_CR1_SET_OVERSAMPLLING_8(REG) (SET_BIT(CLEAR_BIT, 15))
#define USART_CR1_PARITY_CTRL_ENABLE(REG) (SET_BIT(REG, 10))
#define USART_CR1_PARITY_CTRL_DISABLE(REG) (CLEAR_BIT(REG, 10))
#define USART_CR1_TRANSMITTER_ENABLE(REG) (SET_BIT(REG, 3))
#define USART_CR1_TRANSMITTER_DISABLE(REG) (CLEAR_BIT(REG, 3))
#define USART_CR1_RECIEVER_ENABLE(REG) (SET_BIT(REG, 2))
#define USART_CR1_RECIEVER_DISABLE(REG) (CLEAR_BIT(REG, 2))
#define USART_CR2_MSBFIRST_ENABLE(REG) (SET_BIT(REG, 19))
#define USART_CR2_MSBFIRST_DISABLE(REG) (CLEAR_BIT(REG, 19))
#define USART_CR2_1_STOP_BIT(REG) (MODIFY_REG(REG,USART_CR2_STOP_BIT_MASK, 0b00 << 12))
#define USART_CR2_0_5_STOP_BIT(REG) (MODIFY_REG(REG,USART_CR2_STOP_BIT_MASK, 0b01 << 12))
#define USART_CR2_2_STOP_BIT(REG) (MODIFY_REG(REG,USART_CR2_STOP_BIT_MASK, 0b10 << 12))
#define USART_CR2_1_5_STOP_BIT(REG) (MODIFY_REG(REG,USART_CR2_STOP_BIT_MASK, 0b11 << 12))
#define USART_CR1_UART_ENABLE(REG) (SET_BIT(REG, 0))
#define USART_CR1_UART_DISABLE(REG) (CLEAR_BIT(REG, 0))


void print_string(const char *buf);
void uart_send_byte(char sym);
void uart_print_int(int val);
char uart_rcv_byte();
void uart_init(size_t baudrate, size_t frequency);