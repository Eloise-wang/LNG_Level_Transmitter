/*
 * algo_filter.c
 *
 *  Created on: 2026年6月30日
 *      Author: Eloise
 *     Project: S32K142_CAN_APP
 *       Brief: 数字滤波算法实现
 *       Note : 1. 适配芯片：S32K142_64
 *              2. 编码格式：UTF-8
 *              3. 编译环境：S32DS 3.4 + GCC 7.2.1
 */

#include "algo_filter.h"

uint32_t ALGO_FILTER_Median5(const uint32_t samples[5])
{
    uint32_t sorted[5];
    uint32_t i;
    uint32_t j;

    /* 拷贝到局部数组，避免修改入参 */
    for (i = 0U; i < 5U; i++)
    {
        sorted[i] = samples[i];
    }

    /* 插入排序，5 个元素最优 */
    for (i = 1U; i < 5U; i++)
    {
        uint32_t key = sorted[i];

        j = i;
        while ((j > 0U) && (sorted[j - 1U] > key))
        {
            sorted[j] = sorted[j - 1U];
            j--;
        }
        sorted[j] = key;
    }

    return sorted[2U];
}
