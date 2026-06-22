/*!
** Copyright 2020 NXP
** @file main.c
** @brief  S32K142 CAN APP - 接收0x10 0x02后跳转回Bootloader
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */

/* Including necessary configuration files. */
#include "can_hal.h"
#include "uart_hal.h"
#include "watchdog_hal.h"
#include "sdk_project_config.h"
#include "device_registers.h"
#include "wdog_driver.h"
#include "includes.h"

volatile int exit_code = 0;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* 阶段1: 关闭bootloader的看门狗 */
    *(volatile uint8 *)0x20002FF2u = 0xA1u;
    WDOG_DRV_Trigger(WDOG_CONFIG_1_INST);
    (void)WDOG_DRV_Deinit(WDOG_CONFIG_1_INST);

    /* 阶段2: BSP初始化 */
    *(volatile uint8 *)0x20002FF2u = 0xA2u;
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    WATCHDOG_HAL_Init();

    /* 阶段3: CAN初始化（内部自动注册UDS Bootloader触发回调） */
    *(volatile uint8 *)0x20002FF2u = 0xA3u;
    CAN_HAL_Init();

    /* 使能全局中断 */
    INT_SYS_EnableIRQGlobal();

    /* UART初始化并打印欢迎信息 */
    UART_HAL_Init();
    UART_HAL_SendString("Welcome to the YWJ Project2\r\n");

    for (;;)
    {
        WATCHDOG_HAL_Fed();

        if (exit_code != 0)
        {
            break;
        }
    }

    return exit_code;
}

/* END main */
/*!
** @}
*/

void HardFault_Handler(void)
{
    *(volatile uint8 *)0x20002FF2u = 0xE1u;
    while (1)
    {
    }
}
