#include "gpio.h"
#include "stm32f401xe.h"

void GPIOEnable(const uint32_t port, const uint32_t pin, const uint32_t mode, GPIODesc *outGPIO)
{
    // Enable GPIO clock on particular bus
    RCC->AHB1ENR |= 1U << port;

    // Set GPIO mode on a particular pin
    GPIO_TypeDef* h = (GPIO_TypeDef*)(AHB1PERIPH_BASE + (GPIO_PORT_OFFSET * port));
    h->MODER &= ~(1U << ((pin * 2U)));
    switch(mode)
    {
        case GPIO_MODE_READ:
        h->MODER &= ~(1U << ((pin * 2U))); break;
        break;

        case GPIO_MODE_WRITE:
        h->MODER |= 1U << ((pin * 2U)); break;

        case GPIO_MODE_ALT:
        h->MODER |= 2U << ((pin * 2U)); break;

        case GPIO_MODE_ANALOG:
        h->MODER |= 3U << ((pin * 2U)); break;
    }

    // Return GPIO description
    outGPIO->m_port = port;
    outGPIO->m_mode = mode;
    outGPIO->m_pin = pin;
}

void GPIOSetAltFn(const GPIODesc *gpio, const uint32_t fn)
{
    GPIO_TypeDef* h = (GPIO_TypeDef*)(AHB1PERIPH_BASE + (GPIO_PORT_OFFSET * gpio->m_port));   
    const uint32_t offset = 4U;

    if(gpio->m_pin < 8)
    {
        // TODO: clear bits first !
        // Use AFR low register for the first 8 pins
        h->AFR[0] |= (fn << (offset * gpio->m_pin));
    }
    else 
    {
        // TODO: clear bits first !
        // Use AFR high register for the last 8 pins
        h->AFR[1] |= (fn << (offset * (gpio->m_pin - 8)));
    }
}

void GPIODisable(const GPIODesc *gpio)
{
    // TODO: Disabling GPIO bus clock here might not be a good idea
}

void GPIOWrite(const GPIODesc *gpio, const uint32_t val)
{
    GPIO_TypeDef* h = (GPIO_TypeDef*)(AHB1PERIPH_BASE + (GPIO_PORT_OFFSET * gpio->m_port));
    switch(val)
    {
        case GPIO_VAL_LOW:
        h->BSRR |= 1 << (16U + gpio->m_pin); break;
        
        case GPIO_VAL_HIGH:
        h->BSRR |= 1 << gpio->m_pin; break;
    }
}

void GPIORead(const GPIODesc *gpio, uint32_t *outVal)
{
    GPIO_TypeDef* h = (GPIO_TypeDef*)(AHB1PERIPH_BASE + (GPIO_PORT_OFFSET * gpio->m_port));
    *outVal = (uint32_t) (h->ODR >> gpio->m_pin ) & 1U;
}

void GPIOToggle(const GPIODesc *gpio)
{
    GPIO_TypeDef* h = (GPIO_TypeDef*)(AHB1PERIPH_BASE + (GPIO_PORT_OFFSET * gpio->m_port));
    const uint32_t val = (h->ODR >> gpio->m_pin ) & 1U;
    //const uint32_t val = h->ODR & (1 << gpio->m_pin);
    
    
    switch(val)
    {
        case GPIO_VAL_LOW:
        h->BSRR |= 1 << gpio->m_pin; break;
        
        case GPIO_VAL_HIGH:
        h->BSRR |= 1 << (16U + gpio->m_pin); break;
    }
}