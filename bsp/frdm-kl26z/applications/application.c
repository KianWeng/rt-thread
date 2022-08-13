/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 *
 */

/**
 * @addtogroup k64
 */
/*@{*/

#include <stdint.h>
#include <stdio.h>

#include "MKL26Z4.h"
#include <board.h>
#include <rtthread.h>

#include "drv_led.h"
#include "drv_oled.h"
#include "drv_oled_font.h"

void rt_init_thread_entry(void* parameter)
{
#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif
}

ALIGN(RT_ALIGN_SIZE)
static char thread_led_stack[1024];
static char thread_oled_stack[1024];

struct rt_thread thread_led;
struct rt_thread thread_oled;

 static void screen_one(){
	
	// 欢迎使用
	oled_write_font(0x00, 0x00, China_1616[0], 1);
	oled_write_font(0x10, 0x00, China_1616[1], 1);
	oled_write_font(0x20, 0x00, China_1616[2], 1);
	oled_write_font(0x30, 0x00, China_1616[3], 1);
	
	// OLED驱动库
	oled_write_string(0x20, 0x02, "OLED", 4);
	oled_write_font(0x40, 0x02, China_1616[4], 1);
	oled_write_font(0x50, 0x02, China_1616[5], 1);
	oled_write_font(0x60, 0x02, China_1616[6], 1);
	
	// 作者：39度C
	oled_write_font(0x00, 0x06, China_1616[7], 1);
	oled_write_font(0x10, 0x06, China_1616[8], 1);
	oled_write_str(0x20, 0x06, ':');
	oled_write_string(0x30, 0x06, "39", 2);
	oled_write_font(0x40, 0x06, China_1616[9], 1);
	oled_write_str(0x50, 0x06, 'C');
	
}

static void screen_two(){
	
	// CSDN博主
	oled_write_string(0x20, 0x00, "CSDN", 4);
	oled_write_font(0x40, 0x00, China_1616[10], 1);
	oled_write_font(0x50, 0x00, China_1616[11], 1);
	
	// 欢迎关注
	oled_write_font(0x20, 0x02, China_1616[0], 1);
	oled_write_font(0x30, 0x02, China_1616[1], 1);
	oled_write_font(0x40, 0x02, China_1616[12], 1);
	oled_write_font(0x50, 0x02, China_1616[13], 1);
	
	// 谢谢你的支持
	oled_write_font(0x10, 0x06, China_1616[14], 1);
	oled_write_font(0x20, 0x06, China_1616[14], 1);
	oled_write_font(0x30, 0x06, China_1616[15], 1);
	oled_write_font(0x40, 0x06, China_1616[16], 1);
	oled_write_font(0x50, 0x06, China_1616[17], 1);
	oled_write_font(0x60, 0x06, China_1616[18], 1);
	
}

static void rt_thread_entry_led(void* parameter)
{
    int n = 0;
    rt_hw_led_init();

    while (1)
    {
        //rt_kprintf("LED\t%d\tis shining\r\n",n);

        rt_hw_led_on(n, GREEN);
        rt_thread_delay(RT_TICK_PER_SECOND/2);
        rt_hw_led_off(n, GREEN);
        rt_thread_delay(RT_TICK_PER_SECOND/2);

        n++;

        if (n == LED_MAX)
            n = 0;
    }
}

static void rt_thread_entry_oled(void* parameter)
{
    rt_hw_oled_init();

    while (1)
    {
        screen_one();
		rt_thread_mdelay(5000);
		oled_clear_screen();
		screen_two();
        rt_thread_mdelay(5000);
		oled_clear_screen();
    }
}

int rt_application_init()
{
    rt_thread_t init_thread;

    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    //------- init led thread
    rt_thread_init(&thread_led,
                   "led_demo",
                   rt_thread_entry_led,
                   RT_NULL,
                   &thread_led_stack[0],
                   sizeof(thread_led_stack),11,5);
    rt_thread_startup(&thread_led);

    //------- init oled thread
    rt_thread_init(&thread_oled,
                   "oled_demo",
                   rt_thread_entry_oled,
                   RT_NULL,
                   &thread_oled_stack[0],
                   sizeof(thread_oled_stack),11,5);
    rt_thread_startup(&thread_oled);

    return 0;
}

/*@}*/
