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
