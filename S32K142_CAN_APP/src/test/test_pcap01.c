/*
 * test_pcap01.c
 *
 *  Created on: 2026年6月29日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 测试文件 —— 完整采集链路：电容 → 液位 → 串口打印
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. 量产可删除本文件，不影响电容采集及液位转换功能
 */

#include "test_pcap01.h"

#include "algo_CapToLevel.h"
#include "app_pcap01.h"
#include "app_TankParameterConfig.h"
#include "osif.h"
#include "uart_hal.h"
#include "watchdog_hal.h"

void TEST_PCAP01_Init(void)
{
    APP_PCAP01_Init();
    UART_HAL_SendString("PCAP01 Init done. State=");
    UART_HAL_SendString(APP_PCAP01_GetStateName(APP_PCAP01_GetState()));
    UART_HAL_SendString("\r\n");
}

void TEST_PCAP01_PollAndPrintOnce(void)
{
    uint32_t cap_pF_x1000;
    uint32_t cap_pF_int;
    uint32_t cap_pF_frac;
    PCAP01_State_t state;
    uint8_t  convStatus;
    uint16_t volume_L;
    uint16_t height_mm;

    /* ---- 1. 电容采集 ---- */
    state = APP_PCAP01_ReadCapacitance_pF_x1000(&cap_pF_x1000);

    if (state != PCAP01_STATE_DATA_VALID)
    {
        UART_HAL_SendString("PCAP01 fail: ");
        UART_HAL_SendString(APP_PCAP01_GetStateName(state));
        UART_HAL_SendString("\r\n");
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
    UART_HAL_SendString("pF");

    /* ---- 2. 电容 → 液位 ---- */
    if (APP_TankParam_IsConfigured())
    {
        const LngCylinderProfile_t *pProfile = APP_TankParam_GetProfile();

        convStatus = ALGO_CapToLevel_Convert(pProfile, cap_pF_x1000, &volume_L, &height_mm);

        UART_HAL_SendString(" -> V:");
        UART_HAL_SendDec32(volume_L);
        UART_HAL_SendString("L H:");
        UART_HAL_SendDec32(height_mm);
        UART_HAL_SendString("mm");

        if (convStatus == 1U)
        {
            UART_HAL_SendString(" (clamped)");
        }
    }
    else
    {
        UART_HAL_SendString(" (no tank config)");
    }

    UART_HAL_SendString("\r\n");

    /* ---- 3. 延时 ---- */
    for (uint32_t i = 0U; i < 50U; i += 10U)
    {
        WATCHDOG_HAL_Fed();
        OSIF_TimeDelay(10U);
    }
}
