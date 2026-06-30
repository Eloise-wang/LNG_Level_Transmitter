/*
 * algo_CapToLevel.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 电容值 → 液位（容积 + 高度）转换算法
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. 依赖 algo_LNG_Level_Table 的查表能力
 */

#ifndef ALGORITHM_ALGO_CAPTOLEVEL_H_
#define ALGORITHM_ALGO_CAPTOLEVEL_H_

#include <stdint.h>
#include "algo_LNG_Level_Table.h"

/*******************************************************************************
 * API
 ******************************************************************************/

/*
 * 电容值 → 液位转换：
 *   1. pF*1000 → 整数 pF
 *   2. 钳位到表范围 [0, MaxTableIndex_pF]
 *   3. 查表输出容积 (L) 和液位高度 (mm)
 *
 * 返回值：0 = 正常, 1 = 电容超限已钳位, 2 = 配置无效 (profile 为空)
 */
uint8_t ALGO_CapToLevel_Convert(const LngCylinderProfile_t *pProfile,
                                uint32_t cap_pF_x1000,
                                uint16_t *out_volume_L,
                                uint16_t *out_height_mm);

#endif /* ALGORITHM_ALGO_CAPTOLEVEL_H_ */
