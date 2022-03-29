/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 14:11:54
 * @LastEditTime: 2021-08-31 10:36:31
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       PalTimer  //保持与文件名同步
#define LOG_TAG             "[PalTimer]"
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

typedef struct _timer_param{
    u16 timer_id;
    uplus_timer_mode_e mode;
    uint32_t timeout_value_ms;
    uplus_timer_handler_t timeout_handler;

}timer_param;
static u32 uplus_timer_param_table[UPLUS_TIMER_END] __attribute__((aligned(4)));

void uplus_clear_timer_id(uplus_timer_id_e timer_id)
{
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[timer_id]);
    ptimer->timer_id = 0;
}

static void timer_cbk(void *para)
{
    u16 id = (int) para;
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[id]);
    //g_printf("%s[id:%d]\n",__func__,id);
    if(ptimer->mode == UPLUS_TIMER_SINGLE_SHOT){
        ptimer->timer_id = 0;
    }
    ptimer->timeout_handler(NULL);
}
int8_t uplus_timer_create(uplus_timer_id_e id, uplus_timer_mode_e mode, uint32_t interval, uplus_timer_handler_t time_handler)
{
    int8_t ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d,handle:0x%x]\n",__func__,id,time_handler);
    if((id >UPLUS_TIMER_END) || (id <UPLUS_POLL_TIMER) || (time_handler == NULL) || (0 == interval)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    uplus_timer_param_table[id] = (u32) malloc(sizeof(timer_param));
    if(uplus_timer_param_table[id] == 0){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Failed to request memory");
        ret =  UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[id]);
    ptimer->timer_id = 0;
    ptimer->mode = mode;
    ptimer->timeout_value_ms = interval;
    ptimer->timeout_handler = time_handler;

param_error:
    return ret;
}

/**
 * @description: 该定时器的步径是10ms
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-16 10:45:23
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_timer_id_e} timer_id
 */
void uplus_timer_start(uplus_timer_id_e timer_id)
{
    int ret = 0;
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[timer_id]);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d,0x%x]\n",__func__,timer_id,uplus_timer_param_table[timer_id]);

    if((timer_id >UPLUS_TIMER_END) || (timer_id <UPLUS_POLL_TIMER)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }



    if(uplus_timer_param_table[timer_id] == 0){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Timer not created");
        ret =  UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    //UPLUS_BLE_LOG_INTERNAL_DEBUG("id:%d,mode:%d,ms:%d,handle:0x%x",ptimer->timer_id,ptimer->mode,ptimer->timeout_value_ms,ptimer->timeout_handler);
    if(ptimer->timer_id){
        r_printf("%s[line:%d],delete timer care!!!\n",__func__,__LINE__);
        if(ptimer->mode == UPLUS_TIMER_SINGLE_SHOT){
            sys_timeout_del(ptimer->timer_id);
        }else{
            sys_timer_del(ptimer->timer_id);
        }
    }

    typedef u16 (*sys_timer_create)(void *priv, void (*func)(void *priv), u32 msec);
    sys_timer_create timer_create = (ptimer->mode == UPLUS_TIMER_SINGLE_SHOT) ? sys_timeout_add : sys_timer_add;
    ptimer->timer_id = timer_create((void *)timer_id,timer_cbk,ptimer->timeout_value_ms);


param_error:
    if(ret != UPLUS_BLE_SUCCESS)
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%d]",__func__,ret);

}

void uplus_timer_stop(uplus_timer_id_e timer_id)
{

    int ret = 0;
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[timer_id]);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d]\n",__func__,timer_id);

    if((timer_id >UPLUS_TIMER_END )|| (timer_id <UPLUS_POLL_TIMER)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(uplus_timer_param_table[timer_id] == 0){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Timer not created");
        ret =  UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }

    if(ptimer->timer_id){
        if(ptimer->mode == UPLUS_TIMER_SINGLE_SHOT){
            sys_timeout_del(ptimer->timer_id);
        }else{
            sys_timer_del(ptimer->timer_id);
        }
        ptimer->timer_id = 0;
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Timer does not exist!!!");
    }



param_error:
    if(ret != UPLUS_BLE_SUCCESS)
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%d]",__func__,ret);
}

void uplus_timer_restart(uplus_timer_id_e timer_id, uint32_t interval_ms)
{
    int ret = 0;
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[timer_id]);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[id:%d,timer:%d]\n",__func__,timer_id,interval_ms);

    if((timer_id >UPLUS_TIMER_END) || (timer_id <UPLUS_POLL_TIMER)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(uplus_timer_param_table[timer_id] == 0){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Timer not created");
        ret =  UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }

    if(ptimer->mode == UPLUS_TIMER_REPEATED){
    if(ptimer->timer_id == 0)
    {
        //changed by yuanlf 2021-10-14
        ptimer->timeout_value_ms = interval_ms;
        uplus_timer_start(timer_id);
    }
    else{

        //changed by yuanlf 2021-10-14
            // uplus_timer_stop(timer_id);
            ptimer->timeout_value_ms = interval_ms;
            sys_timer_modify(ptimer->timer_id, ptimer->timeout_value_ms);
        }
    }else if(ptimer->mode == UPLUS_TIMER_SINGLE_SHOT){
        ptimer->timeout_value_ms = interval_ms;
        uplus_timer_start(timer_id);
        //sys_timer_modify(ptimer->timer_id, ptimer->timeout_value_ms);
        //sys_timer_re_run(ptimer->timer_id);
    }


param_error:
    if(ret != UPLUS_BLE_SUCCESS)
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%d]",__func__,ret);
}

/**
 * @description: 用户可修改获返回结果
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-16 11:03:13
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_timer_id_e} timer_id
 */
int8_t uplus_timer_is_active(uplus_timer_id_e timer_id)
{
    int ret = 0;
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[timer_id]);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d]\n",__func__,timer_id);

    if((timer_id >UPLUS_TIMER_END )|| (timer_id <UPLUS_POLL_TIMER)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(uplus_timer_param_table[timer_id] == 0){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Timer not created");
        ret =  UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    printf("[timer id:%d mode:%d internal:%d handle:%x]",ptimer->timer_id,ptimer->mode,ptimer->timeout_value_ms,ptimer->timeout_handler);

    if(ptimer->timer_id == 0)
    {
        ret = 1;
    }
param_error:
    return ret?0:1;
}

void uplus_timer_delete(uplus_timer_id_e timer_id)
{
    int ret = 0;
    timer_param *ptimer = (timer_param *)(uplus_timer_param_table[timer_id]);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d]\n",__func__,timer_id);

    if((timer_id >UPLUS_TIMER_END )|| (timer_id <UPLUS_POLL_TIMER)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(uplus_timer_param_table[timer_id] == 0){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Timer not created");
        ret =  UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }

    if(ptimer->timer_id){
        if(ptimer->mode == UPLUS_TIMER_SINGLE_SHOT){
            sys_timeout_del(ptimer->timer_id);
        }else{
            sys_timer_del(ptimer->timer_id);
        }
    }

    free((void *)uplus_timer_param_table[timer_id]);
    uplus_timer_param_table[timer_id] = 0;

param_error:
    if(ret != UPLUS_BLE_SUCCESS)
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%d]",__func__,ret);
}
