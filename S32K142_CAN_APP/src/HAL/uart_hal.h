/*
 * uart_hal.h
 *
 *  Created on: 2026年6月22日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: UART HAL层，封装LPUART驱动
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef HAL_UART_HAL_H_
#define HAL_UART_HAL_H_

#include "lpuart_driver.h"

/*******************************************************************************
 * API
 ******************************************************************************/

/* UART HAL 初始化 */
void UART_HAL_Init(void);

/* 发送单个字符 (阻塞) */
void UART_HAL_SendChar(char c);

/* 发送字符串 (阻塞) */
void UART_HAL_SendString(const char *str);

/* 发送 32 位 Hex (阻塞) */
void UART_HAL_SendHex32(uint32_t value);

#endif /* HAL_UART_HAL_H_ */
