#ifndef CLOCK_H
#define CLOCK_H

#include "platform.h"

// At this point the platfrom should have specified clokc specs
// If it did not then we set some small values here

// Max frequency of internal oscilator
#ifndef MAX_HSI_CLK_FREQ
#define MAX_HSI_CLK_FREQ 16000000UL
#endif

// Max frequency of external oscilator
#ifndef MAX_HSE_CLK_FREQ
#define MAX_HSE_CLK_FREQ 16000000UL
#endif

// Max CPU frequency 
#ifndef MAX_CLK_FREQ
#define MAX_CLK_FREQ 84000000UL
#endif

/// Max clock frequency on peripheral bus 1
#ifndef MAX_AHB1_CLK_FREQ
#define MAX_AHB2_CLK_FREQ (8000000UL)
#endif

/// Max clock frequency on peripheral bus 2
#ifndef MAX_AHB2_CLK_FREQ
#define MAX_AHB2_CLK_FREQ (8000000UL)
#endif


#define CLK_SRC_HSI (0UL)
#define CLK_SRC_HSE (1UL)
#define CLK_SRC_PLL (2UL)


#endif