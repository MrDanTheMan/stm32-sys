#include "sys_stm32f4.h"
#include "stm32f401xe.h"

volatile uint32_t SysClkFreq = MAX_HSI_CLK_FREQ;
volatile uint32_t SysClkMs = 0U;

/// Resets the system to default initial values
void ConfigureSys()
{
    // Reset clock configuration
    RCC->CFGR = 0x0;
    RCC->PLLCFGR = 0x24003010;
    RCC->CR |= 0 << RCC_CR_HSEON_Pos;
    RCC->CR |= 0 << RCC_CR_PLLON_Pos;
    RCC->CR |= 0 << RCC_CR_CSSON_Pos;

    // Reset flash configuration
    FLASH->ACR = 0x0;
    FLASH->ACR |= FLASH_ACR_LATENCY_0WS;
}

/// Configures the system tick interupt event to be fired every given ticks
/// Make sure the system clock is setup before calling this
void ConfigureSysTick(uint32_t interval)
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

/// Configures the systhem clock to max 84MHz speed
/// Uses PLL as a source clock
void ConfigureSysClockPLL(void)
{
    // Update flash latency before increasing clock frequency
    // Flash natively run at 24mhz 
    // Since we want to clock the system to max 84Mhz we need to insert 3 wait states to be in sync
    uint32_t flash = 0x0;
    flash |= FLASH_ACR_LATENCY_3WS;
    flash |= FLASH_ACR_PRFTEN;
    flash |= FLASH_ACR_ICEN;
    flash |= FLASH_ACR_DCEN;
    
    FLASH->ACR = flash;
//
    /// Setup sys clock and peripheral clocks perscalers
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, 0 << RCC_CFGR_HPRE_Pos);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, 4 << RCC_CFGR_PPRE1_Pos);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, 0 << RCC_CFGR_PPRE2_Pos);

    // Reset pll configuration
    uint32_t pllcfgr =  0x24003010;

    // Configure PLL clock system
    MODIFY_REG(pllcfgr, RCC_PLLCFGR_PLLM_Msk, PLL_M << RCC_PLLCFGR_PLLM_Pos);
    MODIFY_REG(pllcfgr, RCC_PLLCFGR_PLLN_Msk, PLL_N << RCC_PLLCFGR_PLLN_Pos);
    MODIFY_REG(pllcfgr, RCC_PLLCFGR_PLLP_Msk, PLL_P << RCC_PLLCFGR_PLLP_Pos);
    MODIFY_REG(pllcfgr, RCC_PLLCFGR_PLLQ_Msk, PLL_Q << RCC_PLLCFGR_PLLQ_Pos);

    RCC->PLLCFGR = pllcfgr;

    // Switch PLL clock on and wait for lock
    RCC->CR |= RCC_CR_PLLON;
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
        // Wait
    } 

    // Set the PLL as the main sys clock source & wait to make sure its ready
    RCC->CFGR |= 2 << RCC_CFGR_SW_Pos;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    {
    }

    // Update global syc clock frequency
    SysClkFreq = MAX_CLK_FREQ;
}

/// Configures the system clock
/// Uses internal oscilator as a clock source (16MHz)
void ConfigureSysClockHSI(void)
{
    // NucleoF401 board does not have external clock so we use internal 
    // oscilator (HSI) as a main clock source
    RCC->CR |= RCC_CR_HSION;
    while((RCC->CR & RCC_CR_HSIRDY) == 0)
    {
        // Make sure the clock is ready
        // Wait if not
    }

    // Assign HSI clock as a main sys clock
    // Make sure the sys clock is ready
    RCC->CFGR |= RCC_CFGR_SW_HSI;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    {
        // Wait
    }

    // Update flash latency before increasing clock frequency
    // Flash natively run at 24mhz 
    FLASH->ACR |= FLASH_ACR_PRFTEN;
    FLASH->ACR |= FLASH_ACR_LATENCY_0WS;
    FLASH->ACR |= FLASH_ACR_ICEN;
    FLASH->ACR |= FLASH_ACR_DCEN;

    /// Setup sys clock and peripheral clocks pre scalers
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, 0 << RCC_CFGR_HPRE_Pos);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, 0 << RCC_CFGR_PPRE1_Pos);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, 0 << RCC_CFGR_PPRE2_Pos);

    SysClkFreq = MAX_HSI_CLK_FREQ;
}