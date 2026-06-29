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
#include "pcap01.h"
#include "sdk_project_config.h"
#include "device_registers.h"
#include "wdog_driver.h"
#include "osif.h"
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

    /* 阶段3: CAN初始化（内部自动注册UDS Bootloader触发回调） */
    *(volatile uint8 *)0x20002FF2u = 0xA3u;
    CAN_HAL_Init();

    /* 使能全局中断 */
    INT_SYS_EnableIRQGlobal();

    /* UART初始化并打印欢迎信息 */
    UART_HAL_Init();
    UART_HAL_SendString("\r\nRCM_SRS=");
    UART_HAL_SendHex32(WATCHDOG_HAL_GetResetSrc());
    UART_HAL_SendString("\r\n");
    UART_HAL_SendString("Welcome to the YWJ Project2\r\n");

    /* PCAP01初始化（耗时较长，在看门狗启动前完成） */
    PCAP01_Init();
    UART_HAL_SendString("PCAP01 Init done.\r\n");

    /* 所有初始化完成后再启动看门狗，超时约 0.47s */
    WATCHDOG_HAL_Init();

    for (;;)
    {
        uint32_t rawIdx[8];
        uint32_t ratioRaw;
        uint32_t ratio_x10000;
        uint32_t cap_pF;
        uint32_t index;

        for (index = 0U; index < 8U; index++)
        {
            rawIdx[index] = PCAP01_ReadData(index + 1U);
        }

        ratioRaw = rawIdx[0];
        cap_pF = PCAP01_RawToCapacitance_pF(ratioRaw);
        ratio_x10000 = (uint32_t)(((uint64_t)ratioRaw * 10000ULL + (1ULL << 20U)) >> 21U);

        WATCHDOG_HAL_Fed();

        UART_HAL_SendString("I1:");
        UART_HAL_SendHex32(rawIdx[0]);
        UART_HAL_SendString(" I2:");
        UART_HAL_SendHex32(rawIdx[1]);
        UART_HAL_SendString(" I3:");
        UART_HAL_SendHex32(rawIdx[2]);
        UART_HAL_SendString(" I4:");
        UART_HAL_SendHex32(rawIdx[3]);
        UART_HAL_SendString(" I5:");
        UART_HAL_SendHex32(rawIdx[4]);
        UART_HAL_SendString(" I6:");
        UART_HAL_SendHex32(rawIdx[5]);
        UART_HAL_SendString(" I7:");
        UART_HAL_SendHex32(rawIdx[6]);
        UART_HAL_SendString(" I8:");
        UART_HAL_SendHex32(rawIdx[7]);
        UART_HAL_SendString(" Kx10000:");
        UART_HAL_SendDec32(ratio_x10000);
        UART_HAL_SendString(" C:");
        UART_HAL_SendDec32(cap_pF);
        UART_HAL_SendString("pF\r\n");

        /* 分片延时 500ms，每 30ms 喂一次狗 */
        for (uint32_t i = 0U; i < 500U; i += 30U)
        {
            WATCHDOG_HAL_Fed();
            OSIF_TimeDelay(30U);
        }

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
