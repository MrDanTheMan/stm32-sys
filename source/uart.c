#include "uart.h"
#include <math.h>

USART_TypeDef * USARTHandle(const uint32_t channel)
{
    switch(channel)
    {
        case USART_CHANNEL1 : return USART1;
        case USART_CHANNEL2 : return USART2;
        case USART_CHANNEL3 : return USART3;
        case USART_CHANNEL4 : return USART4;
        case USART_CHANNEL5 : return USART5;
        case USART_CHANNEL6 : return USART6;
        case USART_CHANNEL7 : return USART7;
        case USART_CHANNEL8 : return USART8;
    }

    return NULL;
}

void USARTSetGPIO(const uint32_t channel, const GPIODesc *gpio)
{
    // NOTE: Might need the below
    //CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT6);
    //CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT8);

    //CLEAR_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR6_0);
    //SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR6_1);
    //CLEAR_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8_0);
    //SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8_1);

    //SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD6_0);
    //CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD6_1);
    //SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD8_0);
    //CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD8_1);

    // NOTE: Depending on the platform different alternative pin mode might be indicating USART mode
    // NOTE: For now we only support default platform -- STM32F401RE
    #if PLATFORM == PLAT_STM32F401RE
    switch (channel)
    {
        case USART_CHANNEL1: GPIOSetAltFn(gpio, GPIO_FN7); return;
        case USART_CHANNEL2: GPIOSetAltFn(gpio, GPIO_FN7); return;
        case USART_CHANNEL6: GPIOSetAltFn(gpio, GPIO_FN8); return;
    }
    #endif
}

void USARTClkEnable(const uint32_t channel)
{
    // NOTE: Depending on the platform usart controller might use different clock bus
    // NOTE: For now we only support default platfrom -- STM32F401RE
    #if PLATFORM == PLAT_STM32F401RE
    switch (channel)
    {
        case USART_CHANNEL1: SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN); return;
        case USART_CHANNEL2: SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN); return;
        case USART_CHANNEL6: SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART6EN); return;
    }

    #endif
}

void USARTClkDisable(const uint32_t channel)
{
    // NOTE: Depending on the platform usart controller might use different clock bus
    // NOTE: For now we only support default platfrom -- STM32F401RE
    #if PLATFORM == PLAT_STM32F401RE
    switch (channel)
    {
        case USART_CHANNEL1: CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN); return;
        case USART_CHANNEL2: CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN); return;
        case USART_CHANNEL6: CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_USART6EN); return;
    }

    #endif
}

void USARTSetRate(const uint32_t channel, const uint32_t baud)
{
    // USART Speed is dependant on peripheral bus clock speed
    // Different platforms have different bus speeds & different platform uses different buses for each USAR channels
    // For now we only support default platform -- STM32F401RE
    uint32_t busClk = 0;
    #if PLATFORM == PLAT_STM32F401RE
    switch(channel)
    {
        case USART_CHANNEL1: busClk = MAX_AHB1_CLK_FREQ; break;
        case USART_CHANNEL2: busClk = MAX_AHB1_CLK_FREQ; break;
        case USART_CHANNEL6: busClk = MAX_AHB2_CLK_FREQ; break;
    }
    #endif

    double rate = (float)(busClk / 16UL) / (float)baud;
    double mantissa = rate;
    double frac = modf(rate, &mantissa);

    USART_TypeDef *usart = USARTHandle(channel);
    MODIFY_REG(usart->BRR, USART_BRR_DIV_Fraction, (uint32_t)frac << USART_BRR_DIV_Fraction_Pos);
    MODIFY_REG(usart->BRR, USART_BRR_DIV_Mantissa, (uint32_t)mantissa << USART_BRR_DIV_Mantissa_Pos);
}

void USARTInit(const uint32_t channel, const GPIODesc *rx, const GPIODesc *tx)
{
    // Set GPIO usage to USART mode
    // We can use any IO pin to transmit serial inforamtion via IO multiplexing
    USARTSetGPIO(channel, rx);
    USARTSetGPIO(channel, tx);

    // Enable usart clock
    USARTClkEnable(channel);

    // Select the desired baud rate using the USART_BRR register.
    USARTSetRate(channel, 9600);

    // One Stop bit 
    USART_TypeDef *usart = USARTHandle(channel);
    CLEAR_BIT(usart->CR3, USART_CR2_STOP_0);
    CLEAR_BIT(usart->CR3, USART_CR2_STOP_1);

    // Word length is 8 bit
    CLEAR_BIT(usart->CR1, USART_CR1_M);

    // Disable hardware flow
    CLEAR_BIT(usart->CR2, USART_CR2_CPHA);
    CLEAR_BIT(usart->CR2, USART_CR2_LBCL);

    // Enable usart controllers
    MODIFY_REG(usart->CR1, USART_CR1_RE, 1 << USART_CR1_RE_Pos);
    MODIFY_REG(usart->CR1, USART_CR1_TE, 1 << USART_CR1_TE_Pos);
    MODIFY_REG(usart->CR1, USART_CR1_UE, 1 << USART_CR1_UE_Pos);
}

void USARTWrite(const uint32_t channel, const char* msg, const uint32_t len)
{
    USART_TypeDef *usart = USARTHandle(channel);
    for(uint32_t i=0; i < len; i++)
    {
        usart->DR = msg[i];
        while(!(usart->SR & (1 << 6)));
    }
}