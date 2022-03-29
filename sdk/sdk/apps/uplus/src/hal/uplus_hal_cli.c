/*
 * @Description: file function implementation
 * @Author: jianye
 * @Date: 2021-08-11 14:13:15
 * @LastEditTime: 2021-08-16 11:18:44
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       HalCli  //保持与文件名同步
#define LOG_TAG             "[HalCli]"
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

uint32_t uplus_hal_cli_init(void)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return ret;
}

void uplus_hal_cli_deinit(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}

void uplus_hal_cli_start(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}

void uplus_hal_cli_period(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
}


