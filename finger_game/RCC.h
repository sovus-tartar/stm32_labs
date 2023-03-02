#pragma once

#include "bit_arithmetic.h"
 
//-------
// Masks
//-------
#define REG_RCC_CFGR_PLL_SET_SRC_MASK (0b11U << 15)
#define REG_RCC_CFGR_PLLMUL_MASK (15U << 18)
#define REG_RCC_CFGR_AHB_MASK (0b1111U << 4)
#define REG_RCC_CFGR_SYSCLK_MASK (0b11U)
#define REG_RCC_CFGR_PCLK_PRESCALER_MASK (0b111U << 8)
#define REG_RCC_CFGR2_PLL_MASK (0b11U)
#define REG_RCC_HSEON_CHECK (1U << 17)
#define REG_RCC_CR_PLL_ENABLE_CHECK (1U << 25)
#define REG_RCC_CFGR_SET_SYSCLK_SRC_CHECK (1U << 3)
 
//------------
// Constants
//------------
#define PLL_SRC (0b10U)
#define AHB_FREQ_48 0U
#define HSI_SRC (1U << 15)
#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
 
//---------------
// RCC Registers
//---------------
 
#define REG_RCC_CR     (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR   (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2  (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2
 
//--------------
// RCC macro
//--------------
#define REG_RCC_CR_HSEON(REG) (SET_BIT((REG), 16U))
#define REG_RCC_CFGR2_CONF_PLL(REG, DIV) (MODIFY_REG((REG), REG_RCC_CFGR2_PLL_MASK, ((DIV) - 1))) //DIV - divider
#define REG_RCC_CFGR_PLL_SET_SRC(REG, SRC) (MODIFY_REG((REG), REG_RCC_CFGR_PLL_SET_SRC_MASK, (SRC)))
#define REG_RCC_CFGR_PLLMUL_SET_MULT(REG, MULT) (MODIFY_REG((REG), REG_RCC_CFGR_PLLMUL_MASK, ((MULT) - 2U) << 18))
#define REG_RCC_CR_PLL_ENABLE(REG) (SET_BIT((REG), 24))
#define REG_RCC_CFGR_SET_AHB(REG, FREQ) (MODIFY_REG((REG), REG_RCC_CFGR_AHB_MASK, (FREQ) << 4))
#define REG_RCC_CFGR_SET_SYSCLK_SRC(REG, SRC) (MODIFY_REG((REG), REG_RCC_CFGR_SYSCLK_MASK, (SRC)))
#define REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_2(REG) (MODIFY_REG((REG), REG_RCC_CFGR_PCLK_PRESCALER_MASK, (0b100 << 8)))
#define REG_RCC_AHBENR_PORT_A_ENABLE(REG) (SET_BIT((REG), 17))
#define REG_RCC_AHBENR_PORT_B_ENABLE(REG) (SET_BIT((REG), 18))
#define REG_RCC_AHBENR_PORT_C_ENABLE(REG) (SET_BIT((REG), 19))
#define REG_RCC_AHBENR_PORT_D_ENABLE(REG) (SET_BIT((REG), 20))
#define REG_RCC_AHBENR_PORT_E_ENABLE(REG) (SET_BIT((REG), 21))
#define REG_RCC_AHBENR_PORT_F_ENABLE(REG) (SET_BIT((REG), 22))