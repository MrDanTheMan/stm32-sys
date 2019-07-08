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
#define MAX_FLASH_CLK_FREQ (24000000UL)

// PLL Coeficients
// NOTE: These should be calculated at runtime depending on platform clock specs
// Output clock formula ((InputFreq / PLL_M) * PLL_N) / PLL_P
#define PLL_M  MAX_HSI_CLK_FREQ / 1000000UL
#define PLL_N  336U
#define PLL_P  1U //(DIV 4)
#define PLL_Q  8U

#endif

#endif