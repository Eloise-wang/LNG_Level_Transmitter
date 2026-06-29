/*
 * app_pcap01_INTN.h
 *
 *  Created on: 2026年6月29日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 【 】
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef APP_APP_PCAP01_INTN_H_
#define APP_APP_PCAP01_INTN_H_

#include <stdbool.h>
#include <stdint.h>

void APP_PCAP01_INTN_Init(void);

//清除事件
void APP_PCAP01_INTN_ClearEvent(void);

//取事件
bool APP_PCAP01_INTN_TakeEvent(void);

#endif /* APP_APP_PCAP01_INTN_H_ */
