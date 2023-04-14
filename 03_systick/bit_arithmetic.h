#pragma once

//---------------
// Bit arithmetic 
//---------------
#define SET_BIT(REG, BIT) (*(REG) |= 1u << (BIT))                   // выставление бита BIT в регистре REG в 1
#define CLEAR_BIT(REG, BIT) (*(REG) &= (~(1u << (BIT))))            // выставление бита BIT в регистре REG в 0
#define READ_BIT(REG, BIT) ((*(REG) >> (BIT)) & 1u)                 // возвращает бит BIT регистра REG
#define WRITE_REG(REG, VAL) (*(REG) = (VAL))                        // выставление регистра REG в VAL
#define READ_REG(REG) (*(REG))                                    // возвращает регистр REG

#define MODIFY_REG(REG, MODIFYMASK, VALUE) (*(REG) = ((*(REG) & (~(MODIFYMASK))) | (VALUE & MODIFYMASK)))
#define WAIT_FOR(REG, MASK) ({ while ((READ_REG(REG) & (MASK)) != (MASK));})