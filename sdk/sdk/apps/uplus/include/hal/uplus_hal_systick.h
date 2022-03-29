/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_HAL_SYSTICK_H__
#define __UPLUS_HAL_SYSTICK_H__


#ifdef __cplusplus
extern "C" {
#endif

extern void uplus_systick_init(void);
extern void  uplus_systick_deinit(void);
extern uint32_t uplus_systick_uptime_get(void);
extern void uplus_delay_ms(uint32_t ms);            // Blocking delay in us.

#ifdef __cplusplus
}
#endif

#endif

