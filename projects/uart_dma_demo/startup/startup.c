#include "hpm_common.h"
#include "hpm_soc.h"
#include "hpm_l1c_drv.h"
#include <rtthread.h>

void system_init(void);

extern int entry(void);

extern void __libc_init_array(void);
extern void __libc_fini_array(void);

void system_init(void)
{
    disable_global_irq(CSR_MSTATUS_MIE_MASK);
    disable_irq_from_intc();
    enable_irq_from_intc();
    enable_global_irq(CSR_MSTATUS_MIE_MASK);
#ifndef CONFIG_NOT_ENABLE_ICACHE
    l1c_ic_enable();
#endif
#ifndef CONFIG_NOT_ENABLE_DCACHE
    l1c_dc_enable();
#endif
}

__attribute__((weak)) void c_startup(void)
{
    uint32_t i, size;
#ifdef FLASH_XIP
    extern uint8_t __vector_ram_start__[], __vector_ram_end__[], __vector_load_addr__[];
    size = __vector_ram_end__ - __vector_ram_start__;
    for (i = 0; i < size; i++) {
        *(__vector_ram_start__ + i) = *(__vector_load_addr__ + i);
    }
#endif

    extern uint8_t __etext[];
    extern uint8_t __bss_start__[], __bss_end__[];
    extern uint8_t __data_start__[], __data_end__[];
    extern uint8_t __noncacheable_bss_start__[], __noncacheable_bss_end__[];
    extern uint8_t __ramfunc_start__[], __ramfunc_end__[];
    extern uint8_t __noncacheable_init_start__[], __noncacheable_init_end__[];

    /* bss section */
    size = __bss_end__ - __bss_start__;
    for (i = 0; i < size; i++) {
        *(__bss_start__ + i) = 0;
    }

    /* noncacheable bss section */
    size = __noncacheable_bss_end__ - __noncacheable_bss_start__;
    for (i = 0; i < size; i++) {
        *(__noncacheable_bss_start__ + i) = 0;
    }

    /* data section LMA: etext */
    size = __data_end__ - __data_start__;
    for (i = 0; i < size; i++) {
        *(__data_start__ + i) = *(__etext + i);
    }

    /* ramfunc section LMA: etext + data length */
    size = __ramfunc_end__ - __ramfunc_start__;
    for (i = 0; i < size; i++) {
        *(__ramfunc_start__ + i) = *(__etext + (__data_end__ - __data_start__) + i);
    }

    /* noncacheable init section LMA: etext + data length + ramfunc legnth */
    size = __noncacheable_init_end__ - __noncacheable_init_start__;
    for (i = 0; i < size; i++) {
        *(__noncacheable_init_start__ + i) = *(__etext + (__data_end__ - __data_start__) + (__ramfunc_end__ - __ramfunc_start__) + i);
    }
}

__attribute__((weak)) int main(void)
{
    while(1);
}

void reset_handler(void)
{

    /*
     * Initialize LMA/VMA sections.
     * Relocation for any sections that need to be copied from LMA to VMA.
     */
    c_startup();

    /* Call platform specific hardware initialization */
    system_init();

    /* Do global constructors */
    __libc_init_array();



    /* Entry function */
    entry();
}


__attribute__((weak)) void _init()
{
}