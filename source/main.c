#include "gpio.h"
#include "uart.h"



void Wait(uint32_t ms);

extern uint32_t SysClkMs;
extern uint32_t SysClkFreq;


int main()
{
    // USART receiver and transmiter pins
    GPIODesc rx;
    GPIODesc tx;

    // Enable IO Pins and set to USART alternative function
    GPIOEnable(GPIO_PORTC, GPIO_PIN6, GPIO_MODE_ALT, &tx);
    GPIOEnable(GPIO_PORTC, GPIO_PIN8, GPIO_MODE_ALT, &rx);
    USARTInit(USART_CHANNEL6, &rx, &tx);

    GPIODesc PINA5;
    GPIOEnable(0UL, GPIO_PIN5, GPIO_MODE_WRITE, &PINA5);

    while(1)
    {
        USARTWrite(USART_CHANNEL6, "Hello there Laura !\n", 19);
        USARTWrite(USART_CHANNEL6, "You is a lovely gurly gurl!\n", 28);
        USARTWrite(USART_CHANNEL6, "\n\n", 2);

        GPIOToggle(&PINA5);
        Wait(80UL);
        GPIOToggle(&PINA5);
        Wait(100UL);
        GPIOToggle(&PINA5);
        Wait(80UL);
        GPIOToggle(&PINA5);
        Wait(500UL);
    }
    
    return 0;
}

void Wait(uint32_t ms)
{
    uint32_t st = SysClkMs;

    while(SysClkMs - st < ms)
    {
        // wait
    }
}