#ifndef SYS_STM32F4_H
#define SYS_STM32F4_H

#include "clock.h"

// Bus speed prescalers
// AHB - main system bus | APB1 low speed peripheral bus | APB2 high speed peripheral bus
#define AHB_SCALE 0U
#define APB1_SCALE 2U
#define APB2_SCALE 0U

extern void SYSConfigureTick(uint32_t interval);
extern void SYSWait(const uint32_t ms);

#endif