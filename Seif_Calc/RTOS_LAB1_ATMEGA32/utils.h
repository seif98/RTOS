#ifndef UTILS_H
#define UTILS_H


#define SETBIT(reg, bit) (reg) |= (1<<bit)

#define CLRBIT(reg, bit) (reg) &= ~(1<<bit)

#define TOGGLEBIT(reg, bit) (reg) ^= (1<<bit)

#define GETBIT(reg, bit) (((reg)>>(bit)) & 0x01)

#define CONCBIT(B7,B6,B5,B4,B3,B2,B1,B0) CONCBIT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)
#define CONCBIT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)  (0b##B7##B6##B5##B4##B3##B2##B1##B0)

#endif