/*
 * app_pcap01.c
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 电容采集应用层实现
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "app_pcap01.h"

#include "algo_filter.h"
#include "pcap01_intn_hal.h"
#include "osif.h"
#include "pcap01.h"
#include "watchdog_hal.h"

void APP_PCAP01_Init(void)
{
    PCAP01_Init();
    PCAP01_INTN_HAL_Init();
}

bool APP_PCAP01_ReadCapacitance_pF_x1000(uint32_t *out_pF_x1000)
{
    uint32_t status;
    uint32_t ready = 0U;
    uint32_t ratioRaw;

    /* 1. 等待 INTN 中断或轮询 DRDY 位 */
    PCAP01_INTN_HAL_ClearEvent();
    for (uint32_t i = 0U; i < 200U; i++)
    {
        if (PCAP01_INTN_HAL_TakeEvent())
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
            return false;
        }
    }

    /* 2. 二次确认状态寄存器 */
    status = PCAP01_ReadData(8U);
    if ((status & 0x00100000UL) == 0U)
    {
        return false;
    }

    /* 3. 中值滤波：连续采集 5 个样本取中值 */
    {
        uint32_t samples[5];

        for (uint32_t i = 0U; i < 5U; i++)
        {
            samples[i] = PCAP01_ReadData(1U);
            WATCHDOG_HAL_Fed();
        }
        ratioRaw = ALGO_FILTER_Median5(samples);
    }

    /* 4. 原始值 → pF * 1000 */
    *out_pF_x1000 = PCAP01_RawToCapacitance_pF(ratioRaw);

    return true;
}
