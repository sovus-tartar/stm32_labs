#include <stdint.h>
#include <stddef.h>
#include "../03_systick/led.h"
#include "GPIO.h"
#include "RCC.h"


void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    *REG_RCC_CR = 0x00010000U;
    while ((*REG_RCC_CR & 0x00020000U) != 0x00020000U);

    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    *REG_RCC_CFGR2 |= 1U;

    // (3) Select PREDIV output as PLL input (4 MHz):
    *REG_RCC_CFGR |= 0x00010000U;

    // (4) Set PLLMUL to 12:
    // SYSCLK frequency = 48 MHz
    *REG_RCC_CFGR |= (12U-2U) << 18U;

    // (5) Enable PLL:
    *REG_RCC_CR |= 0x01000000U;
    while ((*REG_RCC_CR & 0x02000000U) != 0x02000000U);

    // (6) Configure AHB frequency to 48 MHz:
    *REG_RCC_CFGR |= 0b000U << 4U;

    // (7) Select PLL as SYSCLK source:
    *REG_RCC_CFGR |= 0b10U;
    while ((*REG_RCC_CFGR & 0xCU) != 0x8U);

    // (8) Set APB frequency to 48 MHz
    *REG_RCC_CFGR |= 0b000U << 8U;
}

//--------------------
// GPIO configuration
//--------------------

void board_gpio_init()
{
    // (1) Configure PC8 as LED
    *REG_RCC_AHBENR |= (1U << 19U);

    // Configure PC8 mode:
    *GPIOC_MODER |= (0b01U << (2U * 8U));

    // Configure PC8 type:
    *GPIOC_TYPER |= (0U << 8U);

    // (2) Configure pins PA9 and PA10 for UART
    *REG_RCC_AHBENR |= (1U << 17U);

    // Set alternate functions:
    *GPIOA_AFRH |= (1U << 4U * ( 9U - 8U));
    *GPIOA_AFRH |= (1U << 4U * (10U - 8U));

    // Configure pin operating speed:
    *GPIOA_OSPEEDR |= (0b11U << (2U *  9U));
    *GPIOA_OSPEEDR |= (0b11U << (2U * 10U));

    // Configure mode register:
    *GPIOA_MODER |= (0b10U << (2U *  9U));
    *GPIOA_MODER |= (0b10U << (2U * 10U));
}

//--------------------
// GPIO configuration
//--------------------

void uart_init(size_t baudrate, size_t frequency)
{
    // (1) Configure USART1 clocking:
    *REG_RCC_APB2ENR |= (1U << 14U);
    *REG_RCC_CFGR3   |= 0b00U;

    // (2) Set USART1 parameters:
    uint32_t reg_usart_cr1 = 0U;
    uint32_t reg_usart_cr2 = 0U;


    reg_usart_cr1 |= 0x00000000U;  // Data length: 8 bits
    reg_usart_cr1 |=  (0U << 15U); // Use oversampling by 16
    reg_usart_cr1 &= ~(1U << 10U); // Parity control: disabled
    reg_usart_cr1 |=  (1U <<  3U); // Transmitter: enabled
    reg_usart_cr1 |= (1U << 2U); // Reciever: enabled

    reg_usart_cr2 |= (0U << 19U);    // Endianness: LSB first
    reg_usart_cr2 |= (0b10U << 12U); // Number of stop bits: 2 stop bit


    *USART1_CR1 = reg_usart_cr1;
    *USART1_CR2 = reg_usart_cr2;

    

    // (3) Configure USART baud rate:
    uint32_t usartdiv = (frequency + baudrate/2)/baudrate;

    *USART1_BRR = usartdiv;

    // (4) Enable UART:
    *USART1_CR1 |= (1U << 0U);


    // (5) Wait for TX to enable:
    while ((*USART1_ISR & (1U << 21U)) == 0U);
}

char uart_rcv_byte()
{
    while ((*USART1_ISR & (1U <<  5U)) != (1U << 5U));
    CLEAR_BIT(USART1_ISR, 5);

    return *USART1_RDR;

}

void uart_send_byte(char sym)
{
    // Wait for TXE:
    while ((*USART1_ISR & (1U <<  7U)) == 0U);

    // Put data into DR:
    *USART1_TDR = sym;
}

void print_string(const char* buf)
{
    for (size_t i = 0; buf[i] != '\0'; i++)
    {
        uart_send_byte(buf[i]);
    }
}

//------
// Main
//------

#define UART_BAUDRATE 115200
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
        for(int i = 8; i < 64; ++i)
        {
            temp[i] = uart_rcv_byte();
            
            if(temp[i] == '\r')
                break;
        }
        temp[63] = '\n';
        
        for(int i = 0; i < 64; ++i)
        {
            uart_send_byte(temp[i]);
            if(temp[i] == '\r')
                break;
        }
    }

    
}
