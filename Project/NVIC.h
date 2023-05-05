//----------
// Registers
//----------

#define NVIC_ISER (volatile uint32_t*)(uintptr_t)0xE000E100
#define NVIC_ICER (volatile uint32_t*)(uintptr_t)0xE000E180
#define NVIC_ISPR (volatile uint32_t*)(uintptr_t)0xE000E200
#define NVIC_ICPR (volatile uint32_t*)(uintptr_t)0xE000E280
#define NVIC_IPR0 (volatile uint32_t*)(uintptr_t)0xE000E400
#define NVIC_IPR1 (volatile uint32_t*)(uintptr_t)0xE000E404
#define NVIC_IPR2 (volatile uint32_t*)(uintptr_t)0xE000E408
#define NVIC_IPR3 (volatile uint32_t*)(uintptr_t)0xE000E40C
#define NVIC_IPR4 (volatile uint32_t*)(uintptr_t)0xE000E410
#define NVIC_IPR5 (volatile uint32_t*)(uintptr_t)0xE000E414
#define NVIC_IPR6 (volatile uint32_t*)(uintptr_t)0xE000E418
#define NVIC_IPR7 (volatile uint32_t*)(uintptr_t)0xE000E41C