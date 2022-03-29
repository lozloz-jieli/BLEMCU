/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 11:09:12
 * @LastEditTime: 2021-08-31 10:03:17
 * @LastEditors: jianye
 */
/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_PAL_TIMER_H__
#define __UPLUS_PAL_TIMER_H__


#define TIME_MILLISECOND 	(1)
#define TIME_SECOND 		(1000 * (TIME_MILLISECOND))
#define TIME_MINUTE 		(60 * (TIME_SECOND))
#define TIME_HOUR 			(60 * (TIME_MINUTE))
#define TIME_DAY 			(24 * (TIME_HOUR))

/**
 * \brief
 */
typedef enum {
	UPLUS_TIMER_SINGLE_SHOT,
	UPLUS_TIMER_REPEATED
} uplus_timer_mode_e;

/**
 * \brief
 */

typedef enum {
    UPLUS_POLL_TIMER = 1,
    UPLUS_BINDCONFIRM_TIMER,
	UPLUS_CONNECT_TIMER,
	UPLUS_TS_REQ_TIMER,
	UPLUS_REFRESH_ADVERTISE_INTERVER_TIMER,
	UPLUS_HIST_RECORD_TIMER,
	UPLUS_HIST_READ_RECORD_OUTTIMER_TIMER,
	UPLUS_COMMON_TIMER,
	UPLUS_WORKER_TIMER,
	UPLUS_SEND_NOTIFY_TIMER,
	UPLUS_BRD_RECOVER_TIMER,
	UPLUS_OTA_TIMER,
	UPLUS_USER_CONFIG_SAVE_TIMER,
	UPLUS_USER_RSV1_TIMER,
	UPLUS_USER_RSV2_TIMER,
	UPLUS_USER_RSV3_TIMER,
	UPLUS_TIMER_INVALID,
	UPLUS_TIMER_USER,
	UPLUS_TIMER_USER1,
	UPLUS_TIMER_USER2,
	UPLUS_TIMER_USER3,
	UPLUS_TIMER_END,
} uplus_timer_id_e;

/**
 * \brief
 */
typedef void(*uplus_timer_handler_t)(void *);

/**
 * \brief
 */
int8_t uplus_timer_create(uplus_timer_id_e id, uplus_timer_mode_e mode, uint32_t interval, uplus_timer_handler_t time_handler);

/**s
 * \brief
 */
void uplus_timer_start(uplus_timer_id_e timer_id);

/**
 * \brief
 */
void uplus_timer_stop(uplus_timer_id_e timer_id);

/**
 * \brief
 */
void uplus_timer_restart(uplus_timer_id_e timer_id, uint32_t interval_ms);

/**
 * \brief
 */
int8_t uplus_timer_is_active(uplus_timer_id_e timer_id);

/**
 * \brief
 */
void uplus_timer_delete(uplus_timer_id_e timer_id);
void uplus_clear_timer_id(uplus_timer_id_e timer_id);
#endif

