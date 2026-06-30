/*
 * pcap01_intn_hal.c
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 INTN 引脚中断 HAL 层实现
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. PORTA IRQ 共享入口，通过 PTA1 标志识别 INTN 事件
 */

#include "pcap01_intn_hal.h"

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

void PCAP01_INTN_HAL_Init(void)
{
    PINS_DRV_SetPinIntSel(PORTA, 1U, PORT_INT_EITHER_EDGE);
    PINS_DRV_ClearPinIntFlagCmd(PORTA, 1U);
    INT_SYS_EnableIRQ(PORTA_IRQn);
    s_pcap01IntnEvent = 0U;
}

void PCAP01_INTN_HAL_ClearEvent(void)
{
    s_pcap01IntnEvent = 0U;
}

bool PCAP01_INTN_HAL_TakeEvent(void)
{
    if (s_pcap01IntnEvent != 0U)
    {
        s_pcap01IntnEvent = 0U;
        return true;
    }

    return false;
}
