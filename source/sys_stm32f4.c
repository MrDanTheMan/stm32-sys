#include "sys_stm32f4.h"

/// Configures the system tick interupt event to be fired every given ticks
/// Make sure the system clock is setup before calling this
void SYSConfigureTick(uint32_t interval)
{
    if (interval - 1UL > SysTick_LOAD_RELOAD_Msk)
    {
        return;
    }

    SysTick->LOAD  = interval - 1UL;
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    SysTick->VAL   = 0UL;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SYSWait(const uint32_t ms)
{
    uint32_t st = g_SysClkMs;
    while(g_SysClkMs - st < ms)
    {
        // wait
    }
}