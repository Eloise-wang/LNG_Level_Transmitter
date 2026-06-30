/*
 * algo_CapToLevel.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 电容值 → 液位（容积 + 高度）转换算法（含线性插值 + 寄生电容扣除）
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. 依赖 algo_LNG_Level_Table 的查表数据
 *              5. pF 的小数部分（千分位）通过相邻两点线性插值，输出更平滑
 *              6. 寄生电容基线 = 宏默认值 → 未来 CAN/BLE 校准可运行时覆盖
 */

#ifndef ALGORITHM_ALGO_CAPTOLEVEL_H_
#define ALGORITHM_ALGO_CAPTOLEVEL_H_

#include <stdint.h>
#include "algo_LNG_Level_Table.h"

/*******************************************************************************
 * 寄生电容基线（pF * 1000，编译期默认，CAN/BLE 校准后覆盖）
 ******************************************************************************/

#define TANK_DEFAULT_PARASITIC_CAP_PF_X1000  8751U  /* 悬空实测 8.751 pF */

/*******************************************************************************
 * API
 ******************************************************************************/

/* 运行时设置寄生电容基线（CAN / BLE 校准指令调用，单位 pF*1000） */
void ALGO_CapToLevel_SetBaseline(uint32_t baseline_pF_x1000);

/* 查询当前基线值 */
uint32_t ALGO_CapToLevel_GetBaseline(void);

/*
 * 电容值 → 液位转换（固定点线性插值）：
 *
 *   流程：
 *     1. C_effective = max(cap_pF_x1000 - baseline, 0)     ← 扣除寄生电容
 *     2. 分离整数部分 N 和小数部分 f（千分位）
 *     3. 钳位 N 到 [0, MaxTableIndex_pF]
 *     4. 查表取 table[N] 和 table[N+1]，线性插值：
 *          Volume  = table[N].Vol + (table[N+1].Vol - table[N].Vol) * f / 1000
 *          Height  = table[N].Ht  + (table[N+1].Ht  - table[N].Ht ) * f / 1000
 *     5. N 在表尾时直接取 table[N]，不插值
 *
 * 返回值：0 = 正常, 1 = 电容超限已钳位, 2 = 配置无效 (profile 为空)
 */
uint8_t ALGO_CapToLevel_Convert(const LngCylinderProfile_t *pProfile,
                                uint32_t cap_pF_x1000,
                                uint16_t *out_volume_L,
                                uint16_t *out_height_mm);

#endif /* ALGORITHM_ALGO_CAPTOLEVEL_H_ */
