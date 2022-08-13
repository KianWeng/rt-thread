/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 *
 */
#include <MKL26Z4.h>
#include "drv_led.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

const rt_uint32_t led_mask[] = {1 << 21, 1 << 22, 1 << 26};

void rt_hw_led_init(void)
{
    rt_hw_led_off(LED1, GREEN);
    rt_hw_led_off(LED1, BLUE);
    rt_hw_led_off(LED1, RED);

    rt_hw_led_off(LED2, GREEN);
    rt_hw_led_off(LED2, BLUE);
    rt_hw_led_off(LED2, RED);

    rt_hw_led_off(LED3, GREEN);
    rt_hw_led_off(LED3, BLUE);
    rt_hw_led_off(LED3, RED);
}

void rt_hw_led_uninit(void)
{

}

void rt_hw_led_on(rt_uint32_t led_num, rt_uint16_t color)
{
    if(led_num == LED1 && color == GREEN){
        GPIO_WritePinOutput (BOARD_LED1_G_GPIO, BOARD_LED1_G_PIN, 1);
    }else if(led_num == LED1 && color == BLUE){
        GPIO_WritePinOutput (BOARD_LED1_B_GPIO, BOARD_LED1_B_PIN, 1);
    }else if(led_num == LED1 && color == RED){
        GPIO_WritePinOutput (BOARD_LED1_R_GPIO, BOARD_LED1_R_PIN, 1);
    }else if(led_num == LED2 && color == BLUE){
        GPIO_WritePinOutput (BOARD_LED2_B_GPIO, BOARD_LED2_B_PIN, 1);
    }else if(led_num == LED2 && color == GREEN){
        GPIO_WritePinOutput (BOARD_LED2_G_GPIO, BOARD_LED2_G_PIN, 1);
    }else if(led_num == LED2 && color == RED){
        GPIO_WritePinOutput (BOARD_LED2_R_GPIO, BOARD_LED2_R_PIN, 1);
    }else if(led_num == LED3 && color == BLUE){
        GPIO_WritePinOutput (BOARD_LED3_B_GPIO, BOARD_LED3_B_PIN, 1);
    }else if(led_num == LED3 && color == GREEN){
        GPIO_WritePinOutput (BOARD_LED3_G_GPIO, BOARD_LED3_G_PIN, 1);
    }else if(led_num == LED3 && color == RED){
        GPIO_WritePinOutput (BOARD_LED3_R_GPIO, BOARD_LED3_R_PIN, 1);
    }else{
        rt_kprintf("Not a valid led.\n");
    }

}

void rt_hw_led_off(rt_uint32_t led_num, rt_uint16_t color)
{
    if(led_num == LED1 && color == GREEN){
        GPIO_WritePinOutput (BOARD_LED1_G_GPIO, BOARD_LED1_G_PIN, 0);
    }else if(led_num == LED1 && color == BLUE){
        GPIO_WritePinOutput (BOARD_LED1_B_GPIO, BOARD_LED1_B_PIN, 0);
    }else if(led_num == LED1 && color == RED){
        GPIO_WritePinOutput (BOARD_LED1_R_GPIO, BOARD_LED1_R_PIN, 0);
    }else if(led_num == LED2 && color == BLUE){
        GPIO_WritePinOutput (BOARD_LED2_B_GPIO, BOARD_LED2_B_PIN, 0);
    }else if(led_num == LED2 && color == GREEN){
        GPIO_WritePinOutput (BOARD_LED2_G_GPIO, BOARD_LED2_G_PIN, 0);
    }else if(led_num == LED2 && color == RED){
        GPIO_WritePinOutput (BOARD_LED2_R_GPIO, BOARD_LED2_R_PIN, 0);
    }else if(led_num == LED3 && color == BLUE){
        GPIO_WritePinOutput (BOARD_LED3_B_GPIO, BOARD_LED3_B_PIN, 0);
    }else if(led_num == LED3 && color == GREEN){
        GPIO_WritePinOutput (BOARD_LED3_G_GPIO, BOARD_LED3_G_PIN, 0);
    }else if(led_num == LED3 && color == RED){
        GPIO_WritePinOutput (BOARD_LED3_R_GPIO, BOARD_LED3_R_PIN, 0);
    }else{
        rt_kprintf("Not a valid led.\n");
    }
}


