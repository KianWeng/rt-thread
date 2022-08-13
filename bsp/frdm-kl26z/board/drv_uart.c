/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-05-18     Bernard      The first version for LPC40xx
 * 2014-12-16     RT_learning  The first version for LPC5410x
 * 2017-08-01     XiaoYang     The first version for LPC546xx
 * 2018-11-30     yangjie      The first version for LPC54114
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "fsl_uart.h"
#include "fsl_common.h"
#include "fsl_lpsci.h"
#include "fsl_clock.h"

#ifdef BSP_USING_UART0

struct kl26z_uart_lpsci
{
    UART0_Type *UART;
    IRQn_Type UART_IRQn;
    char uart_num;
};

static rt_err_t kl26z_configure_lpsci(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct kl26z_uart_lpsci *uart;
    lpsci_config_t u0_config;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct kl26z_uart_lpsci *)serial->parent.user_data;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPSCI_GetDefaultConfig(&u0_config);

    u0_config.baudRate_Bps = cfg->baud_rate;
    u0_config.parityMode = kLPSCI_ParityDisabled,
    u0_config.stopBitCount = kLPSCI_OneStopBit,
    u0_config.idleLineType = kLPSCI_IdleLineStartBit,
    u0_config.enableTx = true;
    u0_config.enableRx = true;

    LPSCI_Init(uart->UART, &u0_config, CLOCK_GetPllFllSelClkFreq());

    /* Enable RX interrupt. */
    LPSCI_EnableInterrupts(uart->UART, kLPSCI_RxDataRegFullInterruptEnable);
    EnableIRQ(uart->UART_IRQn);
    // LPSCI_EnableTx(uart->UART, true);
    // LPSCI_EnableRx(uart->UART, true);

    return RT_EOK;
}

static rt_err_t kl26z_control_lpsci(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct kl26z_uart_lpsci *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct kl26z_uart_lpsci *)serial->parent.user_data;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        LPSCI_EnableInterrupts(uart->UART,kLPSCI_RxDataRegFullInterruptEnable);
        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        LPSCI_EnableInterrupts(uart->UART, kLPSCI_RxDataRegFullInterruptEnable);
        break;
    }

    return RT_EOK;
}

static int kl26z_putc_lpsci(struct rt_serial_device *serial, char c)
{
    struct kl26z_uart_lpsci *uart;

    uart = (struct kl26z_uart_lpsci *)serial->parent.user_data;

    while (!(kLPSCI_TxDataRegEmptyFlag & LPSCI_GetStatusFlags(uart->UART)));

    LPSCI_WriteByte(uart->UART, c);

    return 1;
}

static int kl26z_getc_lpsci(struct rt_serial_device *serial)
{
    struct kl26z_uart_lpsci *uart;

    uart = (struct kl26z_uart_lpsci *)serial->parent.user_data;
    if (kLPSCI_RxDataRegFullFlag & LPSCI_GetStatusFlags(uart->UART))
    {
        return LPSCI_ReadByte(uart->UART);
    }
    else
        return -1;
}

static const struct rt_uart_ops kl26z_uart_ops_lpsci =
{
    kl26z_configure_lpsci,
    kl26z_control_lpsci,
    kl26z_putc_lpsci,
    kl26z_getc_lpsci,
};
#endif

struct kl26z_uart
{
    UART_Type *UART;
    IRQn_Type UART_IRQn;
    char uart_num;
};

static rt_err_t kl26z_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct kl26z_uart *uart;
    uart_config_t u0_config;
    status_t result;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct kl26z_uart *)serial->parent.user_data;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&u0_config);

    // u0_config.baudRate_Bps = cfg->baud_rate;
    u0_config.baudRate_Bps = 9600;
    u0_config.parityMode = kUART_ParityDisabled,
    u0_config.stopBitCount = kUART_OneStopBit,
    u0_config.enableTx = true;
    u0_config.enableRx = true;

    if(uart->uart_num == 1){
        result = UART_Init(uart->UART, &u0_config, CLOCK_GetFreq(UART1_CLK_SRC));
        // result = UART_Init(uart->UART, &u0_config, 25000000);
        if(result != kStatus_Success) 
            return RT_ERROR;
    }
    else{
        result = UART_Init(uart->UART, &u0_config, CLOCK_GetFreq(UART2_CLK_SRC));
        if(result != kStatus_Success) 
            return RT_ERROR;
    }

    /* Enable RX interrupt. */
    UART_EnableInterrupts(uart->UART, kUART_RxDataRegFullInterruptEnable);
    EnableIRQ(uart->UART_IRQn);

    return RT_EOK;
}

