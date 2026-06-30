/*
 * app_LNG_Level_Table.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 【 】
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef APP_APP_LNG_LEVEL_TABLE_H_
#define APP_APP_LNG_LEVEL_TABLE_H_

#include <stdint.h>

/* 基础查表数据节点定义 */
typedef struct {
    uint16_t Volume_L;     // 当前容积 (升)
    uint16_t Height_mm;    // 当前液位高度 (毫米)
} LngLevelData_t;

/* 气瓶总成配置参数定义 */
typedef struct {
    uint16_t CylinderVol_L;      // 特征码1：气瓶总容积 (例如: 1000)
    uint16_t SensorLength_mm;    // 特征码2：传感器长度 (例如: 850)

    // 自动匹配的关联参数
    uint16_t DeadVolume_L;       // 留底容积
    float    DielectricConst;    // 介电常数
    uint16_t GrowthCap_pF;       // 增长电容
    uint16_t EffectiveVol_L;     // 有效容积
    uint16_t MaxTableIndex_pF;   // 查表最大安全索引值
    const LngLevelData_t* pTable;// 数据表指针
} LngCylinderProfile_t;

/* ====================================================================
 * API 接口声明
 * ==================================================================== */

/**
 * @brief  自动匹配气瓶配置参数
 * @param  target_vol_l    目标气瓶容积 (L)
 * @param  target_length_mm 目标传感器长度 (mm)
 * @return 匹配成功返回配置结构体指针；匹配失败返回 0 (NULL)
 */
const LngCylinderProfile_t* AutoMatch_Cylinder_Profile(uint16_t target_vol_l, uint16_t target_length_mm);

/**
 * @brief  通用液位查表函数 (入参改为刚刚匹配到的指针)
 */
uint8_t Get_LngLevel_By_Profile(const LngCylinderProfile_t* pProfile,
                                uint16_t sensor_pf,
                                uint16_t* out_vol,
                                uint16_t* out_height);

#endif /* APP_APP_LNG_LEVEL_TABLE_H_ */
