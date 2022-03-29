/*
 * @Description: 
 * @Author: jianye
 * @Date: 2021-08-11 14:15:50
 * @LastEditTime: 2021-08-16 15:22:08
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       HalRtc  //保持与文件名同步
#define LOG_TAG             "[HalRtc]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"

#define LOG_v(t)  log_tag_const_v_ ## t
#define LOG_i(t)  log_tag_const_i_ ## t
#define LOG_d(t)  log_tag_const_d_ ## t
#define LOG_w(t)  log_tag_const_w_ ## t
#define LOG_e(t)  log_tag_const_e_ ## t


#define LOG_tag(tag, n) n(tag)

const char LOG_tag(LOG_TAG_CONST,LOG_v) AT(.LOG_TAG_CONST) = 0;
const char LOG_tag(LOG_TAG_CONST,LOG_i) AT(.LOG_TAG_CONST) = 1;
const char LOG_tag(LOG_TAG_CONST,LOG_d) AT(.LOG_TAG_CONST) = 1;
const char LOG_tag(LOG_TAG_CONST,LOG_w) AT(.LOG_TAG_CONST) = 1;
const char LOG_tag(LOG_TAG_CONST,LOG_e) AT(.LOG_TAG_CONST) = 1;

#include "app_config.h"
#include "include/includes.h"

void uplus_rtc_init(uplus_rtc_time_mode_e time_mode, uplus_rtc_time_t* p_rtc_init_pram)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}

void uplus_rtc_deinit(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}

void uplus_rtc_time_get(uplus_rtc_time_t* p_rtc_time)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}

void  uplus_rtc_time_set(uplus_rtc_time_t* p_rtc_time)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}


