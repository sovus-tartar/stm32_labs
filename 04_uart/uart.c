#include <stdint.h>
#include <stddef.h>
#include "../03_systick/led.h"
#include "GPIO.h"
#include "RCC.h"
#include "uart.h"

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    REG_RCC_AHBENR_PORT_C_ENABLE(REG_RCC_AHBENR);
    REG_RCC_AHBENR_PORT_A_ENABLE(REG_RCC_AHBENR);

    // Set alternate functions:
    GPIO_AFRH_SET_ALT(GPIOA_AFRH, 9);
    GPIO_AFRH_SET_ALT(GPIOA_AFRH, 10);

    // Configure pin operating speed:
    GPIO_OSPEEDR_SET_FAST(GPIOA_OSPEEDR, 9);
    GPIO_OSPEEDR_SET_FAST(GPIOA_OSPEEDR, 10);

    GPIO_MODER_PORT_SET_MODE_ALT(GPIOA_MODER, 9);
    GPIO_MODER_PORT_SET_MODE_ALT(GPIOA_MODER, 10);
}

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

    // (5) Wait for TX and to enable:
    WAIT_FOR(USART1_ISR, USART_ISR_WAIT_TX);
    WAIT_FOR(USART1_ISR, USART_ISR_WAIT_TX);
}

char uart_rcv_byte()
{
    WAIT_FOR(USART1_ISR, USART_ISR_WAIT_RCV);
    CLEAR_BIT(USART1_ISR, 5);

    return READ_REG(USART1_RDR);
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
}

//------
// Main
//------

#define UART_BAUDRATE 1200
#define UART_BAUDRATE_FIX (UART_BAUDRATE * 0.03)

int main()
{
    struct led_t led;
    led_init(&led, GPIOC_BSRR, 8, GPIOC_MODER, GPIOC_TYPER);
    board_clocking_init();

    board_gpio_init();

    uart_init(UART_BAUDRATE + UART_BAUDRATE_FIX, CPU_FREQENCY);
    char temp[64] = "No, you ";
    while (1)
    {
        for (int i = 8; i < 62; ++i)
        {
            temp[i] = uart_rcv_byte();

            if (temp[i] == '\r')
                break;
        }
        temp[62] = '\r';
        temp[63] = '\0';

        print_string(temp);
    }
}
