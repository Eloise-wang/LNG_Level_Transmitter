/*
 * algo_filter.h
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 数字滤波算法模块
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#ifndef ALGORITHM_ALGO_FILTER_H_
#define ALGORITHM_ALGO_FILTER_H_

#include <stdint.h>

/*******************************************************************************
 * API
 ******************************************************************************/

/* 5 样本中值滤波：对 samples[5] 排序后返回中值（索引 2）
 * - 调用方负责采集 5 个样本
 * - 无状态，不修改原数组（内部拷贝排序）
 * - 适合抑制偶发尖峰毛刺 */
uint32_t ALGO_FILTER_Median5(const uint32_t samples[5]);

#endif /* ALGORITHM_ALGO_FILTER_H_ */
