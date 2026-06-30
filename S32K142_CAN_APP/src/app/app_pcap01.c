/*
 * app_pcap01.c
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 电容采集应用层实现（含状态机）
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "app_pcap01.h"

#include "algo_filter.h"
#include "osif.h"
#include "pcap01.h"
#include "pcap01_intn_hal.h"
#include "watchdog_hal.h"

/*******************************************************************************
 * Local Defines
 ******************************************************************************/

#define PCAP01_WAIT_TIMEOUT_MS    200U   /* INTN / DRDY 等待超时            */
#define PCAP01_DRDY_BIT           0x00100000UL  /* 状态寄存器 Bit20         */

/* 芯片故障判定阈值：连续 N 次 TIMEOUT 或 NOT_READY 后升级为 CHIP_FAULT */
#define PCAP01_FAULT_THRESHOLD    5U

/*******************************************************************************
 * Local Variables
 ******************************************************************************/

static PCAP01_State_t s_state = PCAP01_STATE_NOT_INIT;
static uint32_t      s_errorCount = 0U;

/*******************************************************************************
 * Static Helpers
 ******************************************************************************/

static void PCAP01_State_Set(PCAP01_State_t newState)
{
    s_state = newState;
}

static void PCAP01_Error_Record(PCAP01_State_t failState)
{
    s_errorCount++;

    if (s_errorCount >= PCAP01_FAULT_THRESHOLD)
    {
        s_state = PCAP01_STATE_CHIP_FAULT;
    }
    else
    {
        s_state = failState;
    }
}

static void PCAP01_Error_Reset(void)
{
    s_errorCount = 0U;
}

/*******************************************************************************
 * API
 ******************************************************************************/

void APP_PCAP01_Init(void)
{
    PCAP01_State_Set(PCAP01_STATE_NOT_INIT);

    PCAP01_Init();
    PCAP01_INTN_HAL_Init();

    PCAP01_Error_Reset();
    PCAP01_State_Set(PCAP01_STATE_INIT_DONE);
}

PCAP01_State_t APP_PCAP01_GetState(void)
{
    return s_state;
}

const char * APP_PCAP01_GetStateName(PCAP01_State_t state)
{
    switch (state)
    {
    case PCAP01_STATE_NOT_INIT:    return "NOT_INIT";
    case PCAP01_STATE_INIT_DONE:   return "INIT_DONE";
    case PCAP01_STATE_WAIT_INTN:   return "WAIT_INTN";
    case PCAP01_STATE_WAIT_DRDY:   return "WAIT_DRDY";
    case PCAP01_STATE_DATA_READY:  return "DATA_READY";
    case PCAP01_STATE_SAMPLING:    return "SAMPLING";
    case PCAP01_STATE_DATA_VALID:  return "DATA_VALID";
    case PCAP01_STATE_TIMEOUT:     return "TIMEOUT";
    case PCAP01_STATE_NOT_READY:   return "NOT_READY";
    case PCAP01_STATE_CHIP_FAULT:  return "CHIP_FAULT";
    default:                       return "UNKNOWN";
    }
}

PCAP01_State_t APP_PCAP01_ReadCapacitance_pF_x1000(uint32_t *out_pF_x1000)
{
    uint32_t status;
    uint32_t ready = 0U;
    uint32_t ratioRaw;

    /* ---- 阶段 1：等待 INTN 中断 ---- */
    PCAP01_State_Set(PCAP01_STATE_WAIT_INTN);
    PCAP01_INTN_HAL_ClearEvent();

    for (uint32_t i = 0U; i < PCAP01_WAIT_TIMEOUT_MS; i++)
    {
        if (PCAP01_INTN_HAL_TakeEvent())
        {
            ready = 1U;
            break;
        }
        WATCHDOG_HAL_Fed();
        OSIF_TimeDelay(1U);
    }

    /* ---- 阶段 2：INTN 超时，降级轮询 DRDY ---- */
    if (ready == 0U)
    {
        PCAP01_State_Set(PCAP01_STATE_WAIT_DRDY);

        for (uint32_t i = 0U; i < PCAP01_WAIT_TIMEOUT_MS; i++)
        {
            status = PCAP01_ReadData(8U);
            if ((status & PCAP01_DRDY_BIT) != 0U)
            {
                ready = 1U;
                break;
            }
            WATCHDOG_HAL_Fed();
            OSIF_TimeDelay(1U);
        }

        if (ready == 0U)
        {
            PCAP01_Error_Record(PCAP01_STATE_TIMEOUT);
            return s_state;
        }
    }

    /* ---- 阶段 3：二次确认状态寄存器 ---- */
    PCAP01_State_Set(PCAP01_STATE_DATA_READY);
    status = PCAP01_ReadData(8U);
    if ((status & PCAP01_DRDY_BIT) == 0U)
    {
        PCAP01_Error_Record(PCAP01_STATE_NOT_READY);
        return s_state;
    }

    /* ---- 阶段 4：多采样 + 中值滤波 ---- */
    PCAP01_State_Set(PCAP01_STATE_SAMPLING);
    {
        uint32_t samples[5];

        for (uint32_t i = 0U; i < 5U; i++)
        {
            samples[i] = PCAP01_ReadData(1U);
            WATCHDOG_HAL_Fed();
        }
        ratioRaw = ALGO_FILTER_Median5(samples);
    }

    /* ---- 阶段 5：格式转换 ---- */
    *out_pF_x1000 = PCAP01_RawToCapacitance_pF(ratioRaw);

    PCAP01_Error_Reset();
    PCAP01_State_Set(PCAP01_STATE_DATA_VALID);

    return s_state;
}