static rt_err_t kl26z_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct kl26z_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct kl26z_uart *)serial->parent.user_data;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        UART_DisableInterrupts(uart->UART, kUART_RxDataRegFullInterruptEnable);
        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        UART_EnableInterrupts(uart->UART, kUART_RxDataRegFullInterruptEnable);
        break;
    }

    return RT_EOK;
}

static int kl26z_putc(struct rt_serial_device *serial, char c)
{
    struct kl26z_uart *uart;

    uart = (struct kl26z_uart *)serial->parent.user_data;

    while (!(kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(uart->UART)));

    UART_WriteByte(uart->UART, c);

    return 1;
}

static int kl26z_getc(struct rt_serial_device *serial)
{
    struct kl26z_uart *uart;

    uart = (struct kl26z_uart *)serial->parent.user_data;
    if (kUART_RxDataRegFullFlag & UART_GetStatusFlags(uart->UART))
    {
        return UART_ReadByte(uart->UART);
    }
    else
        return -1;
}

static const struct rt_uart_ops kl26z_uart_ops =
{
    kl26z_configure,
    kl26z_control,
    kl26z_putc,
    kl26z_getc,
};

/* UART device driver structure */
#ifdef BSP_USING_UART0
struct kl26z_uart_lpsci uart0 =
{
    UART0,
    UART0_IRQn,
    0,
};
struct serial_configure config0 = RT_SERIAL_CONFIG_DEFAULT;
struct rt_serial_device serial0;

void UART0_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_hw_serial_isr(&serial0, RT_SERIAL_EVENT_RX_IND);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif

#ifdef BSP_USING_UART1
struct kl26z_uart uart1 =
{
    UART1,
    UART1_IRQn,
    1,
};
struct serial_configure config1 = RT_SERIAL_CONFIG_DEFAULT;
struct rt_serial_device serial1;

void UART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_hw_serial_isr(&serial1, RT_SERIAL_EVENT_RX_IND);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif

#ifdef BSP_USING_UART2
struct kl26z_uart uart2 =
{
    UART2,
    UART2_IRQn,
    2,
};
struct serial_configure config2 = RT_SERIAL_CONFIG_DEFAULT;
struct rt_serial_device serial2;

void UART2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_hw_serial_isr(&serial2, RT_SERIAL_EVENT_RX_IND);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif

int rt_hw_uart_init(void)
{
    struct kl26z_uart_lpsci *tmp_lpsci;
    struct kl26z_uart *tmp;

#ifdef BSP_USING_UART0
    tmp_lpsci = &uart0;

    serial0.ops    = &kl26z_uart_ops_lpsci;
    serial0.config = config0;
    serial0.parent.user_data = tmp_lpsci;
	
    CLOCK_SetLpsci0Clock(1);

    /* register UART0 device */
    rt_hw_serial_register(&serial0, "uart0",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM,
                          tmp_lpsci);
#endif

#ifdef BSP_USING_UART1
    tmp = &uart1;
    serial1.ops    = &kl26z_uart_ops;
    serial1.config = config1;
    serial1.parent.user_data = tmp;

    /* register UART0 device */
    rt_hw_serial_register(&serial1, "uart1",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM,
                          tmp);
#endif

#ifdef BSP_USING_UART2
    tmp = &uart2;
    serial2.ops    = &kl26z_uart_ops;
    serial2.config = config2;
    serial2.parent.user_data = tmp;

    /* register UART0 device */
    rt_hw_serial_register(&serial2, "uart2",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM,
                          tmp);
#endif

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_uart_init);
