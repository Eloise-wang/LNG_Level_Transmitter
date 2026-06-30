/*
 * algo_CapToLevel.c
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 电容值 → 液位转换算法实现（固定点线性插值 + 寄生电容扣除）
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "algo_CapToLevel.h"

/*******************************************************************************
 * Local Variables
 ******************************************************************************/

/* 寄生电容基线，初始值来自编译期宏，运行时可通过 CAN/BLE 校准修改 */
static uint32_t s_baseline_pF_x1000 = TANK_DEFAULT_PARASITIC_CAP_PF_X1000;

/*******************************************************************************
 * API
 ******************************************************************************/

void ALGO_CapToLevel_SetBaseline(uint32_t baseline_pF_x1000)
{
    s_baseline_pF_x1000 = baseline_pF_x1000;
}

uint32_t ALGO_CapToLevel_GetBaseline(void)
{
    return s_baseline_pF_x1000;
}

uint8_t ALGO_CapToLevel_Convert(const LngCylinderProfile_t *pProfile,
                                uint32_t cap_pF_x1000,
                                uint16_t *out_volume_L,
                                uint16_t *out_height_mm)
{
    uint16_t cap_pF_int;
    uint16_t cap_pF_frac;
    uint8_t  err_status = 0U;
    uint32_t effective;

    if ((pProfile == 0) || (pProfile->pTable == 0))
    {
        if (out_volume_L  != 0) { *out_volume_L  = 0U; }
        if (out_height_mm != 0) { *out_height_mm = 0U; }
        return 2U;
    }

    /* 0. 扣除寄生电容，防止下溢出 */
    if (cap_pF_x1000 > s_baseline_pF_x1000)
    {
        effective = cap_pF_x1000 - s_baseline_pF_x1000;
    }
    else
    {
        effective = 0U;
    }

    /* 1. pF * 1000 → 整数 pF + 小数部分（千分位） */
    cap_pF_int  = (uint16_t)(effective / 1000U);
    cap_pF_frac = (uint16_t)(effective % 1000U);

    /* 2. 钳位 */
    if (cap_pF_int > pProfile->MaxTableIndex_pF)
    {
        cap_pF_int  = pProfile->MaxTableIndex_pF;
        cap_pF_frac = 0U;
        err_status  = 1U;
    }

    /* 3. 表尾或无小数：直接取 table[N] */
    if ((cap_pF_frac == 0U) || (cap_pF_int >= pProfile->MaxTableIndex_pF))
    {
        if (out_volume_L  != 0) { *out_volume_L  = pProfile->pTable[cap_pF_int].Volume_L;  }
        if (out_height_mm != 0) { *out_height_mm = pProfile->pTable[cap_pF_int].Height_mm; }
        return err_status;
    }

    /* 4. 线性插值：table[N] + (table[N+1] - table[N]) * frac / 1000 */
    {
        const LngLevelData_t *pLow  = &pProfile->pTable[cap_pF_int];
        const LngLevelData_t *pHigh = &pProfile->pTable[cap_pF_int + 1U];

        uint16_t dVol = (uint16_t)(pHigh->Volume_L - pLow->Volume_L);
        uint16_t dHt  = (uint16_t)(pHigh->Height_mm - pLow->Height_mm);

        if (out_volume_L != 0)
        {
            *out_volume_L = pLow->Volume_L
                          + (uint16_t)(((uint32_t)dVol * cap_pF_frac + 500U) / 1000U);
        }
        if (out_height_mm != 0)
        {
            *out_height_mm = pLow->Height_mm
                           + (uint16_t)(((uint32_t)dHt * cap_pF_frac + 500U) / 1000U);
        }
    }

    return err_status;
}
