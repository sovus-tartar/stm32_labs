#define SET_BIT(REG, BIT) (*REG |= 1u << BIT)    // выставление бита BIT в регистре REG в 1
#define CLEAR_BIT(REG, BIT) (*REG &= (~(1u << BIT)))   // выставление бита BIT в регистре REG в 0
#define READ_BIT(REG, BIT) ((*REG >> BIT) & 1u)    // возвращает бит BIT регистра REG
#define WRITE_REG(REG, VAL) (*REG = VAL)  // выставление регистра REG в VAL
#define READ_REG(REG) (*REG)        // возвращает регистр REG


#include <stdio.h>

void test()
{
    unsigned test = 0;

    for(unsigned i = 0; i < 32; ++i)
    {
        SET_BIT(&test, i);
        printf("0x%08x\n", test);
        printf("%u\n", READ_BIT(&test, i));
    }

    for(unsigned i = 32; i > 0; --i)
    {
        CLEAR_BIT(&test, (i - 1));
        printf("0x%08x\n", test);
        printf("%u\n", READ_BIT(&test, (i - 1)));
    }
}

int main()
{
    test();
}

