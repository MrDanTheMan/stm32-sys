// Startup code initializes MCU, its pretty low level and its  tightly coupled with the linker script
// Any changes in the linker script will have to be reflected here
// Any symbol prefixed with '__' is hard referenced in the linker script

#include "sys_stm32f4.h"

// Forward declarations
void __reset_handler(void);
void __def_handler(void);
void __sys_tick_handler(void);
void __hard_fault_handler(void);
void __usage_fault_handler(void);
void __mem_fault_handler(void);
void FlashToRAM(unsigned long *src, unsigned long *dest, unsigned long *end);
void ZeroMemory(unsigned long *from, unsigned long *to);
extern void main(void);

// Global variable used to look up current system clock speed and tick count
extern uint32_t SysClkFreq;
extern uint32_t SysClkMs;

// Memory addresses from the linker script
extern unsigned long __data_flash;
extern unsigned long __data_begin;
extern unsigned long __data_end;
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;
extern unsigned long __stack_end;

// This is the boot entry point
// The MCU entry point is linked to this method via the linker script
// This method is called as soon as the MCU board is reset
void __reset_handler(void)
{
    FlashToRAM(&__data_flash, &__data_begin, &__data_end);
    ZeroMemory(&__bss_start__, &__bss_end__);
    ConfigureSys();
    //ConfigureSysClockHSI();
    ConfigureSysClockPLL();
    ConfigureSysTick(SysClkFreq / 1000UL);
    main();
}

/// This is the sys tick interrupt handler
/// The frequency of sys tick interrupt is configured via ConfigureSysTick()
/// Lets us measure the time since boot
void __sys_tick_handler(void)
{
    SysClkMs++;
}

// Dummy handler used for all other interrupt handlers that we do not care at the moment
void __def_handler(void)
{
    while(1)
    {
    }
}

void __hard_fault_handler(void)
{
    while(1)
    {
    }
}

void __mem_fault_handler(void)
{
    while(1)
    {
    }
}

void __usage_fault_handler(void)
{
    while(1)
    {
    }
}

/// Copies data from FlashROM source to RAM destination
void FlashToRAM(unsigned long *src, unsigned long *dest, unsigned long *end)
{
    for(unsigned long *addr = dest; addr < end;)
    {
        *(addr++) = *(src++);
    }
}

/// Zeros out given memory block
void ZeroMemory(unsigned long *from, unsigned long *to)
{
    for(unsigned long *addr = from; addr < to; addr++)
    {
        *(addr) = 0;
    }
}

void _exit(int satus)
{

	while(1); // hang on endless loop
}

/// This is our interrupt vector map, it is part of ARM architecture requirement
/// Via this table we can bind handler method pointers to certain ARM events
/// For now we only care about the reset event and stack pointer and ignore the rest via a dummy def handler
/// This definition is tightly coupled with the linker script
__attribute__ ((section(".vec_table")))
void (* const interrupt_vector_table[])(void) =
{
    (void *) &__stack_end,      // Stack pointer
    __reset_handler,            // Reset interrupt handler
    __def_handler,              // NMI interrupt handler
    __hard_fault_handler,       // Hard fault interrupt handler
    __mem_fault_handler,        // Memory fault interrupt handler
    __def_handler,              // Bus fault interrupt handler
    __usage_fault_handler,      // Usage fault interrupt handler
    0,                          // -- Reserved by chipset
    0,                          // -- Reserved by chipset
    0,                          // -- Reserved by chipset
    0,                          // -- Reserved by chipset
    __def_handler,              // SV Call interrupt handler
    __def_handler,              // Debugging hadnler
    0,                          // -- Reserved by chipset
    __def_handler,              // Pend SV interrupt handler
    __sys_tick_handler,         // Sys tick interrupt handler
    __def_handler,              // Peripherals IRQ 0 handler
    __def_handler,              // Peripherals IRQ 1 handler
    __def_handler,              // Peripherals IRQ 2 handler
    __def_handler,              // Peripherals IRQ 3 handler
    __def_handler,              // Peripherals IRQ 4 handler
    __def_handler,              // Peripherals IRQ 5 handler
    __def_handler               // Peripherals IRQ 6 handler
};