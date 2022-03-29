/*
 * @Description: 默认uart diver适配为8位数据位，1位停止位
 * @Author: jianye
 * @Date: 2021-08-11 14:16:27
 * @LastEditTime: 2021-08-30 21:32:10
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       HalUart  //保持与文件名同步
#define LOG_TAG             "[HalUart]"
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

#define UPLUS_UART_MAX_NUM      1   //uplus 使用的最大串口数量

static uint32_t uplus_uartid_table_mapp[UPLUS_UART_MAX_NUM] = {0};
static uint8_t uplus_uart_numbers = 0;

/**
 * @description:
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-16 19:40:11
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint32_t} uart_bus
 */
void set_uartid_table(uint32_t uart_bus,u8 uartid)
{
    uplus_uartid_table_mapp[uartid] = uart_bus;
}

u32 uplus_uart_baud_mmap(u8 type)
{
    u32 ret_baud = 0;
    switch (type)
    {
    case UPLUS_UART_BAUDRATE_9600:
        ret_baud = 9600;
        break;
    case UPLUS_UART_BAUDRATE_19200:
        ret_baud = 19200;
        break;

    case UPLUS_UART_BAUDRATE_38400:
        ret_baud = 38400;
        break;
    case UPLUS_UART_BAUDRATE_115200:
        ret_baud = 115200;
        break;

    default:
        break;
    }
    return ret_baud;
}


extern bool uart_dev_test_init_api(u32 baud,u8 uartid);
/**
 * @description: uart_id 需要从0开始
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-16 19:39:35
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_uart_t*} p_uart_init_pram
 */
void uplug_uart_init(uplus_uart_t* p_uart_init_pram)
{
    u32 uart_baud = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[baud:%d]\n",__func__,p_uart_init_pram->baudrate);
    if(p_uart_init_pram == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    if(uplus_uart_numbers >=  UPLUS_UART_MAX_NUM){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_RESOURCES");
        return;
    }
    uplus_uart_numbers ++;

    uart_baud = uplus_uart_baud_mmap(p_uart_init_pram->baudrate);
    bool ret = uart_dev_test_init_api(uart_baud,p_uart_init_pram->uart_id);
    if(ret){
        uplus_uart_numbers++;
    }

}
void uart_no_rx_income_timeout(void *p);

void uplus_uart_deinit(uint8_t uart_id)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[id:%d]\n",__func__,uart_id);
    if(uplus_uart_numbers <= uart_id){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }

    uart_no_rx_income_timeout((void *)(uplus_uartid_table_mapp[uart_id]));
    uplus_uartid_table_mapp[uart_id] = 0;
    uplus_uart_numbers --;
}

uplus_uart_handler_t uart_receive_data = NULL;
void uplus_uart_regester(uplus_uart_handler_t uart_handler)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,uart_handler);
    if(uart_handler == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    uart_receive_data = uart_handler;
}

extern u16 uart_dev_test_send_api(void *data, u16 len);
void uplus_uart_tx(uplus_uart_data_t* p_uart_tx_data_pram)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,p_uart_tx_data_pram);
    if(p_uart_tx_data_pram == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }

    uart_dev_test_send_api(p_uart_tx_data_pram->data_buff,p_uart_tx_data_pram->data_len);

}

/**
 * @description:
 * @param  {*}
 * @return {*}0：空闲   1：占用
 * @Author: jianye
 * @Date: 2021-08-16 20:29:14
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint8_t} uart_id
 */
bool uplus_uart_tx_in_progress(uint8_t uart_id)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[id:%d]\n",__func__,uart_id);
    if(uplus_uart_numbers <= uart_id){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return 1;
    }

    u16 uart_dev_busy_api(u8 uartid);
    return uart_dev_busy_api(uart_id);
}

void uplus_uart_rx(uplus_uart_data_t* p_uart_rx_data_pram)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,p_uart_rx_data_pram);
    if(p_uart_rx_data_pram == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
}


void uplus_uart_rx_enable(uint8_t uart_id)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[id:%d]\n",__func__,uart_id);
    if(uplus_uart_numbers <= uart_id){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return ;
    }
    extern void uart_dev_tx_enable_api(u8 uartid);
    uart_dev_tx_enable_api(uart_id);
}
void uplus_uart_rx_disable(uint8_t uart_id)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[id:%d]\n",__func__,uart_id);
    if(uplus_uart_numbers <= uart_id){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return;
    }
    extern void uart_dev_tx_disable_api(u8 uartid);
    uart_dev_tx_disable_api(uart_id);
}
