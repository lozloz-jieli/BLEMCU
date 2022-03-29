/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef UPLUS_BLE_PORT_H__
#define UPLUS_BLE_PORT_H__


#ifndef NULL
#define NULL 0
#endif
#if 0
#if defined(__CC_ARM)
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language = extended
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#endif

#if defined ( __CC_ARM )

    #ifndef __ASM
        #define __ASM               __asm
    #endif

    #ifndef __INLINE
        #define __INLINE            __inline
    #endif

    #ifndef __WEAK
        #define __WEAK              __weak
    #endif

    #ifndef __ALIGN
        #define __ALIGN(n)          __align(n)
    #endif

    #ifndef __PACKED
        #define __PACKED            __packed
    #endif

    #define GET_SP()                __current_sp()

#elif defined ( __ICCARM__ )

    #ifndef __ASM
        #define __ASM               __asm
    #endif

    #ifndef __INLINE
        #define __INLINE            inline
    #endif

    #ifndef __WEAK
        #define __WEAK              __weak
    #endif

    #ifndef __ALIGN
        #define STRING_PRAGMA(x) _Pragma(#x)
        #define __ALIGN(n) STRING_PRAGMA(data_alignment = n)
    #endif

    #ifndef __PACKED
        #define __PACKED            __packed
    #endif

    #define GET_SP()                __get_SP()

#elif defined   ( __GNUC__ )

    #ifndef __ASM
        #define __ASM               __asm
    #endif

    #ifndef __INLINE
        #define __INLINE            inline
    #endif

    #ifndef __WEAK
        #define __WEAK              __attribute__((weak))
    #endif

    #ifndef __ALIGN
        #define __ALIGN(n)          __attribute__((aligned(n)))
    #endif

    #ifndef __PACKED
        #define __PACKED           __attribute__((packed))
    #endif

    #define GET_SP()                gcc_current_sp()

    static inline unsigned int gcc_current_sp(void)
    {
        register unsigned sp __ASM("sp");
        return sp;
    }
#endif
//#include "hal_mcu.h"//Telink Del
#define CRITICAL_SECTION_ENTER() //HAL_ENTER_CRITICAL_SECTION()
#define CRITICAL_SECTION_EXIT() //HAL_EXIT_CRITICAL_SECTION()
#endif
//int my_printf_uart_hexdump(char *p_buf,int len );
//int my_printf_uart(const char *format,...);


#define LOG_CLI_ENABLE
#include "debug.h"

#define UPLUS_BLE_LOG_ENABLED

#ifdef UPLUS_BLE_LOG_ENABLED
#ifdef P_TELINK_8258
#include "log.h"
#define UPLUS_BLE_PRINTF(...)                       my_printf_uart
#define UPLUS_BLE_HEXDUMP(...)                      my_printf_uart_hexdump
#endif

#ifdef P_BK_3431
int uart_printf(const char *fmt,...);
void uart_hexdump(const unsigned char *buf, int len);
#define UPLUS_BLE_PRINTF(...)			            uart_printf(__VA_ARGS__)
#define UPLUS_BLE_HEXDUMP(base_addr, bytes)         uart_hexdump(base_addr, bytes)
#endif

#ifdef P_JL_AC632N

#define UPLUS_BLE_PRINTF    			            log_info
#define UPLUS_BLE_HEXDUMP                           put_buf
#endif

#else
#define UPLUS_BLE_PRINTF(...)
#define UPLUS_BLE_HEXDUMP(...)
#endif

//#include "cmsis_compiler.h"

#ifdef DEBUG
#include "nrf_gpio.h"

#define TRACE_INIT(x)                                                           \
do {                                                                            \
    nrf_gpio_cfg_output(x);                                                     \
    nrf_gpio_pin_clear(x);                                                      \
} while(0)
#define TRACE_ENTER(x) do {nrf_gpio_pin_set(x);} while(0)
#define TRACE_EXIT(x)  do {nrf_gpio_pin_clear(x);} while(0)
#else
#define TRACE_INIT(x)
#define TRACE_ENTER(x)
#define TRACE_EXIT(x)
#endif

#endif // UPLUS_BLE_PORT_H__
