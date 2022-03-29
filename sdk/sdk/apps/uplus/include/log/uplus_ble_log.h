/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_BLE_LOG_H__
#define __UPLUS_BLE_LOG_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "uplus_ble_port.h"

#define UPLUS_BLE_LOG_ENABLED
#ifdef UPLUS_BLE_LOG_ENABLED
/**
 * Log error   level    :1
 * Log warning level    :2
 * Log info    level    :3
 * Log debug   level    :4
 */

#define UPLUS_BLE_LOG_LEVEL              4
#define UPLUS_BLE_LOG_COLORS_ENABLE      1

#include "uplus_ble_log_internal.h"

#define UPLUS_BLE_LOG_HEXDUMP(...)						UPLUS_BLE_LOG_INTERNAL_HEXDUMP( __VA_ARGS__)
#define UPLUS_BLE_LOG_ERROR(...)                      	UPLUS_BLE_LOG_INTERNAL_ERROR(__VA_ARGS__)
#define UPLUS_BLE_LOG_WARNING(...)                   	UPLUS_BLE_LOG_INTERNAL_WARNING( __VA_ARGS__)
#define UPLUS_BLE_LOG_INFO(...)                        	UPLUS_BLE_LOG_INTERNAL_INFO( __VA_ARGS__)
#define UPLUS_BLE_LOG_DEBUG(...)                       	//UPLUS_BLE_LOG_INTERNAL_DEBUG( __VA_ARGS__)


#else // UPLUS_BLE_LOG_ENABLED
#define UPLUS_BLE_LOG_ERROR(...)
#define UPLUS_BLE_LOG_WARNING(...)
#define UPLUS_BLE_LOG_INFO(...)
#define UPLUS_BLE_LOG_DEBUG(...)
#define UPLUS_BLE_LOG_HEXDUMP(...)
#endif // UPLUS_BLE_LOG_ENABLED


#ifdef UPLUS_BLE_ASSERT

#define UPLUS_BLE_ERR_HANDLER(ERR_CODE)                                       \
    do                                                                 \
    {                                                                  \
        UPLUS_BLE_LOG_ERROR("Error code 0x%04X <%d>.  %s:%d\n", ERR_CODE, ERR_CODE, (uint32_t)__FILE__, __LINE__); \
    } while (0)

#define UPLUS_BLE_ERR_CHECK(ERR_CODE)                              \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != 0)                            \
        {                                                   \
            UPLUS_BLE_ERR_HANDLER(LOCAL_ERR_CODE);                 \
        }                                                   \
    } while (0)

#define UPLUS_BLE_ERR_TEST(ERR_CODE, EXPECT)                       \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != (EXPECT))                     \
        {                                                   \
            UPLUS_BLE_ERR_HANDLER(LOCAL_ERR_CODE);                 \
        }                                                   \
    } while (0)
#else // UPLUS_BLE_ASSERT

#define UPLUS_BLE_ERR_CHECK(ERR_CODE)
#define UPLUS_BLE_ERR_TEST(ERR_CODE, EXPECT)

#endif // UPLUS_BLE_ASSERT END

#endif // __UPLUS_BLE_LOG_H__
