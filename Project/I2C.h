#pragma once

#include <stdint.h>

#define I2C_CR1 (volatile uint32_t*)(uintptr_t)0x40005400U
#define I2C_CR2 (volatile uint32_t*)(uintptr_t)0x40005404U
#define I2C_OAR1 (volatile uint32_t*)(uintptr_t)0x40005408U
#define I2C_OAR2 (volatile uint32_t*)(uintptr_t)0x4000540CU
#define I2C_TIMINGR (volatile uint32_t*)(uintptr_t)0x40005410U
#define I2C_TIMOUTR (volatile uint32_t*)(uintptr_t)0x40005414U
#define I2C_ISR (volatile uint32_t*)(uintptr_t)0x40005418U
#define I2C_ICR (volatile uint32_t*)(uintptr_t)0x4000541CU
#define I2C_PECR (volatile uint32_t*)(uintptr_t)0x40005420U
#define I2C_RXDR (volatile uint32_t*)(uintptr_t)0x40005424U
#define I2C_TXDR (volatile uint32_t*)(uintptr_t)0x40005428U


void I2C_START();
void I2C_STOP();
void I2C_WRITE();
void I2C_SEND_ADDRESS(unsigned char address, int mode, int size);
void I2C_READ(int Address, int size, unsigned char * buffer);
void I2C_Master_init();
