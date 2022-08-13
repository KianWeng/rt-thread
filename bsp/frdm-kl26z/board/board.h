/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 *
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <MKL26Z4.h>


/* board configuration */

// <o> Internal SRAM memory size[Kbytes] <8-64>
// <i>Default: 64
#define KL26Z4_SRAM_SIZE         32
#define KL26Z4_SRAM_END          (0x1FFFE000 + (KL26Z4_SRAM_SIZE * 1024))

//#define RT_USING_UART1
#define RT_USING_UART0
//#define RT_USING_UART3

// <o> Console on USART: <0=> no console <1=>USART 1 <2=>USART 2
// <i>Default: 1
#define KL26Z4_CONSOLE_USART        0

void rt_hw_board_init(void);

#if KL26Z4_CONSOLE_USART == 0
#define CONSOLE_DEVICE "uart0"
#elif KL26Z4_CONSOLE_USART == 1
#define CONSOLE_DEVICE "uart1"
#elif KL26Z4_CONSOLE_USART == 2
#define CONSOLE_DEVICE "uart2"
#endif

#define FINSH_DEVICE_NAME   CONSOLE_DEVICE

#endif

