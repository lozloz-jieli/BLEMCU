/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UH_SYSTICK_H
#define __UH_SYSTICK_H

#include "uplus_type.h"
#include "uplus_hal_systick.h"

extern void uh_systick_init(void);
extern uint32_t uh_systick_uptime_get(void);
extern void uh_delay_ms(uint32_t);
#endif
