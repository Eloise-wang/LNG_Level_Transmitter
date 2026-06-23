/*
 * fifo_hal.h
 *
 *  Created on: 2026年6月22日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: CAN帧环形缓冲区，防止中断上下文与主循环之间的帧漏收
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. SPSC 无锁设计：Put(ISR生产者) / Get(主循环消费者)
 */

#ifndef HAL_FIFO_HAL_H_
#define HAL_FIFO_HAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "can_pal.h"

/* 环形缓冲区容量，必须是2的幂 */
#define CAN_FIFO_SIZE  32u

/*******************************************************************************
 * API
 ******************************************************************************/

/* 初始化环形缓冲区 */
void CAN_FIFO_Init(void);

/* 写入一帧到缓冲区 (中断上下文调用)，满时返回 false */
bool CAN_FIFO_Put(const can_message_t *msg);

/* 从缓冲区取出一帧 (主循环调用)，空时返回 false */
bool CAN_FIFO_Get(can_message_t *msg);

/* 判断缓冲区是否为空 */
bool CAN_FIFO_IsEmpty(void);

/* 获取缓冲区中当前帧数量 */
uint32_t CAN_FIFO_Count(void);

#endif /* HAL_FIFO_HAL_H_ */
