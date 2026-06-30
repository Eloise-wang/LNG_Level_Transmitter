/*
 * pcap01_intn_hal.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 INTN 引脚中断 HAL 层
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. 引脚：PTA1，双边沿触发
 */

#ifndef HAL_PCAP01_INTN_HAL_H_
#define HAL_PCAP01_INTN_HAL_H_

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * API
 ******************************************************************************/

void PCAP01_INTN_HAL_Init(void);

void PCAP01_INTN_HAL_ClearEvent(void);

bool PCAP01_INTN_HAL_TakeEvent(void);

#endif /* HAL_PCAP01_INTN_HAL_H_ */
