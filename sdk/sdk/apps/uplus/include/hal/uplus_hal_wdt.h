/*
 * @Description: 
 * @Author: jianye
 * @Date: 2021-08-11 11:09:12
 * @LastEditTime: 2021-08-28 16:29:34
 * @LastEditors: jianye
 */
/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_HAL_WDT_H__
#define __UPLUS_HAL_WDT_H__

#ifdef __cplusplus
extern "C" {
#endif

extern void uplug_hal_wdt_init(uint32_t period);
extern void uplug_hal_wdt_deinit(void);
extern void uplug_hal_wdt_feed(void);

#ifdef __cplusplus
}
#endif

#endif

