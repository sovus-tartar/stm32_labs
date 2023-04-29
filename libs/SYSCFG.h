#include "bit_arithmetic.h"



//------------------
// SYSCFGR registers
//------------------

#define SYSCFG_CFGR1    (volatile uint32_t*)(uintptr_t)0x40010000U
#define SYSCFG_CFGR2    (volatile uint32_t*)(uintptr_t)0x40010018U
#define SYSCFG_EXTICR1  (volatile uint32_t*)(uintptr_t)0x40010008U
#define SYSCFG_EXTICR2  (volatile uint32_t*)(uintptr_t)0x4001000CU
#define SYSCFG_EXTICR3  (volatile uint32_t*)(uintptr_t)0x40010010U
#define SYSCFG_EXTICR4  (volatile uint32_t*)(uintptr_t)0x40010014U


//---------------
// EXTI registers
//---------------

#define EXTI_IMR    (volatile uint32_t*)(uintptr_t)0x40010400U
#define EXTI_EMR    (volatile uint32_t*)(uintptr_t)0x40010404U
#define EXTI_RTSR   (volatile uint32_t*)(uintptr_t)0x40010408U
#define EXTI_FTSR   (volatile uint32_t*)(uintptr_t)0x4001040CU
#define EXTI_SWIER  (volatile uint32_t*)(uintptr_t)0x40010410U
#define EXTI_PR     (volatile uint32_t*)(uintptr_t)0x40010414U

//------
// Macro
//------

#define EXTI_IMR_MASK_IRQ(REG, LINE) (CLEAR_BIT(REG, LINE))
#define EXTI_IMR_UNMASK_IRQ(REG, LINE) (SET_BIT(REG, LINE))
#define EXTI_IER_MASK_IRQ(REG, LINE) (CLEAR_BIT(REG, LINE))
#define EXTI_IER_UNMASK_IRQ(REG, LINE) (SET_BIT(REG, LINE))
#define EXTI_RTSR_ENABLE(REG, BIT) (SET_BIT(REG, BIT))
#define EXTI_FTSR_ENABLE(REG, BIT) (SET_BIT(REG, BIT))
#define SYSCFGR_EXTICR1_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * PIN)), (0b0000u << (4 * PIN))))
#define SYSCFGR_EXTICR2_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * (PIN - 4))), ((LETTER - 'A') << (4 * (PIN - 4)))))
#define SYSCFGR_EXTICR3_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * (PIN - 8))), ((LETTER - 'A') << (4 * (PIN - 8)))))
#define SYSCFGR_EXTICR4_ENABLE(REG, PIN, LETTER) (MODIFY_REG(REG, (0b1111 << (4 * (PIN - 12))), ((LETTER - 'A') << (4 * (PIN - 12)))))
