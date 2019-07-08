
#ifndef BASE_H
#define BASE_H

#include <stdint.h>

#define NULL (0UL)
#define __REG volatile

#define __SET(reg, bit)     ((reg) |= (bit))
#define __CLEAR(reg, bit)   ((reg) &= ~(bit))
#define __READ(reg, bit)    ((reg) & (bit))
#define __MODIFY(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))



#endif