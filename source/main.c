#include "gpio.h"
#include "uart.h"
#include "sys_stm32f4.h"

int main()
{
    // USART receiver and transmiter pins
    GPIODesc rx;
    GPIODesc tx;

    // Configure serial communication
    GPIOEnable(GPIO_PORTC, GPIO_PIN6, GPIO_MODE_ALT, &tx);
    GPIOEnable(GPIO_PORTC, GPIO_PIN8, GPIO_MODE_ALT, &rx);
    USARTInit(USART_CHANNEL6, &rx, &tx);
    USARTWrite(USART_CHANNEL6, "Serial communication enabled !\n", 31);

    // Configure onboard LED IO
    GPIODesc PINA5;
    GPIOEnable(0UL, GPIO_PIN5, GPIO_MODE_WRITE, &PINA5);

    while(1)
    {
        USARTWrite(USART_CHANNEL6, "Hello there partner !\n", 22);
        USARTWrite(USART_CHANNEL6, "--- --- ---\n", 12);

        GPIOWrite(&PINA5, GPIO_VAL_HIGH);
        SYSWait(50UL);
        GPIOWrite(&PINA5, GPIO_VAL_LOW);
        SYSWait(500UL);
        GPIOWrite(&PINA5, GPIO_VAL_HIGH);
        SYSWait(50UL);
        GPIOWrite(&PINA5, GPIO_VAL_LOW);
        SYSWait(500UL);
        GPIOWrite(&PINA5, GPIO_VAL_HIGH);
        SYSWait(50UL);
        GPIOWrite(&PINA5, GPIO_VAL_LOW);
        SYSWait(1500UL);
    }
    
    return 0;
}