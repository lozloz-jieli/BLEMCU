/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 14:12:27
 * @LastEditTime: 2021-08-24 16:10:16
 * @LastEditors: jianye
 */


#define LOG_TAG_CONST       PalSys  //保持与文件名同步
#define LOG_TAG             "[PalSys]"
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
#include "generic/jiffies.h"
#include "include/includes.h"

uint32_t uplus_sys_uptime_get(void)
{
    //UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return jiffies_msec();
}


uint32_t uplus_sys_uptime_delta(uint32_t now, uint32_t ref_time)
{

    if(now < ref_time){
        //UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return UPLUS_BLE_ERR_INVALID_PARAM;
    }else{

        //UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d], delta[%d]\n",__func__,now, now - ref_time);
        return (now - ref_time);
    }
}

void uplus_sys_reboot(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    cpu_reset();
}
