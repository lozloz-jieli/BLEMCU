/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 16:15:55
 * @LastEditTime: 2021-08-30 19:10:36
 * @LastEditors: jianye
 */

#include "system/includes.h"
#include "asm/includes.h"


#define UPLUS_BLE_PRINTF   log_info  ///printf
#define UPLUS_BLE_HEXDUMP  put_buf

#include "include/user_config.h"
#include "include/uplus_typedef.h"
#include "include/pal/uplus_type.h"
#include "include/pal/uplus_ble_type.h"

#include "include/log/uplus_ble_port.h"
#include "include/log/uplus_ble_log.h"
#include "include/log/uplus_ble_log_internal.h"
#include "include/log/uplus_ble_trace.h"

#include "include/pal/uplus_pal_ble.h"
#include "include/pal/uplus_pal_def.h"
#include "include/pal/uplus_pal_sys.h"
#include "include/pal/uplus_pal_timer.h"

#include "include/hal/uplus_hal_cli.h"
#include "include/hal/uplus_hal_flash.h"
#include "include/hal/uplus_hal_gpio.h"
#include "include/hal/uplus_hal_pwr_mgmt.h"
#include "include/hal/uplus_hal_rtc.h"
#include "include/hal/uplus_hal_systick.h"
#include "include/hal/uplus_hal_uart.h"
#include "include/hal/uplus_hal_wdt.h"

#include "include/api/uplus_ble_sdk_api.h"
#include "include/uhome/uh_bluetooth.h"
#include "include/uhome/uh_record_api.h"

extern int uplus_main(void);
extern int haier_main(void);

