/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_BLE_LOG_INTERNAL__H__
#define __UPLUS_BLE_LOG_INTERNAL__H__

#if 0
#ifndef UPLUS_BLE_LOG_COLORS_ENABLE
#define UPLUS_BLE_LOG_COLORS_ENABLE      0
#endif
#endif

#ifndef UPLUS_BLE_LOG_MODULE_NAME
#define UPLUS_BLE_LOG_MODULE_NAME        ""
#endif

#define UPLUS_BLE_LOG_LEVEL_ERROR        1U
#define UPLUS_BLE_LOG_LEVEL_WARNING      2U
#define UPLUS_BLE_LOG_LEVEL_INFO         3U
#define UPLUS_BLE_LOG_LEVEL_DEBUG        4U

#define UPLUS_BLE_LOG_COLOR_DEFAULT      "\x1B[0m"
#define UPLUS_BLE_LOG_COLOR_BLACK        "\x1B[1;30m"
#define UPLUS_BLE_LOG_COLOR_RED          "\x1B[1;31m"
#define UPLUS_BLE_LOG_COLOR_GREEN        "\x1B[1;32m"
#define UPLUS_BLE_LOG_COLOR_YELLOW       "\x1B[1;33m"
#define UPLUS_BLE_LOG_COLOR_BLUE         "\x1B[1;34m"
#define UPLUS_BLE_LOG_COLOR_MAGENTA      "\x1B[1;35m"
#define UPLUS_BLE_LOG_COLOR_CYAN         "\x1B[1;36m"
#define UPLUS_BLE_LOG_COLOR_WHITE        "\x1B[1;37m"

#if UPLUS_BLE_LOG_COLORS_ENABLE
#define UPLUS_BLE_LOG_ERROR_COLOR   UPLUS_BLE_LOG_COLOR_RED
#define UPLUS_BLE_LOG_WARNING_COLOR UPLUS_BLE_LOG_COLOR_YELLOW
#define UPLUS_BLE_LOG_INFO_COLOR    UPLUS_BLE_LOG_COLOR_DEFAULT
#define UPLUS_BLE_LOG_DEBUG_COLOR   UPLUS_BLE_LOG_COLOR_GREEN
#define UPLUS_BLE_LOG_DEFAULT_COLOR UPLUS_BLE_LOG_COLOR_DEFAULT
#else // UPLUS_BLE_LOG_COLORS_ENABLE
#define UPLUS_BLE_LOG_ERROR_COLOR
#define UPLUS_BLE_LOG_WARNING_COLOR
#define UPLUS_BLE_LOG_INFO_COLOR
#define UPLUS_BLE_LOG_DEBUG_COLOR
#define UPLUS_BLE_LOG_DEFAULT_COLOR
#endif // UPLUS_BLE_LOG_COLORS_ENABLE

#define UPLUS_BLE_LOG_BREAK      ": "

#define UPLUS_BLE_ERROR_PREFIX   UPLUS_BLE_LOG_ERROR_COLOR "[E] " UPLUS_BLE_LOG_MODULE_NAME UPLUS_BLE_LOG_BREAK
#define UPLUS_BLE_WARNING_PREFIX UPLUS_BLE_LOG_WARNING_COLOR "[W] " UPLUS_BLE_LOG_MODULE_NAME UPLUS_BLE_LOG_BREAK
#define UPLUS_BLE_INFO_PREFIX    UPLUS_BLE_LOG_INFO_COLOR "[I] " UPLUS_BLE_LOG_MODULE_NAME UPLUS_BLE_LOG_BREAK
#define UPLUS_BLE_DEBUG_PREFIX   UPLUS_BLE_LOG_DEBUG_COLOR "[D] " UPLUS_BLE_LOG_MODULE_NAME UPLUS_BLE_LOG_BREAK

#if !defined(UPLUS_BLE_PRINTF)
    #error "Not defined printf function."
#elif !defined(UPLUS_BLE_HEXDUMP)
    #error "Not defined hexdump function."
#endif

#define UPLUS_BLE_LOG_INTERNAL_ERROR(_fmt_, ...)                                       \
do {                                                                            \
    if (UPLUS_BLE_LOG_LEVEL >= UPLUS_BLE_LOG_LEVEL_ERROR)                                     \
    {                                                                           \
        UPLUS_BLE_PRINTF(UPLUS_BLE_ERROR_PREFIX _fmt_ UPLUS_BLE_LOG_DEFAULT_COLOR, ##__VA_ARGS__);   \
    }                                                                           \
} while(0)

#define UPLUS_BLE_LOG_INTERNAL_WARNING(_fmt_, ...)                                     \
do {                                                                            \
    if (UPLUS_BLE_LOG_LEVEL >= UPLUS_BLE_LOG_LEVEL_WARNING)                                   \
    {                                                                           \
        UPLUS_BLE_PRINTF(UPLUS_BLE_WARNING_PREFIX _fmt_ UPLUS_BLE_LOG_DEFAULT_COLOR, ##__VA_ARGS__); \
    }                                                                           \
} while(0)

#define UPLUS_BLE_LOG_INTERNAL_INFO(_fmt_, ...)                                        \
do {                                                                            \
    if (UPLUS_BLE_LOG_LEVEL >= UPLUS_BLE_LOG_LEVEL_INFO)                                      \
    {                                                                           \
        UPLUS_BLE_PRINTF(UPLUS_BLE_INFO_PREFIX _fmt_ UPLUS_BLE_LOG_DEFAULT_COLOR, ##__VA_ARGS__);    \
    }                                                                           \
} while(0)

#define UPLUS_BLE_LOG_INTERNAL_DEBUG(_fmt_, ...)                                       \
do {                                                                            \
    if (UPLUS_BLE_LOG_LEVEL >= UPLUS_BLE_LOG_LEVEL_DEBUG)                                     \
    {                                                                           \
        UPLUS_BLE_PRINTF(UPLUS_BLE_DEBUG_PREFIX _fmt_ UPLUS_BLE_LOG_DEFAULT_COLOR, ##__VA_ARGS__);   \
    }                                                                           \
} while(0)

#define UPLUS_BLE_LOG_INTERNAL_HEXDUMP(p_data, len)                                    \
do {                                                                            \
    if (UPLUS_BLE_LOG_LEVEL >= UPLUS_BLE_LOG_LEVEL_DEBUG)                                     \
    {                                                                           \
        UPLUS_BLE_HEXDUMP(p_data, len);                                                \
    }                                                                           \
} while(0)

#endif // __UPLUS_BLE_LOG_INTERNAL__H__
