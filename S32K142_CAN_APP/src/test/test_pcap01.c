/*
 * test_pcap01.c
 *
 *  Created on: 2026年6月29日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 测试文件 —— 调用 app 层采集电容，串口打印结果
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. 量产可删除本文件，不影响电容采集功能
 */

#include "test_pcap01.h"

#include "app_pcap01.h"
#include "osif.h"
#include "uart_hal.h"
#include "watchdog_hal.h"

void TEST_PCAP01_Init(void)
{
    APP_PCAP01_Init();
    UART_HAL_SendString("PCAP01 Init done.\r\n");
}

void TEST_PCAP01_PollAndPrintOnce(void)
{
    uint32_t cap_pF_x1000;
    uint32_t cap_pF_int;
    uint32_t cap_pF_frac;

    if (!APP_PCAP01_ReadCapacitance_pF_x1000(&cap_pF_x1000))
    {
        UART_HAL_SendString("PCAP01 read failed\r\n");
        return;
    }

    cap_pF_int  = cap_pF_x1000 / 1000U;
    cap_pF_frac = cap_pF_x1000 % 1000U;

    UART_HAL_SendString("C:");
    UART_HAL_SendDec32(cap_pF_int);
    UART_HAL_SendString(".");
    if (cap_pF_frac < 100U) { UART_HAL_SendString("0"); }
    if (cap_pF_frac < 10U)  { UART_HAL_SendString("0"); }
    UART_HAL_SendDec32(cap_pF_frac);
    UART_HAL_SendString("pF\r\n");

    for (uint32_t i = 0U; i < 50U; i += 10U)
    {
        WATCHDOG_HAL_Fed();
        OSIF_TimeDelay(10U);
    }
}
