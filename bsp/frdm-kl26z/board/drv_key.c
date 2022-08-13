#include <rtthread.h>
#include <MKL26Z4.h>

#include "fsl_gpio.h"
#include "drv_key.h"

void handle_key_event(rt_uint8_t key){

    switch(key){
        case SW_RIGHT:
            rt_kprintf("turn right.\n");
            break;
        case SW_LEFT:
            rt_kprintf("turn left.\n");
            break;
        case SW_UP:
            rt_kprintf("go forward.\n");
            break;
        case SW_DOWN:
            rt_kprintf("go back.\n");
            break;
        default:
            break;
    }

}

void PORTA_IRQHandler(void)
{
    rt_uint32_t val;
    rt_uint8_t key;

    rt_interrupt_enter();
    val = GPIO_GetPinsInterruptFlags(GPIOA);
    if(val & SW_C_MASK){
        key = SW_C;
    }
    else if(val & SW_A_MASK){
         key = SW_A;
    }
    else if(val & SW_B_MASK){
         key = SW_B;
    }
    else if(val & SW_D_MASK){
         key = SW_D;
    }
    rt_kprintf("key is %u.\n",key);
    handle_key_event(key);
    GPIO_ClearPinsInterruptFlags(GPIOA, val);
    rt_interrupt_leave();
}

void PORTC_PORTD_IRQHandler(void)
{
    rt_uint32_t val;
    rt_uint8_t key;
    rt_interrupt_enter();
    val = GPIO_GetPinsInterruptFlags(GPIOC);
    if(val & SW_RIGHT_MASK){
        key = SW_RIGHT;
    }
    else if(val & SW_DOWN_MASK){
         key = SW_DOWN;
    }
    else if(val & SW_LEFT_MASK){
         key = SW_LEFT;
    }
    else if(val & SW_UP_MASK){
         key = SW_UP;
    }
    rt_kprintf("key is %u.\n",key);
    handle_key_event(key);
    GPIO_ClearPinsInterruptFlags(GPIOC, val);
    rt_interrupt_leave();
}
