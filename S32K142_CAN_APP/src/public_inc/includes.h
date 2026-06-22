/*
 * includes.h
 *
 *  Created on: 2026年6月22日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 公共头文件
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef PUBLIC_INC_INCLUDES_H_
#define PUBLIC_INC_INCLUDES_H_

#include "sdk_project_config.h"
#include "common_types.h"

#include "user_config.h"

/****************************ASSERT*******************/
#ifdef EN_ASSERT
#define ASSERT(xValue)\
do{\
    if(xValue)\
    {\
        while(1){}\
    }\
}while(0)
#else
#define ASSERT(xValue)
#endif

/************************************************************/

#endif /*PUBLIC_INC_INCLUDES_H_*/

/************************End file********************************/
