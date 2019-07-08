#ifndef CLOCK_H
#define CLOCK_H

#include "platform.h"

// At this point the platfrom should have specified clock specs
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

// Max frequency of flash
#ifndef MAX_FLASH_CLK_FREQ
#define MAX_FLASH_CLK_FREQ (24000000UL)
#endif


#define CLK_SRC_HSI (0UL)
#define CLK_SRC_HSE (1UL)
#define CLK_SRC_PLL (2UL)

extern uint32_t g_SysClkFreq;
extern uint32_t g_SysClkMs;

extern void CLKResetClocks(void);
extern void CLKSetFlash(const uint32_t waitStates);
extern void CLKSetPrescalers(const uint32_t ahb, const uint32_t apb1, const uint32_t apb2);
extern void CLKSetSysClockHSI(void);
extern void CLKSetSysClockHSE(void);
extern void CLKSetSysClockPLL(void);
extern void CLKInitSysClock(const uint32_t clkSrc);


#endif