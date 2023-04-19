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
#define REG_RCC_CFGR3_HSI14_READY_CHECK (1U << 1)
//------------
// Constants
//------------
#define PLL_SRC (0b10U)
#define AHB_FREQ_48 0U
#define HSI_SRC (1U << 15)
#define CPU_FREQENCY 48000000U // CPU frequency: 48 MHz
#define ONE_MILLISECOND CPU_FREQENCY/1000U

//---------------
// RCC Registers
//---------------
 
#define REG_RCC_CR     (volatile uint32_t*)(uintptr_t)0x40021000U // Clock Control Register
#define REG_RCC_CFGR   (volatile uint32_t*)(uintptr_t)0x40021004U // PLL Configuration Register
#define REG_RCC_AHBENR (volatile uint32_t*)(uintptr_t)0x40021014U // AHB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR2  (volatile uint32_t*)(uintptr_t)0x4002102CU // Clock configuration register 2
#define REG_RCC_APB2ENR (volatile uint32_t*)(uintptr_t)0x40021018U // APB1 Peripheral Clock Enable Register
#define REG_RCC_CFGR3   (volatile uint32_t*)(uintptr_t)0x40021030U // Clock configuration register 2

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
#define REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_1(REG) (MODIFY_REG((REG), REG_RCC_CFGR_PCLK_PRESCALER_MASK, (0b000 << 8)))
#define REG_RCC_AHBENR_PORT_A_ENABLE(REG) (SET_BIT((REG), 17))
#define REG_RCC_AHBENR_PORT_B_ENABLE(REG) (SET_BIT((REG), 18))
#define REG_RCC_AHBENR_PORT_C_ENABLE(REG) (SET_BIT((REG), 19))
#define REG_RCC_AHBENR_PORT_D_ENABLE(REG) (SET_BIT((REG), 20))
#define REG_RCC_AHBENR_PORT_E_ENABLE(REG) (SET_BIT((REG), 21))
#define REG_RCC_AHBENR_PORT_F_ENABLE(REG) (SET_BIT((REG), 22))
#define REG_RCC_APB2ENR_USART1_ENABLE(REG) (SET_BIT(REG, 14))
#define REG_RCC_CFGR3_HSI14_CLK_ENABLE(REG) (SET_BIT(REG, 0))


void board_clocking_init()
{
    // (1) Clock HSE and wait for oscillations to setup.
    REG_RCC_CR_HSEON(REG_RCC_CR);
    WAIT_FOR(REG_RCC_CR, REG_RCC_HSEON_CHECK);
    // (2) Configure PLL:
    // PREDIV output: HSE/2 = 4 MHz
    REG_RCC_CFGR2_CONF_PLL(REG_RCC_CFGR2, 2U);

    // (3) Select PREDIV output as PLL input (4 MHz):
    REG_RCC_CFGR_PLL_SET_SRC(REG_RCC_CFGR, HSI_SRC);

    // (4) Set PLLMUL to 12:
    // SYSCLK frequency = 48 MHz
    REG_RCC_CFGR_PLLMUL_SET_MULT(REG_RCC_CFGR, 12);

    // (5) Enable PLL:
    REG_RCC_CR_PLL_ENABLE(REG_RCC_CR);
    WAIT_FOR(REG_RCC_CR, REG_RCC_CR_PLL_ENABLE_CHECK);
    // (6) Configure AHB frequency to 48 MHz:
    REG_RCC_CFGR_SET_AHB(REG_RCC_CFGR, AHB_FREQ_48);

    // (7) Select PLL as SYSCLK source:
    REG_RCC_CFGR_SET_SYSCLK_SRC(REG_RCC_CFGR, PLL_SRC);
    WAIT_FOR(REG_RCC_CFGR, REG_RCC_CFGR_SET_SYSCLK_SRC_CHECK);
    // (8) Set APB frequency to 48 MHz
    REG_RCC_CFGR_PCLK_PRESCALER_SET_DIV_1(REG_RCC_CFGR);
}