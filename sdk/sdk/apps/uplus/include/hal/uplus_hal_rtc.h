/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_HAL_RTC_H__
#define __UPLUS_HAL_RTC_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UPLUS_TIME_MODE_12,
    UPLUS_TIME_MODE_24
}uplus_rtc_time_mode_e;

typedef struct
{
    uint8_t   sec;
    uint8_t   min;
    uint8_t   hour;
    uint8_t   am_or_pm;//0:am,1:pm
    uint8_t   day;
    uint8_t   month;
    uint16_t  year;
    uint8_t   week;
}uplus_rtc_time_t;

extern void  uplus_rtc_init(uplus_rtc_time_mode_e time_mode, uplus_rtc_time_t* p_rtc_init_pram);
extern void  uplus_rtc_deinit(void);
extern void  uplus_rtc_time_get(uplus_rtc_time_t* p_rtc_time);
extern void  uplus_rtc_time_set(uplus_rtc_time_t* p_rtc_time);

#ifdef __cplusplus
}
#endif

#endif

