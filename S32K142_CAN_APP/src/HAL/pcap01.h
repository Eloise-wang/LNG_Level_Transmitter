/*
 * pcap01.h
 *
 *  Created on: 2026年6月23日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 电容传感器 HAL层，基于 LPSPI0 硬件SPI通信
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. SPI模式：Mode 1 (CPOL=0, CPHA=1), CS低有效
 *              5. 引脚复用：PTE0(SCK) PTE1(MISO) PTE2(MOSI) PTE6(CS)
 */

#ifndef HAL_PCAP01_H_
#define HAL_PCAP01_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * API
 ******************************************************************************/

void PCAP01_Init(void);

uint32_t PCAP01_ReadData(uint32_t index);

uint32_t PCAP01_RawToCapacitance_pF(uint32_t rawValue);

#endif /* HAL_PCAP01_H_ */
