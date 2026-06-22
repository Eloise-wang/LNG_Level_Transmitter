
/************************************************************************************
**	  金龙107开发板（V1.0）
**	  SPI FLASH实验
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.1
**	  作    者：openmcu
**	  完成日期:	2014.3.14
------------------------------------------------------------------------------------
**	程序测试说明: 
**				  
**
**				  JLINK下载运行后，用串口线连接串口，打开串口调试助手，按下复位键，
**			  调试助手会显示相关信息。		   
**                                
************************************************************************************/
#include <stdio.h>
#include "hw_config.h"
#include "w25q16.h"
#include "../PCAP/pcap.h"
#include "../Public/Common/Common.h"



uint8_t TX_Buffer[256];
uint8_t RX_Buffer[256];

unsigned int table[]={0xc04200ff,0xc1201022,0xc20f460b,0xc3090000,0xc4080001,0xc5000000,
	                  0xc6004340,0xc71f0000,0xc8800010,0xc9ff000f,0xca180047,0xd4000001};//寄存器配置

unsigned long cap_status = 0,cap_reszult0 = 0,cap_reszult6 = 0,cap_reszult7 = 0 ,cap_temp = 0;
float cap_val = 0.0f;
unsigned char cap[6] = {'3','5','.','1','2',' '};
unsigned int capsw,capgw,capzs,capxs1,capxs2;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void Delay(void)
{
	int x,y;
	for(x=1000;x>0;x--)
		for(y=1000;y>0;y--);

}

void Delayms(void)
{
	int x;
	for(x=1000;x>0;x--);
}


int main(void)
{
    int t= 0;
	SystemInit();     	                         //系统初始化

	Delay_Init();

	SPI_Configuration();

	USART_Configuration();

	TIMER_Configuration();

    NVIC_Configuration();

	GPIO_SetBits(GPIOB,GPIO_Pin_9);

	cap01_init();

    //delay_ms(500);
	while(1)
	{
        SPI_Disable();
	    __3nop();
        write_date_8(0x48);   //addr8
        __3nop();
        cap_status = spi_read();//返回结果应该为0x100000或0x900000(在测量时通讯,准确性 	

		  SPI_Disable();
	    __3nop();
        write_date_8(0x41);   //addr0 Res0 C0 LSB 0-7.9999  0b01xxxxxx
        __3nop();
        //cap_reszult0 = spi_read();

		cap_reszult0 = 0x1b3011;
        cap_temp = 0x1fffff;
		cap_val = (double)cap_reszult0/(double)cap_temp;
		cap_val = cap_val*47;

		capzs = (unsigned int)cap_val;
		capsw = capzs/10;
		capgw = capzs%10;

		cap_val = cap_val - capzs;
		cap_val = cap_val*100;
		capzs = (unsigned int)cap_val;
		capxs1 = capzs/10;
		capxs2 = capzs%10;

		cap[0] = capsw + '0';
		cap[1] = capgw + '0';
		cap[3] = capxs1 + '0';
		cap[4] = capxs2 + '0';
		
		
	}	
}


PUTCHAR_PROTOTYPE
{
  
  USART_SendData(USART1, (uint8_t) ch);   						 //发送一个字符

  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	 //等待传输结束
  {}

  return ch;
}


