/******************************************************************
**	  金龙107开发板（V1.0）
**	  外设配置文件
**
**	  论    坛：bbs.openmcu.com
**	  旺    宝：www.openmcu.com
**	  邮    箱：support@openmcu.com
**
**    版    本：V1.1
**	  作    者：openmcu
**	  完成日期:	2014.3.14
********************************************************************/
#include "hw_config.h"

/**************************************************
*函数名称：void LED_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：LED初始化配置
***************************************************/

void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED, ENABLE);	                      //使能GPIOD时钟
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                      // 管脚频率为50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	                      //推挽输出
	GPIO_Init(LED_PORT, &GPIO_InitStructure);			                      //初始化GPIOD

	LED1(0);		 //LED全OFF
	LED2(0);
	LED3(0);
	LED4(0);		
}

/**************************************************
*函数名称：void USART_Configuration(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：USART初始化配置 包括GPIO初始化 TX必须配置为复用输出
***************************************************/
void USART_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    /*USART1端口配置
	  PA9 TX 复用推挽输出 PA10 RX 浮空输入模式*/
	GPIO_InitStructure.GPIO_Pin   = USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_PORT,&GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin   = USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_PORT, &GPIO_InitStructure);
      	
    /*--------------------USART1配置----------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);	
}

void USART1_SendString(uint8_t *ch)
{
	while(*ch!=0)
	{		
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, *ch);
		ch++;
	}   	
}

void TIMER_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

	/*Prescaler 预分频系数为36000-1，这样计数器时钟为72MHz/36000 = 2kHz
	   Period    设置计数溢出大小，每计2000个数就产生一个更新事件
	   这样就实现了1s闪烁一次
	*/
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36000-1;               // 分频值  最大65536

	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	  //向上计数模式

	TIM_TimeBaseInitStructure.TIM_Period = 2000-1; 		              //ARR的值 最大65536

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);	     // 中断使能

 	TIM_Cmd(TIM4, ENABLE);                        //使能定时器
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);   	  //使用优先级分组0
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;       //指定中断源
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    // 指定响应优先级别1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

