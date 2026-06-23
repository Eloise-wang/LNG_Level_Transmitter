/*
 * fifo_hal.c
 *
 *  Created on: 2026年6月22日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: CAN帧环形缓冲区实现
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. SPSC 无锁设计：head 仅由生产者(ISR)修改，tail 仅由消费者(主循环)修改
 *              5. 保留1个空槽用于区分"满"和"空"，实际可用容量 = CAN_FIFO_SIZE - 1
 */

#include "fifo_hal.h"

/*******************************************************************************
 * Local Defines
 ******************************************************************************/

#define CAN_FIFO_MASK  (CAN_FIFO_SIZE - 1u)

/*******************************************************************************
 * Local Types
 ******************************************************************************/

typedef struct
{
    can_message_t buffer[CAN_FIFO_SIZE];  /* CAN帧存储区 */
    volatile uint32_t head;               /* 写指针，仅 ISR 修改 */
    volatile uint32_t tail;               /* 读指针，仅主循环修改 */
} can_fifo_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

static can_fifo_t gs_canFifo;

/*******************************************************************************
 * API
 ******************************************************************************/

void CAN_FIFO_Init(void)
{
    gs_canFifo.head = 0u;
    gs_canFifo.tail = 0u;
}

bool CAN_FIFO_Put(const can_message_t *msg)
{
    uint32_t head = gs_canFifo.head;
    uint32_t next = (head + 1u) & CAN_FIFO_MASK;

    /* 满：下一个写位置追上读位置 */
    if (next == gs_canFifo.tail)
    {
        return false;
    }

    gs_canFifo.buffer[head] = *msg;
    /* 确保帧数据写入完成后再更新 head，防止消费者读到不完整数据 */
    gs_canFifo.head = next;
    return true;
}

bool CAN_FIFO_Get(can_message_t *msg)
{
    uint32_t tail = gs_canFifo.tail;

    /* 空：读写指针重合 */
    if (gs_canFifo.head == tail)
    {
        return false;
    }

    *msg = gs_canFifo.buffer[tail];
    gs_canFifo.tail = (tail + 1u) & CAN_FIFO_MASK;
    return true;
}

bool CAN_FIFO_IsEmpty(void)
{
    return (gs_canFifo.head == gs_canFifo.tail);
}

uint32_t CAN_FIFO_Count(void)
{
    return (gs_canFifo.head - gs_canFifo.tail) & CAN_FIFO_MASK;
}
