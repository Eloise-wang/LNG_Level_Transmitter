/*
 * test_pcap01.h
 *
 *  Created on: 2026年6月29日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 【 】
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef TEST_TEST_PCAP01_H_
#define TEST_TEST_PCAP01_H_

#include <stdint.h>

void TEST_PCAP01_Init(void);

void TEST_PCAP01_PollAndPrintOnce(void);

#endif /* TEST_TEST_PCAP01_H_ */
