/*
 * app_pcap01.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 电容采集应用层 —— 编排 HAL + 滤波，完成一次完整采集
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef APP_APP_PCAP01_H_
#define APP_APP_PCAP01_H_

#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * API
 ******************************************************************************/

/* 初始化 PCAP01 芯片及 INTN 中断 */
void APP_PCAP01_Init(void);

/*
 * 执行一次完整电容采集：
 *   等待 INTN / 轮询 DRDY → 5 样本中值滤波 → 转换为 pF
 *
 * 返回值：true = 采集成功，out_pF_x1000 为电容值 (pF * 1000，即 3 位小数)
 *         false = 超时或芯片未就绪，out_pF_x1000 不写入
 */
bool APP_PCAP01_ReadCapacitance_pF_x1000(uint32_t *out_pF_x1000);

#endif /* APP_APP_PCAP01_H_ */
