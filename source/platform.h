#ifndef PLATFROM_H
#define PLATFORM_H

#include "base.h"

// STM Platfrom enumerators
#define PLAT_UNKNOWN (0UL)
#define PLAT_STM32F401RE (1UL)

#define PLATFORM PLAT_STM32F401RE


#if PLATFORM == PLAT_STM32F401RE

// Platfrom defines provided by the manufacturer
#include "stm32f4xx.h"
#include "stm32f401xe.h"

// Platfrom clock specs
#define MAX_HSI_CLK_FREQ (16000000UL)
#define MAX_HSE_CLK_FREQ (16000000UL)
#define MAX_CLK_FREQ (84000000UL)
#define MAX_AHB1_CLK_FREQ (42000000UL)
#define MAX_AHB2_CLK_FREQ (84000000UL)

#endif

#endif