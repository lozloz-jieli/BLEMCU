/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_HAL_PWR_MGMT_H__
#define __UPLUS_HAL_PWR_MGMT_H__


#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t uplug_hal_pwr_mgmt_init(void);
extern void uplug_hal_pwr_mgmt_run(void);
extern void uplug_hal_pwr_mgmt_reset(void);
extern void uplug_hal_pwr_mgmt_sleep(void);

#ifdef __cplusplus
}
#endif

#endif

