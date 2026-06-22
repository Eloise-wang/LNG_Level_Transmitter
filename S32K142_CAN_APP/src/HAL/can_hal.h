/*
 * can_hal.h
 *
 *  Created on: 2026年6月22日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: CAN HAL层，封装CAN PAL驱动
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef HAL_CAN_HAL_H_
#define HAL_CAN_HAL_H_

#include "can_pal.h"

/* CAN 消息 ID (与 bootloader 通信) */
#define CAN_RX_FUN_ID   0x7FFu
#define CAN_RX_PHY_ID   0x784u
#define CAN_TX_ID       0x7F0u

/*******************************************************************************
 * API
 ******************************************************************************/

/* CAN HAL 初始化: 配置CAN模块、接收buffer、使能中断，自动注册UDS Bootloader触发 */
void CAN_HAL_Init(void);

/* CAN 发送消息 */
bool CAN_HAL_Send(uint32_t msgId, uint8_t length, const uint8_t *data);

#endif /* HAL_CAN_HAL_H_ */
