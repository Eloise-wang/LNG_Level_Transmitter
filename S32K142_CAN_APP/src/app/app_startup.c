/*
 * app_startup.c
 *
 *  Created on: 2026年6月29日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 【 】
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "app_startup.h"

#include "app_TankParameterConfig.h"
#include "can_hal.h"
#include "test_pcap01.h"
#include "uart_hal.h"
#include "watchdog_hal.h"

#include "device_registers.h"
#include "sdk_project_config.h"
#include "wdog_driver.h"

#include "includes.h"

void APP_Startup_Run(void)
{
    *(volatile uint8 *)0x20002FF2u = 0xA1u;
    WDOG_DRV_Trigger(WDOG_CONFIG_1_INST);
    (void)WDOG_DRV_Deinit(WDOG_CONFIG_1_INST);

    *(volatile uint8 *)0x20002FF2u = 0xA2u;
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    *(volatile uint8 *)0x20002FF2u = 0xA3u;
    CAN_HAL_Init();

    INT_SYS_EnableIRQGlobal();

    UART_HAL_Init();
    UART_HAL_SendString("\r\nRCM_SRS=");
    UART_HAL_SendHex32(WATCHDOG_HAL_GetResetSrc());
    UART_HAL_SendString("\r\n");
    UART_HAL_SendString("Welcome to the YWJ Project2\r\n");

    WATCHDOG_HAL_Init();

    TEST_PCAP01_Init();

    APP_TankParam_Init();
}

