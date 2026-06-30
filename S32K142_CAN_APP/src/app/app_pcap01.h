/*
 * app_pcap01.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 电容采集应用层 —— 编排 HAL + 滤波，含状态机
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef APP_APP_PCAP01_H_
#define APP_APP_PCAP01_H_

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * 采集状态机
 ******************************************************************************/

typedef enum
{
    PCAP01_STATE_NOT_INIT   = 0x00U,  /* 未初始化                         */
    PCAP01_STATE_INIT_DONE  = 0x01U,  /* 初始化完成，空闲                  */
    PCAP01_STATE_WAIT_INTN  = 0x02U,  /* 等待 INTN 中断触发                */
    PCAP01_STATE_WAIT_DRDY  = 0x03U,  /* INTN 超时，降级为轮询 DRDY 位      */
    PCAP01_STATE_DATA_READY = 0x04U,  /* 芯片确认数据就绪                  */
    PCAP01_STATE_SAMPLING   = 0x05U,  /* 多采样 + 中值滤波中               */
    PCAP01_STATE_DATA_VALID = 0x06U,  /* 采集成功，数据可信                */
    PCAP01_STATE_TIMEOUT    = 0x07U,  /* 双重超时，芯片无响应              */
    PCAP01_STATE_NOT_READY  = 0x08U,  /* 芯片响应但状态寄存器无效           */
    PCAP01_STATE_CHIP_FAULT = 0x09U,  /* 连续多次失败，芯片疑似故障         */
} PCAP01_State_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/* 初始化 PCAP01 芯片及 INTN 中断，复位状态机 */
void APP_PCAP01_Init(void);

/* 获取当前状态（CAN / 诊断随时可查） */
PCAP01_State_t APP_PCAP01_GetState(void);

/* 状态码 → 可读字符串（调试用） */
const char * APP_PCAP01_GetStateName(PCAP01_State_t state);

/*
 * 执行一次完整电容采集：
 *   NOT_INIT → WAIT_INTN → WAIT_DRDY(降级) → DATA_READY → SAMPLING → DATA_VALID
 *
 * 返回终态（DATA_VALID / TIMEOUT / NOT_READY / CHIP_FAULT）
 * 成功时 *out_pF_x1000 为电容值 (pF * 1000)
 */
PCAP01_State_t APP_PCAP01_ReadCapacitance_pF_x1000(uint32_t *out_pF_x1000);

#endif /* APP_APP_PCAP01_H_ */
