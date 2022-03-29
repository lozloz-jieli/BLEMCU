/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 11:09:12
 * @LastEditTime: 2021-08-30 18:50:45
 * @LastEditors: jianye
 */
/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_HAL_UART_H__
#define __UPLUS_HAL_UART_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "app_config.h"
#include "include/includes.h"
typedef enum
{
    UPLUS_UART_BAUDRATE_9600,
    UPLUS_UART_BAUDRATE_19200,
    UPLUS_UART_BAUDRATE_38400,
    UPLUS_UART_BAUDRATE_115200
}uplus_uart_baudrate_e;

typedef enum
{
    UPLUS_UART_DATABIT_5,
    UPLUS_UART_DATABIT_6,
    UPLUS_UART_DATABIT_7,
    UPLUS_UART_DATABIT_8
}uplus_uart_databit_e;

typedef enum
{
    UPLUS_UART_STOPBIT_1,
    UPLUS_UART_STOPBIT_1_5,
    UPLUS_UART_STOPBIT_2
}uplus_uart_stopbit_e;

typedef enum
{
    UPLUS_UART_PARITY_ODD,
    UPLUS_UART_PARITY_EVEN,
    UPLUS_UART_PARITY_NO
}uplus_uart_parity_e;

typedef enum
{
    UPLUS_UART_HWFLOW_ENABLE,
    UPLUS_UART_HWFLOW_DISABLE
}uplus_uart_hwflow_ctrl_e;

typedef struct{
	uint8_t                     uart_id;
	uplus_uart_baudrate_e       baudrate;
	uplus_uart_databit_e        databit;
	uplus_uart_stopbit_e        stopbit;
	uplus_uart_parity_e         parity;
	uplus_uart_hwflow_ctrl_e    hwflow_ctl;
}uplus_uart_t;

typedef struct{
	uint8_t    uart_id;
	uint8_t*   data_buff;
	uint16_t   data_len;
}uplus_uart_data_t;

typedef  void(* uplus_uart_handler_t )( uplus_uart_data_t*  p_uart_data_pram);

extern void uplug_uart_init(uplus_uart_t* p_uart_init_pram);
extern void uplus_uart_deinit(uint8_t uart_id);
extern void uplus_uart_regester(uplus_uart_handler_t uart_handler);
extern void uplus_uart_tx(uplus_uart_data_t* p_uart_tx_data_pram);
extern bool uplus_uart_tx_in_progress(uint8_t uart_id);
extern void	uplus_uart_rx(uplus_uart_data_t* p_uart_rx_data_pram);
extern void uplus_uart_rx_enable(uint8_t uart_id);
extern void uplus_uart_rx_disable(uint8_t uart_id);

#ifdef __cplusplus
}
#endif

#endif

