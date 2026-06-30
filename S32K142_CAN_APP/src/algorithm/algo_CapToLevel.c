/*
 * algo_CapToLevel.c
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 电容值 → 液位转换算法实现
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "algo_CapToLevel.h"

uint8_t ALGO_CapToLevel_Convert(const LngCylinderProfile_t *pProfile,
                                uint32_t cap_pF_x1000,
                                uint16_t *out_volume_L,
                                uint16_t *out_height_mm)
{
    uint16_t cap_pF;

    if (pProfile == 0)
    {
        if (out_volume_L  != 0) { *out_volume_L  = 0U; }
        if (out_height_mm != 0) { *out_height_mm = 0U; }
        return 2U;
    }

    /* pF * 1000 → 整数 pF（四舍五入） */
    cap_pF = (uint16_t)((cap_pF_x1000 + 500U) / 1000U);

    /* 委托查表（内部已含钳位逻辑） */
    return Get_LngLevel_By_Profile(pProfile, cap_pF, out_volume_L, out_height_mm);
}
