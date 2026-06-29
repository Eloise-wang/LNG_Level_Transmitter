/*
 * app_pcap01_INTN.c
 *
 *  Created on: 2026年6月29日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 【 】
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "app_pcap01_INTN.h"

#include "device_registers.h"
#include "interrupt_manager.h"
#include "pins_driver.h"

static volatile uint32_t s_pcap01IntnEvent = 0U;

void PORTA_IRQHandler(void)
{
    if ((PINS_DRV_GetPortIntFlag(PORTA) & (1UL << 1U)) != 0U)
    {
        PINS_DRV_ClearPinIntFlagCmd(PORTA, 1U);
        s_pcap01IntnEvent = 1U;
    }
}

void APP_PCAP01_INTN_Init(void)
{
    PINS_DRV_SetPinIntSel(PORTA, 1U, PORT_INT_EITHER_EDGE);
    PINS_DRV_ClearPinIntFlagCmd(PORTA, 1U);
    INT_SYS_EnableIRQ(PORTA_IRQn);
    s_pcap01IntnEvent = 0U;
}

void APP_PCAP01_INTN_ClearEvent(void)
{
    s_pcap01IntnEvent = 0U;
}

bool APP_PCAP01_INTN_TakeEvent(void)
{
    if (s_pcap01IntnEvent != 0U)
    {
        s_pcap01IntnEvent = 0U;
        return true;
    }

    return false;
}

