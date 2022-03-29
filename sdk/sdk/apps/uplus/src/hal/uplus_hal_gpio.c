/*
 * @Description: 没有适配IO中断
 * @Author: jianye
 * @Date: 2021-08-11 14:15:05
 * @LastEditTime: 2021-08-31 11:30:43
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       HalGpio  //保持与文件名同步
#define LOG_TAG             "[HalGpio]"
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

static u32 uplus_gpio_mapp(uplus_pin_t* p_pin)
{
    u32 JL_gpio_pin = 0;
    switch (p_pin->pin_port_id)
    {
    case 'A':
        JL_gpio_pin = IO_PORTA_00 + p_pin->pin_id;
        break;
    case 'B':
        JL_gpio_pin = IO_PORTB_00 + p_pin->pin_id;
        break;
    case 'D':
        JL_gpio_pin = IO_PORTD_00 + p_pin->pin_id;
        break;
    default:

        break;
    }
    return JL_gpio_pin;
}


void uplus_gpio_init(uplus_gpio_t* p_gpio_init_pram)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,p_gpio_init_pram);
    if(p_gpio_init_pram == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    
}

void uplus_gpio_deinit(uplus_pin_t* p_pin)
{
    u32 JL_opt_pin = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,p_pin);
    if(p_pin == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    JL_opt_pin = uplus_gpio_mapp(p_pin);
    gpio_set_pull_up(JL_opt_pin, 0);
    gpio_set_pull_down(JL_opt_pin, 0);
    gpio_set_direction(JL_opt_pin, 1);
    gpio_set_die(JL_opt_pin, 0);
    gpio_set_dieh(JL_opt_pin, 0);
}

/**
 * @description: IO中断没有实现，
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-16 14:38:02
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_gpio_handler_t} gpio_handler
 */
void uplus_gpio_register(uplus_gpio_handler_t gpio_handler)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,gpio_handler);
    if(gpio_handler == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
}


void uplus_gpio_set_input(uplus_pin_t* p_pin, uplus_gpio_pull_mode_e pull_mode)
{
    u32 JL_opt_pin = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    if(p_pin == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    JL_opt_pin = uplus_gpio_mapp(p_pin);
    switch (pull_mode)
    {
    case UPLUS_GPIO_PULL_UP:
        gpio_set_pull_down(JL_opt_pin, 0);
        gpio_set_pull_up(JL_opt_pin, 1);
        break;
    case UPLUS_GPIO_PULL_DOWN:
        gpio_set_pull_down(JL_opt_pin, 1);
        gpio_set_pull_up(JL_opt_pin, 0);
        break;
        
    case UPLUS_GPIO_PULL_NO:
        gpio_set_pull_down(JL_opt_pin, 0);
        gpio_set_pull_up(JL_opt_pin, 0);
        break;
    default:
        break;
    }
    gpio_direction_input(JL_opt_pin);
    gpio_set_die(JL_opt_pin, 1);
    return;
}

void uplus_gpio_set_output(uplus_pin_t* p_pin, uplus_gpio_pull_mode_e pull_mode)
{
    u32 JL_opt_pin = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    if(p_pin == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    JL_opt_pin = uplus_gpio_mapp(p_pin);
    switch (pull_mode)
    {
    case UPLUS_GPIO_PULL_UP:
        gpio_set_pull_down(JL_opt_pin, 0);
        gpio_set_pull_up(JL_opt_pin, 1);
        break;
    case UPLUS_GPIO_PULL_DOWN:
        gpio_set_pull_down(JL_opt_pin, 1);
        gpio_set_pull_up(JL_opt_pin, 0);
        break;
        
    case UPLUS_GPIO_PULL_NO:
        gpio_set_pull_down(JL_opt_pin, 0);
        gpio_set_pull_up(JL_opt_pin, 0);
        break;
    default:
        break;
    }
    return;
}

void uplus_gpio_out_set(uplus_pin_t* p_pin, uint8_t value)
{
    u32 JL_opt_pin = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    if(p_pin == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    JL_opt_pin = uplus_gpio_mapp(p_pin);
    gpio_direction_output(JL_opt_pin, value);
    
}

uint8_t uplus_gpio_in_get(uplus_pin_t* p_pin)
{
    u32 JL_opt_pin = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    if(p_pin == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return 0;
    }
    JL_opt_pin = uplus_gpio_mapp(p_pin);
    return gpio_read(JL_opt_pin);
}

void test_uplus_gpio(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    uplus_pin_t input_pin;
    input_pin.pin_id = 2;
    input_pin.pin_port_id = 'A';
    uplus_gpio_set_input(&input_pin,UPLUS_GPIO_PULL_UP);

    uplus_pin_t output_pin;
    output_pin.pin_id = 3;
    output_pin.pin_port_id = 'A';
    uplus_gpio_set_output(&output_pin,UPLUS_GPIO_PULL_NO);
    uplus_gpio_out_set(&output_pin,0);

}




