#include <stdint.h>
#include <stddef.h>
#include "../03_systick/led.h"
#include "GPIO.h"
#include "RCC.h"
#include "uart.h"


//--------------------
// GPIO configuration
//--------------------

void uart_init(size_t baudrate, size_t frequency)
{
    // (1) Configure USART1 clocking:
    REG_RCC_APB2ENR_USART1_ENABLE(REG_RCC_APB2ENR);
    REG_RCC_CFGR3_HSI14_CLK_ENABLE(REG_RCC_CFGR3);

    USART_CR1_SET_DATA_LENGTH_SET_8(USART1_CR1);
    USART_CR1_SET_OVERSAMPLLING_16(USART1_CR1);
    USART_CR1_PARITY_CTRL_DISABLE(USART1_CR1);
    USART_CR1_TRANSMITTER_ENABLE(USART1_CR1);
    USART_CR1_RECIEVER_ENABLE(USART1_CR1);
    USART_CR2_MSBFIRST_DISABLE(USART1_CR2);
    USART_CR2_2_STOP_BIT(USART1_CR2);

    // (3) Configure USART baud rate:
    uint32_t usartdiv = (frequency + baudrate / 2) / baudrate;

    WRITE_REG(USART1_BRR, usartdiv);

    // (4) Enable UART:
    USART_CR1_UART_ENABLE(USART1_CR1);

    // (5) Wait for TX and RX to enable:
    WAIT_FOR(USART1_ISR, USART_ISR_WAIT_TX);
    WAIT_FOR(USART1_ISR, USART_ISR_WAIT_RX);
}

char uart_rcv_byte()
{
    WAIT_FOR(USART1_ISR, USART_ISR_WAIT_RCV);
    CLEAR_BIT(USART1_ISR, 5);

    return READ_REG(USART1_RDR);
}

void uart_print_int(int val)
{
    while(val / 10)
    {
        uart_send_byte(val % 10 + '0');
        val = val / 10;
    }
    uart_send_byte('\t');
}

void uart_send_byte(char sym)
{
    // Wait for TXE:
    WAIT_FOR(USART1_ISR, USART_ISR_WAIT_SND);
    // Put data into DR:
    WRITE_REG(USART1_TDR, sym);
}

void print_string(const char *buf)
{
    for (size_t i = 0; buf[i] != '\0'; i++)
    {
        uart_send_byte(buf[i]);
    }
    uart_send_byte('\n');
}

//------
// Main
//------

#define UART_BAUDRATE 1200
#define UART_BAUDRATE_FIX 0