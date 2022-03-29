/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 14:16:08
 * @LastEditTime: 2021-08-17 14:18:13
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       HalTick  //保持与文件名同步
#define LOG_TAG             "[HalTick]"
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

extern unsigned long jiffies_msec();

void uplus_systick_init(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}

void  uplus_systick_deinit(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}

uint32_t uplus_systick_uptime_get(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return jiffies_msec();
}

void uplus_delay_ms(uint32_t ms)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s:[%dms]\n",__func__, ms);
    if(ms < 10){
        ms = 10;
    }
    os_time_dly(ms/10);
}
