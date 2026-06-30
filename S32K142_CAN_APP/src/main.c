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
#include "app_startup.h"
#include "test_pcap01.h"
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
	//初始化HAL层
    APP_Startup_Run();

    for (;;)
    {
        TEST_PCAP01_PollAndPrintOnce();
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
