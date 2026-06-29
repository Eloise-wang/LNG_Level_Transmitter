/*
 * uart_hal.c
 *
 *  Created on: 2026年6月22日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: UART HAL层实现，基于lpuart_driver
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "uart_hal.h"
#include "peripherals_lpuart_1.h"

void UART_HAL_Init(void)
{
    LPUART_DRV_Init(INST_LPUART_1, &lpUartState0, &lpuart1_InitConfig0);
}

void UART_HAL_SendChar(char c)
{
    LPUART_DRV_SendDataBlocking(INST_LPUART_1, (const uint8_t *)&c, 1u, 100u);
}

void UART_HAL_SendString(const char *str)
{
    while (*str != '\0')
    {
        UART_HAL_SendChar(*str);
        str++;
    }
}

void UART_HAL_SendHex32(uint32_t value)
{
    static const char hexChars[] = "0123456789ABCDEF";

    UART_HAL_SendString("0x");
    for (int32_t i = 28; i >= 0; i -= 4)
    {
        UART_HAL_SendChar(hexChars[(value >> (uint32_t)i) & 0x0FU]);
    }
}

void UART_HAL_SendDec32(uint32_t value)
{
    char buf[12]; /* 最大 4294967295 = 10 位 + '\0' */
    int32_t pos = sizeof(buf) - 1U;

    buf[pos] = '\0';

    if (value == 0U)
    {
        UART_HAL_SendChar('0');
        return;
    }

    while (value != 0U)
    {
        buf[--pos] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    while (buf[pos] != '\0')
    {
        UART_HAL_SendChar(buf[pos++]);
    }
}
