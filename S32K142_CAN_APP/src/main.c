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
        uint32_t status;
        uint32_t ratioRaw;
        uint32_t ratio_x10000;
        uint32_t cap_pF_x1000;
        uint32_t cap_pF_int;
        uint32_t cap_pF_frac;

        uint32_t ready = 0U;
        for (uint32_t i = 0U; i < 200U; i++)
        {
            status = PCAP01_ReadData(8U);
            if ((status & 0x00100000UL) != 0U)
            {
                ready = 1U;
                break;
            }
            WATCHDOG_HAL_Fed();
            OSIF_TimeDelay(1U);
        }

        if (ready == 0U)
        {
            UART_HAL_SendString("S:");
            UART_HAL_SendHex32(status);
            UART_HAL_SendString(" NRDY\r\n");
            continue;
        }

        status = PCAP01_ReadData(8U);

        ratioRaw = PCAP01_ReadData(1U);
        for (uint32_t i = 0U; i < 5U; i++)
        {
            uint32_t raw2 = PCAP01_ReadData(1U);
            if (raw2 == ratioRaw)
            {
                break;
            }
            ratioRaw = raw2;
            WATCHDOG_HAL_Fed();
            OSIF_TimeDelay(1U);
        }

        cap_pF_x1000 = PCAP01_RawToCapacitance_pF(ratioRaw);
        cap_pF_int = cap_pF_x1000 / 1000U;
        cap_pF_frac = cap_pF_x1000 % 1000U;

        ratio_x10000 = (uint32_t)(((uint64_t)ratioRaw * 10000ULL + (1ULL << 20U)) >> 21U);

        WATCHDOG_HAL_Fed();

        UART_HAL_SendString("S:");
        UART_HAL_SendHex32(status);
        UART_HAL_SendString(" R:");
        UART_HAL_SendHex32(ratioRaw);
        UART_HAL_SendString(" Kx10000:");
        UART_HAL_SendDec32(ratio_x10000);
        UART_HAL_SendString(" C:");
        UART_HAL_SendDec32(cap_pF_int);
        UART_HAL_SendString(".");
        if (cap_pF_frac < 100U) { UART_HAL_SendString("0"); }
        if (cap_pF_frac < 10U)  { UART_HAL_SendString("0"); }
        UART_HAL_SendDec32(cap_pF_frac);
        UART_HAL_SendString("pF\r\n");

        for (uint32_t i = 0U; i < 50U; i += 10U)
        {
            WATCHDOG_HAL_Fed();
            OSIF_TimeDelay(10U);
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
