/*
 * test_pcap01.c
 *
 *  Created on: 2026年6月29日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 【 】
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "test_pcap01.h"

#include "app_pcap01_INTN.h"
#include "osif.h"
#include "pcap01.h"
#include "uart_hal.h"
#include "watchdog_hal.h"

void TEST_PCAP01_Init(void)
{
    PCAP01_Init();
    UART_HAL_SendString("PCAP01 Init done.\r\n");
    APP_PCAP01_INTN_Init();
}

void TEST_PCAP01_PollAndPrintOnce(void)
{
    uint32_t status = 0U;
    uint32_t ratioRaw;
    uint32_t ratio_x10000;
    uint32_t cap_pF_x1000;
    uint32_t cap_pF_int;
    uint32_t cap_pF_frac;

    uint32_t ready = 0U;

    APP_PCAP01_INTN_ClearEvent();
    for (uint32_t i = 0U; i < 200U; i++)
    {
        if (APP_PCAP01_INTN_TakeEvent())
        {
            ready = 1U;
            break;
        }
        WATCHDOG_HAL_Fed();
        OSIF_TimeDelay(1U);
    }

    if (ready == 0U)
    {
        for (uint32_t i = 0U; i < 200U; i++)
        {
            status = PCAP01_ReadData(8U);
            if ((status & 0x00100000UL) != 0U)
            {
                ready = 1U;
                break;
            }
            WATCHDOG_HAL_Fed();
            OSIF_TimeDelay(1U);
        }

        if (ready == 0U)
        {
            UART_HAL_SendString("S:");
            UART_HAL_SendHex32(status);
            UART_HAL_SendString(" NRDY\r\n");
            return;
        }
    }

    status = PCAP01_ReadData(8U);
    if ((status & 0x00100000UL) == 0U)
    {
        return;
    }

    ratioRaw = PCAP01_ReadData(1U);
    for (uint32_t i = 0U; i < 5U; i++)
    {
        uint32_t raw2 = PCAP01_ReadData(1U);
        if (raw2 == ratioRaw)
        {
            break;
        }
        ratioRaw = raw2;
        WATCHDOG_HAL_Fed();
        OSIF_TimeDelay(1U);
    }

    cap_pF_x1000 = PCAP01_RawToCapacitance_pF(ratioRaw);
    cap_pF_int = cap_pF_x1000 / 1000U;
    cap_pF_frac = cap_pF_x1000 % 1000U;

    ratio_x10000 = (uint32_t)(((uint64_t)ratioRaw * 10000ULL + (1ULL << 20U)) >> 21U);

    WATCHDOG_HAL_Fed();

    UART_HAL_SendString("S:");
    UART_HAL_SendHex32(status);
    UART_HAL_SendString(" R:");
    UART_HAL_SendHex32(ratioRaw);
    UART_HAL_SendString(" Kx10000:");
    UART_HAL_SendDec32(ratio_x10000);
    UART_HAL_SendString(" C:");
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


