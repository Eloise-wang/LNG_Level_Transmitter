/*
 * watchdog_hal.c
 *
 *  Created on: 2026年4月17日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 【 】
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */
#include "watchdog_hal.h"

/*******************************************************************************
 * Local Defines
 ******************************************************************************/

/* RCM System Reset Status Register 标志位 */
#define RCM_SRS_WDOG_BIT    (1U << 5U)   /* 看门狗复位 */
#define RCM_SRS_PIN_BIT     (1U << 6U)   /* 外部引脚复位 */
#define RCM_SRS_POR_BIT     (1U << 7U)   /* 上电复位 */
#define RCM_SRS_LOCKUP_BIT  (1U << 9U)   /* 内核锁定复位 */
#define RCM_SRS_SW_BIT      (1U << 10U)  /* 软件复位 */

#define WATCHDOG_TIMEOUT_TICKS  60000U

/*******************************************************************************
 * User Include
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : WATCHDOG_HAL_Init
 * Description   : This function initial this module.
 *
 * Implements : WATCHDOG_HAL_Init_Activity
 *END**************************************************************************/
void WATCHDOG_HAL_Init(void)
{
	if(STATUS_SUCCESS != WDOG_DRV_Deinit(WDOG_CONFIG_1_INST))
	{
	}

	if(STATUS_SUCCESS != WDOG_DRV_Init(WDOG_CONFIG_1_INST, &watchdog1_Config0))
	{
		(void)WDOG_DRV_Deinit(WDOG_CONFIG_1_INST);
		(void)WDOG_DRV_Init(WDOG_CONFIG_1_INST, &watchdog1_Config0);
	}

    (void)WDOG_DRV_SetInt(WDOG_CONFIG_1_INST, false);
    (void)WDOG_DRV_SetTimeout(WDOG_CONFIG_1_INST, (uint16_t)WATCHDOG_TIMEOUT_TICKS);
    WDOG_DRV_Trigger(WDOG_CONFIG_1_INST);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WATCHDOG_HAL_Init
 * Description   : This function is fed watchdog.
 *
 * Implements : WATCHDOG_HAL_Init_Activity
 *END**************************************************************************/
void WATCHDOG_HAL_Fed(void)
{
   WDOG_DRV_Trigger(WDOG_CONFIG_1_INST);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : WATCHDOG_HAL_Init
 * Description   : This function is trigger system reset.
 *
 * Implements : WATCHDOG_HAL_Init_Activity
 *END**************************************************************************/

void WATCHDOG_HAL_SystemRest(void)
{
	WDOG_DRV_SetTimeout(WDOG_CONFIG_1_INST, 0u);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : WATCHDOG_HAL_Deinit
 * Description   : This function initial this module.
 *
 * Implements : WATCHDOG_HAL_Deinit_Activity
 *END**************************************************************************/
void WATCHDOG_HAL_Deinit(void)
{


}

uint32_t WATCHDOG_HAL_GetResetSrc(void)
{
    return RCM->SRS;
}
/******************************************************************************
 * EOF
 *****************************************************************************/
