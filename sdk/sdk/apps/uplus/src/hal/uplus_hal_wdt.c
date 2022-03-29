/*
 * @Description: 
 * @Author: jianye
 * @Date: 2021-08-11 14:16:49
 * @LastEditTime: 2021-08-24 16:10:06
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       HalWdt  //保持与文件名同步
#define LOG_TAG             "[HalWdt]"
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
#include "asm/wdt.h"
#include "include/includes.h"

/**
 * @description: 看门狗的初始化时间根据档位传值，具体参考wdt.h
 * 默认看门狗在setup.c文件已开启
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-17 09:36:18
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint32_t} period
 */
void uplug_hal_wdt_init(uint32_t period)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d]\n",__func__,period);
    wdt_init(period);
}
void uplug_hal_wdt_deinit(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    wdt_close();
}
void uplug_hal_wdt_feed(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    wdt_clear();
}