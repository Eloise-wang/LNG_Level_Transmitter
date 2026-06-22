/*
 * can_hal.c
 *
 *  Created on: 2026年6月22日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: CAN HAL层实现，基于can_pal驱动
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "can_hal.h"
#include "device_registers.h"
#include "peripherals_can_pal_1.h"
#include "boot_Cfg.h"
#include "watchdog_hal.h"

/*******************************************************************************
 * Local Defines
 ******************************************************************************/

#define CAN_RX_FUN_BUFF_IDX  0u
#define CAN_RX_PHY_BUFF_IDX  1u
#define CAN_TX_BUFF_IDX      2u

/*******************************************************************************
 * Local Types
 ******************************************************************************/

typedef void (*can_hal_rx_callback_t)(uint32_t msgId, uint8_t length, const uint8_t *data);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static can_message_t gs_rxCanMsg;
static can_hal_rx_callback_t gs_canRxCallback = NULL;

/*******************************************************************************
 * Static Functions
 ******************************************************************************/

/*******************************************************************************
 * CAN接收回调 - 检测 UDS DiagnosticSessionControl → ProgrammingSession (0x10 0x02)
 * ISO 15765-2 SF 格式: [02] [10] [02] [AA] [AA] [AA] [AA] [AA]
 * 收到后发送 Positive Response (50 02)，然后跳转回 Bootloader
 ******************************************************************************/
static void CAN_UdsBootHandler(uint32_t msgId, uint8_t length, const uint8_t *data)
{
    (void)msgId;

    if (length >= 3u && data[0u] == 0x02u && data[1u] == 0x10u && data[2u] == 0x02u)
    {
        /* 发送 UDS Positive Response: 50 02 */
        const uint8_t rsp[] = {0x02u, 0x50u, 0x02u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};
        CAN_HAL_Send(CAN_TX_ID, sizeof(rsp), rsp);

        Boot_RequestEnterBootloader();
        WATCHDOG_HAL_SystemRest();
        while (1)
        {
            /* 等待看门狗复位 */
        }
    }
}

/* CAN RX/TX 完成回调 (中断上下文) */
static void CAN_EventCallback(uint32_t instance,
                              can_event_t eventType,
                              uint32_t objIdx,
                              void *driverState)
{
    (void)instance;
    (void)objIdx;
    (void)driverState;

    switch (eventType)
    {
    case CAN_EVENT_RX_COMPLETE:
        if (gs_canRxCallback != NULL)
        {
            gs_canRxCallback(gs_rxCanMsg.id, gs_rxCanMsg.length, gs_rxCanMsg.data);
        }

        /* 重新使能接收 */
        CAN_Receive(&can_pal1_instance, CAN_RX_FUN_BUFF_IDX, &gs_rxCanMsg);
        CAN_Receive(&can_pal1_instance, CAN_RX_PHY_BUFF_IDX, &gs_rxCanMsg);
        break;

    case CAN_EVENT_TX_COMPLETE:
        break;

    default:
        break;
    }
}

/*******************************************************************************
 * API
 ******************************************************************************/

void CAN_HAL_Init(void)
{
    /* CAN模块初始化 */
    CAN_Init(&can_pal1_instance, &can_pal1_Config0);

    /* 注册CAN事件回调 */
    CAN_InstallEventCallback(&can_pal1_instance, CAN_EventCallback, NULL);

    /* 注册UDS Bootloader触发回调 */
    gs_canRxCallback = CAN_UdsBootHandler;

    /* 配置 Function ID 接收buffer */
    {
        can_buff_config_t rxFunConfig = {
            .enableFD = true,
            .enableBRS = true,
            .fdPadding = 0U,
            .idType = FLEXCAN_MSG_ID_STD,
            .isRemote = false
        };
        CAN_ConfigRxBuff(&can_pal1_instance, CAN_RX_FUN_BUFF_IDX, &rxFunConfig, CAN_RX_FUN_ID);
        CAN_Receive(&can_pal1_instance, CAN_RX_FUN_BUFF_IDX, &gs_rxCanMsg);
    }

    /* 配置 Physical ID 接收buffer */
    {
        can_buff_config_t rxPhyConfig = {
            .enableFD = true,
            .enableBRS = true,
            .fdPadding = 0U,
            .idType = FLEXCAN_MSG_ID_STD,
            .isRemote = false
        };
        CAN_ConfigRxBuff(&can_pal1_instance, CAN_RX_PHY_BUFF_IDX, &rxPhyConfig, CAN_RX_PHY_ID);
        CAN_Receive(&can_pal1_instance, CAN_RX_PHY_BUFF_IDX, &gs_rxCanMsg);
    }

    /* 配置发送buffer */
    {
        can_buff_config_t txConfig = {
            .enableFD = false,
            .enableBRS = false,
            .fdPadding = 0U,
            .idType = FLEXCAN_MSG_ID_STD,
            .isRemote = false
        };
        CAN_ConfigTxBuff(&can_pal1_instance, CAN_TX_BUFF_IDX, &txConfig);
    }

    /* 使能CAN中断 */
    INT_SYS_EnableIRQ(CAN0_ORed_0_15_MB_IRQn);
}

bool CAN_HAL_Send(uint32_t msgId, uint8_t length, const uint8_t *data)
{
    can_message_t txMsg;

    txMsg.cs = 0u;
    txMsg.id = msgId;
    txMsg.length = length;

    for (uint8_t i = 0u; i < length; i++)
    {
        txMsg.data[i] = data[i];
    }

    /* 重新配置TX buffer */
    can_buff_config_t txConfig = {
        .enableFD = false,
        .enableBRS = false,
        .fdPadding = 0U,
        .idType = FLEXCAN_MSG_ID_STD,
        .isRemote = false
    };
    CAN_ConfigTxBuff(&can_pal1_instance, CAN_TX_BUFF_IDX, &txConfig);

    if (STATUS_SUCCESS != CAN_Send(&can_pal1_instance, CAN_TX_BUFF_IDX, &txMsg))
    {
        return false;
    }

    return true;
}
