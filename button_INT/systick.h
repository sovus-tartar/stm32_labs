#include "./bit_arithmetic.h"

//-------------------
// SysTick registers
//-------------------

#define SYSTICK_CSR   (volatile uint32_t*)(uintptr_t)0xE000E010U // SysTick Control and Status Register
#define SYSTICK_RVR   (volatile uint32_t*)(uintptr_t)0xE000E014U // SysTick Reload Value Register
#define SYSTICK_CVR   (volatile uint32_t*)(uintptr_t)0xE000E018U // SysTick Current Value Register
#define SYSTICK_CALIB (volatile uint32_t*)(uintptr_t)0xE000E01CU // SysTick Calibration Value Register

//--------------
// Systick masks
//--------------
#define SYSTICK_RVR_RELOAD_MASK (0xFFFFFFu)
#define SYSTICK_CALIB_TENMS_MASK (0xFFFFFFu)
#define SYSTICK_CVR_CURRENT_MASK (0xFFFFFFu)
//--------------
// SysTick macro
//--------------


#define SYSTICK_RVR_RELOAD_TIME_MS(REG, VAL) (MODIFY_REG((REG), SYSTICK_RVR_RELOAD_MASK ,(((VAL) * ((CPU_FREQENCY) / 1000000U) / 8) - 1)))
#define SYSTICK_CSR_COUNTER_DISABLE(REG) (CLEAR_BIT((REG), 0))
#define SYSTICK_CSR_COUNTER_ENABLE(REG) (SET_BIT((REG), 0))
#define SYSTICK_CSR_TICKINT_ENABLE(REG) (SET_BIT((REG), 1))
#define SYSTICK_CSR_TICKINT_DISABLE(REG) (CLEAR_BIT((REG), 1))
#define SYSTICK_CSR_CLKSRC_REF_CLK(REG) (CLEAR_BIT((REG), 0))
#define SYSTICK_CSR_CLKSRC_CPU_CLK(REG) (SET_BIT((REG), 2))
#define SYSTICK_RVR_RELOAD_SET(REG, VAL) (MODIFY_REG((REG), SYSTICK_RVR_RELOAD_MASK, VAL))
#define SYSTICK_CVR_GET_VAL(REG) ((*(REG)) & SYSTICK_CVR_CURRENT_MASK);
#define SYSTICK_CVR_CLEAR_VAL(REG) (WRITE_REG((REG), 0u))
#define SYSTICK_CALIB_SET_TENMS(REG, VAL) (MODIFY_REG((REG), SYSTICK_CALIB_TENMS_MASK, VAL))
#define SYSTICK_CALIB_SKEW_EXACT(REG) (SET_BIT(REG, 30))
#define SYSTICK_CALIB_SKEW_UNEXACT(REG) (CLEAR_BIT(REG, 30))


void systick_init(uint32_t period_us)
{
    // (0) Read STM32F051 SysTick configuration:
    // Assumptions:
    // - There is a reference clock and it can be chosen as clock source.
    // - The SYST_CALIB SKEW bit is 1.
    // (1) Program the reload value:
    SYSTICK_RVR_RELOAD_TIME_MS(SYSTICK_RVR, period_us);
    

    // (2) Clear the current value:
    SYSTICK_CVR_CLEAR_VAL(SYSTICK_CVR);

    // (3) Program the CSR:
    // Watch out for the clock source!
    SYSTICK_CSR_COUNTER_ENABLE(SYSTICK_CSR);
    SYSTICK_CSR_TICKINT_ENABLE(SYSTICK_CSR);

}