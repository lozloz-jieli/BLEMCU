/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

/**
 * @file uh_random.h
 * @brief 与随机数据相关的接口定义。
 *
 * @date 2017-12-13
 * @author fanming
 *
 */

#ifndef __UH_RANDOM_H__
#define __UH_RANDOM_H__

#include "uh_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化CTR_DRBG上下文。
 *
 * @return 成功返回CTR_DRBG上下文，失败返回NULL。
 */
extern void * uh_ctr_drbg_create(void);

/**
 * @brief 基于CTR_DRBG生成随机数据。
 *
 * @param void * ctx uh_ctr_drbg_init生成的上下文。
 * @param output 输出的随机数据。
 * @param output_len 输出缓存的大小。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_ctr_drbg_random(void * ctx, uint8_t *output, size_t output_len);

/**
 * @brief 释放CTR_DRBG上下文。
 *
 * @return N/A。
 */
extern void uh_ctr_drbg_free(void * ctx);

#ifdef __cplusplus
}
#endif

#endif /*__UH_RANDOM_H__*/

