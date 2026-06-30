/*
 * app_TankParameterConfig.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 气瓶参数配置应用层 —— 编译期默认值 + 运行时覆盖
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. 切换气瓶型号：修改下方两个宏，重新编译即可
 */

#ifndef APP_APP_TANKPARAMETERCONFIG_H_
#define APP_APP_TANKPARAMETERCONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include "algo_LNG_Level_Table.h"

/*******************************************************************************
 * 编译期默认气瓶参数（切换产品型号改这里）
 ******************************************************************************/

#define TANK_DEFAULT_CYLINDER_VOL_L    1000U   /* 气瓶总容积 (L)    */
#define TANK_DEFAULT_SENSOR_LENGTH_MM  850U    /* 传感器长度 (mm)   */

/*******************************************************************************
 * API
 ******************************************************************************/

/* 使用编译期默认参数自动匹配 profile（上电初始化调用一次即可） */
bool APP_TankParam_Init(void);

/* 运行时覆盖配置（例如通过 CAN 下发新参数） */
bool APP_TankParam_Config(uint16_t cylinderVol_L, uint16_t sensorLength_mm);

/* 获取当前匹配的 profile 指针（未配置返回 NULL） */
const LngCylinderProfile_t * APP_TankParam_GetProfile(void);

/* 是否已完成配置 */
bool APP_TankParam_IsConfigured(void);

#endif /* APP_APP_TANKPARAMETERCONFIG_H_ */
