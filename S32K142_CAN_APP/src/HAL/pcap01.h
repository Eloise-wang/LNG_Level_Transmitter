/*
 * pcap01.h
 *
 *  Created on: 2026年6月23日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: PCAP01 电容传感器 HAL层，基于 LPSPI0 硬件SPI通信
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 *              4. SPI模式：Mode 1 (CPOL=0, CPHA=1), CS低有效
 *              5. 引脚复用：PTE0(SCK) PTE1(MISO) PTE2(MOSI) PTE6(CS)
 */

#ifndef HAL_PCAP01_H_
#define HAL_PCAP01_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * API
 ******************************************************************************/

/* PCAP01 初始化：SPI复位、下载固件、配置寄存器、开始转换 */
void PCAP01_Init(void);

/*
 * PCAP01 的工作原理（皮法级微电容测量芯片）：
*       它内部使用的是 acam 公司核心的 TDC（Time-to-Digital Converter，时间数字转换器） 架构。
*      芯片测量电容时，会让内部的恒流源或已知电阻对你的液位计电容进行充电和放电，测量这个电容充放电所需的时间。
*      因为电容越大，充放电时间就越长。
*    TDC 以皮秒（$ps$）级的极高精度记录下这个时间，然后通过芯片内部的 DSP 算出一个比值，最终转换为一个 24 位的数字量。
*    返回的是 24 位原始 DC 比率值（DC ratio)
*    ====================================================================
 * 读取电容测量结果 (24位原始值)
 * index: 结果寄存器地址 (1=C0, 2=C1, 3=C2, ... 7=C6)
 *        对应关系：第一个使能的端口→C0，第二个→C1，以此类推
 *        当前配置下 PC1 对应 C1，调用 PCAP01_ReadData(2U)
 * 返回值: 24位原始ADC值
 */
uint32_t PCAP01_ReadData(uint32_t index);

#endif /* HAL_PCAP01_H_ */
