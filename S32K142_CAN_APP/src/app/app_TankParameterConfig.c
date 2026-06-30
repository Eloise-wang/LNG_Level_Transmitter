/*
 * app_TankParameterConfig.c
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 气瓶参数配置应用层实现
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "app_TankParameterConfig.h"

static const LngCylinderProfile_t *s_pProfile = 0;

bool APP_TankParam_Init(void)
{
    return APP_TankParam_Config(TANK_DEFAULT_CYLINDER_VOL_L,
                                TANK_DEFAULT_SENSOR_LENGTH_MM);
}

bool APP_TankParam_Config(uint16_t cylinderVol_L, uint16_t sensorLength_mm)
{
    s_pProfile = AutoMatch_Cylinder_Profile(cylinderVol_L, sensorLength_mm);
    return (s_pProfile != 0);
}

const LngCylinderProfile_t * APP_TankParam_GetProfile(void)
{
    return s_pProfile;
}

bool APP_TankParam_IsConfigured(void)
{
    return (s_pProfile != 0);
}
