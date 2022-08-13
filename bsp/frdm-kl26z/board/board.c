/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 *
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"

#include "drv_uart.h"
#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"

/**
 * @addtogroup Kl26z
 */

#define VECTORNUM    (*(volatile uint32_t*)(0xE000ED04)) 
void DefaultISR(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
	rt_kprintf("****default_isr entered on vector %d  %x *****\r\n\n", (uint8_t)VECTORNUM, (uint32_t)VECTORNUM);
    /* leave interrupt */
    rt_interrupt_leave();
}

/*@{*/

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{

}

/*******************************************************************************
 * Function Name  : SysTick_Configuration
 * Description    : Configures the SysTick for OS tick.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void  SysTick_Configuration(void)
{
    SystemCoreClockUpdate();              /* Update Core Clock Frequency        */
    SysTick_Config(SystemCoreClock/RT_TICK_PER_SECOND); /* Generate interrupt each 1 ms       */
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * This function will initial Tower board.
 */
void rt_hw_board_init()
{
    /* NVIC Configuration */
    NVIC_Configuration();

    //use auto init 
    rt_components_board_init();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitBootPeripherals();

    /* Configure the SysTick */
    SysTick_Configuration();

    rt_hw_uart_init();

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    rt_console_set_device(CONSOLE_DEVICE);
#endif
}

/*@}*/
