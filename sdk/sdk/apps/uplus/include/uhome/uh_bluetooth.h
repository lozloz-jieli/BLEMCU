/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */
/**
 * @file uh_bluetooth.h
 * @brief
 *
 * @date 2017-12-22
 * @author fanming
 *
 */

#ifndef __UH_BLUETOOTH_H__
#define __UH_BLUETOOTH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "include/includes.h"
/**
 *  @brief Data types used for the advertising data, please refer to <a href="https://www.bluetooth.org/en-us/specification/assigned-numbers/generic-access-profile">Assigned number for the Generic Access Profile</a>.
 */
#define UH_BT_GAP_LE_AD_TYPE_FLAG                     0x01 /**< Flags */
#define UH_BT_GAP_LE_AD_TYPE_16_BIT_UUID_PART         0x02 /**< Incomplete list of the 16-bit service class UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_16_BIT_UUID_COMPLETE     0x03 /**< Complete list of the 16-bit service class UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_32_BIT_UUID_PART         0x04 /**< Incomplete list of the 32-bit service class UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_32_BIT_UUID_COMPLETE     0x05 /**< Complete list of the 32-bit service class UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_128_BIT_UUID_PART        0x06 /**< Incomplete list of the 128-bit service class UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_128_BIT_UUID_COMPLETE    0x07 /**< Complete list of the 128-bit service class UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_NAME_SHORT               0x08 /**< Shortened local name. */
#define UH_BT_GAP_LE_AD_TYPE_NAME_COMPLETE            0x09 /**< Complete local name. */
#define UH_BT_GAP_LE_AD_TYPE_TX_POWER                 0x0A /**< Transmit power level. */
#define UH_BT_GAP_LE_AD_TYPE_COD                      0x0D /**< Class of the device. */
#define UH_BT_GAP_LE_AD_TYPE_SM_TK                    0x10 /**< Security manager's TK value. */
#define UH_BT_GAP_LE_AD_TYPE_SM_OOB_FLAG              0x11 /**< Security manager's Out-of-Band flags. */
#define UH_BT_GAP_LE_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE  0x12 /**< Slave connection interval range. */
#define UH_BT_GAP_LE_AD_TYPE_16_BIT_SOLICITATION_UUID 0x14 /**< List of the 16-bit service solicitation UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_128_BIT_SOLICITATION_UUID        0x15 /**< List of the 128-bit service solicitation UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_SERVICE_DATA             0x16 /**< Service data. */
#define UH_BT_GAP_LE_AD_TYPE_16_BIT_UUID_DATA         0x16 /**< Service data with 16-bit UUID. */
#define UH_BT_GAP_LE_AD_TYPE_PUBLIC_TARGET_ADDRESS    0x17 /**< Public target address. */
#define UH_BT_GAP_LE_AD_TYPE_RANDOM_TARGET_ADDRESS    0x18 /**< Random target address. */
#define UH_BT_GAP_LE_AD_TYPE_APPEARANCE               0x19 /**< Appearance. */
#define UH_BT_GAP_LE_AD_TYPE_ADV_INTERVAL             0x1A /**< Advertising interval. */
#define UH_BT_GAP_LE_AD_TYPE_LE_BT_DEVICE_ADDRESS     0x1B /**< Bluetooth LE device address. */
#define UH_BT_GAP_LE_AD_TYPE_LE_ROLE                  0x1C /**< LE role. */
#define UH_BT_GAP_LE_AD_TYPE_32_BIT_SOLICITATION_UUID 0x1F /**< List of 32-bit service solicitation UUIDs. */
#define UH_BT_GAP_LE_AD_TYPE_32_BIT_UUID_DATA         0x20 /**< Service data - 32-bit UUID. */
#define UH_BT_GAP_LE_AD_TYPE_128_BIT_UUID_DATA        0x21 /**< Service data - 128-bit UUID. */
#define UH_BT_GAP_LE_AD_TYPE_MANUFACTURER_SPECIFIC    0xFF /**< Manufacturer specific data. */

#define UH_BT_GAP_LE_AD_FLAG_LIMITED_DISCOVERABLE     (0x01 << 0) /**< LE limited discoverable mode. */
#define UH_BT_GAP_LE_AD_FLAG_GENERAL_DISCOVERABLE     (0x01 << 1) /**< LE general discoverable mode. */
#define UH_BT_GAP_LE_AD_FLAG_BR_EDR_NOT_SUPPORTED     (0x01 << 2) /**< BR/EDR not supported. */

#define UH_BT_GAP_LE_DIRECT_ADDR_TYPE_INVALID         0           /**< For #bt_gap_le_advertising_report_ind_t. */

#define UH_BT_GAP_LE_SECURITY_UNAUTH_MASK             0x0         /**< The key is unauthenticated. */
#define UH_BT_GAP_LE_SECURITY_AUTHORIZATION_MASK      (0x1 << 0)  /**< The key is authorized. */
#define UH_BT_GAP_LE_SECURITY_AUTHENTICATION_MASK     (0x1 << 1)  /**< The key is authenticated. */
#define UH_BT_GAP_LE_SECURITY_ENCRYPTION_MASK         (0x1 << 2)  /**< The LTK is available. */
#define UH_BT_GAP_LE_SECURITY_SIGN_MASK               (0x1 << 3)  /**< The CSRK is available. */
#define UH_BT_GAP_LE_SECURITY_LESC_MASK               (0x1 << 4)  /**< Use LE secure connection. */
#define UH_BT_GAP_LE_SECURITY_BONDED_MASK             (0x1 << 5)  /**< Bonded mask. */

#define UH_BT_GAP_LE_CLEAR_WHITE_LIST         0x2010                  /**< Clear the white list. */
#define UH_BT_GAP_LE_ADD_TO_WHITE_LIST        0x2011                  /**< Add to the white list. */
#define UH_BT_GAP_LE_REMOVE_FROM_WHITE_LIST   0x2012                  /**< Remove from the white list. */

/**
 * @addtogroup BluetoothBLE_ATT_GATT_define Define
 * @{
 */

/* UUID */
#define UH_BT_GATT_UUID16_PRIMARY_SERVICE                              0x2800      /**< Primary service. */
#define UH_BT_GATT_UUID16_SECONDARY_SERVICE                            0x2801      /**< Secondary service. */
#define UH_BT_GATT_UUID16_INCLUDE                                      0x2802      /**< Include service. */
#define UH_BT_GATT_UUID16_CHARC                               0x2803      /**< Characteristic. */
#define UH_BT_GATT_UUID16_CHARC_EXTENDED_PROPERTIES           0x2900      /**< Characteristic extended properties. */
#define UH_BT_GATT_UUID16_CHARC_USER_DESCRIPTION              0x2901      /**< Characteristic user description. */
#define UH_BT_GATT_UUID16_CLIENT_CHARC_CONFIG                 0x2902      /**< Client characteristic configuration. */
#define UH_BT_GATT_UUID16_SERVER_CHARC_CONFIG                 0x2903      /**< Server characteristic configuration. */
#define UH_BT_GATT_UUID16_CHARC_FORMAT                        0x2904      /**< Characteristic format. */
#define UH_BT_GATT_UUID16_CHARC_AGGREGATE_FORMAT              0x2905      /**< Characteristic aggregate format. */

#define UH_BT_GATT_UUID16_GAP_SERVICE                                  0x1800      /**< GAP service. */
#define UH_BT_GATT_UUID16_GATT_SERVICE                                 0x1801      /**< GATT service. */
#define UH_BT_GATT_UUID16_HEART_RATE_SERVICE                           0x180D      /**< Heart rate service. */
#define UH_BT_GATT_UUID16_BATTERY_SERVICE                              0x180F      /**< Battery service. */
#define UH_BT_GATT_UUID16_ENVIRONMENTAL_SENSING                        0x181A      /**< Environmental sensing. */

#define UH_BT_SIG_UUID16_DEVICE_NAME                                   0x2A00      /**< Device name. */
#define UH_BT_SIG_UUID16_APPEARANCE                                    0x2A01      /**< Appearance. */
#define UH_BT_SIG_UUID16_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS    0x2A04      /**< Peripheral preferred connection parameters. */
#define UH_BT_SIG_UUID16_SERVICE_CHANGED                               0x2A05      /**< Service changed. */
#define UH_BT_SIG_UUID16_BATTERY_LEVEL                                 0x2A19      /**< Battery level. */
#define UH_BT_SIG_UUID16_SERIAL_NUMBER                                 0x2A25      /**< Serial number. */
#define UH_BT_SIG_UUID16_MANUFACTURER_NAME                             0x2A29      /**< Manufacturer name. */
#define UH_BT_SIG_UUID16_TEMPERATURE                                   0x2A6E      /**< Temperature. */
#define UH_BT_SIG_UUID16_CENTRAL_ADDRESS_RESOLUTION                    0x2AA6      /**< Central address resolution. */

#define UH_BT_GATT_CHARC_PROP_BROADCAST                            0x01       /**< Broadcast. */
#define UH_BT_GATT_CHARC_PROP_READ                                 0x02       /**< Read. */
#define UH_BT_GATT_CHARC_PROP_WRITE_WITHOUT_RSP                    0x04       /**< Write without response. */
#define UH_BT_GATT_CHARC_PROP_WRITE                                0x08       /**< Write. */
#define UH_BT_GATT_CHARC_PROP_NOTIFY                               0x10       /**< Notify. */
#define UH_BT_GATT_CHARC_PROP_INDICATE                             0x20       /**< Indicate. */
#define UH_BT_GATT_CHARC_PROP_AUTHENTICATED_SIGNED_WRITE           0x40       /**< Authenticated signed write. */
#define UH_BT_GATT_CHARC_PROP_EXTENDED_PROPERTIES                  0x80       /**< Extended properties. */

#define UH_BT_GATT_CHARC_EXTENDED_PROP_RELIABLE_WRITE              0x0001     /**< Reliable write. */
#define UH_BT_GATT_CHARC_EXTENDED_PROP_WRITABLE_AUXILIARIES        0x0002     /**< Writable auxiliaries. */
/**
 * @}
 */

/**
 * Note, when creating a record, the application should synchronize the record's permission and characteristic properties to be read out by the client. When the client requests to read or write the record, the Bluetooth stack checks this permission.
 */
#define UH_BT_GATTS_REC_PERM_READ_SHIFT(mask)               ((mask)<<4) /**< Read the permission mask shift. */
#define UH_BT_GATTS_REC_PERM_WRITE_SHIFT(mask)              ((mask)<<0) /**< Write the permission mask shift. */
#define UH_BT_GATTS_REC_PERM_READ_MASK                      0xf0 /**< Read the permission mask. */
#define UH_BT_GATTS_REC_PERM_READABLE                       0x80 /**< The record is readable without any security requirement. */
#define UH_BT_GATTS_REC_PERM_READABLE_ENCRYPTION            (UH_BT_GATTS_REC_PERM_READABLE | (UH_BT_GATTS_REC_PERM_READ_SHIFT(UH_BT_GAP_LE_SECURITY_ENCRYPTION_MASK)))/**< The record is readable when the connection is encrypted. */
#define UH_BT_GATTS_REC_PERM_READABLE_AUTHENTICATION        (UH_BT_GATTS_REC_PERM_READABLE | (UH_BT_GATTS_REC_PERM_READ_SHIFT(UH_BT_GAP_LE_SECURITY_AUTHENTICATION_MASK)))/**< The record is readable only for the authenticated connection. */
#define UH_BT_GATTS_REC_PERM_READABLE_AUTHORIZATION         (UH_BT_GATTS_REC_PERM_READABLE | (UH_BT_GATTS_REC_PERM_READ_SHIFT(UH_BT_GAP_LE_SECURITY_AUTHORIZATION_MASK)))/**< The record is readable only after the application is granted a permission. */
#define UH_BT_GATTS_IS_READABLE(perm)                       (((perm) & UH_BT_GATTS_REC_PERM_READ_MASK)> 0) /**< Checks if the record is readable. */
#define UH_BT_GATTS_REC_PERM_WRITE_MASK                     0x0f /**< Write the permission mask. */
#define UH_BT_GATTS_REC_PERM_WRITABLE                       0x08/**< The record is writable without any security requirement. */
#define UH_BT_GATTS_REC_PERM_WRITABLE_ENCRYPTION            (UH_BT_GATTS_REC_PERM_WRITABLE | (UH_BT_GATTS_REC_PERM_WRITE_SHIFT(UH_BT_GAP_LE_SECURITY_ENCRYPTION_MASK)))/**< The record is writable when the connection is encrypted. */
#define UH_BT_GATTS_REC_PERM_WRITABLE_AUTHENTICATION        (UH_BT_GATTS_REC_PERM_WRITABLE | (UH_BT_GATTS_REC_PERM_WRITE_SHIFT(UH_BT_GAP_LE_SECURITY_AUTHENTICATION_MASK)))/**< The record is writable only for the authenticated connection. */
#define UH_BT_GATTS_REC_PERM_WRITABLE_AUTHORIZATION         (UH_BT_GATTS_REC_PERM_WRITABLE | (UH_BT_GATTS_REC_PERM_WRITE_SHIFT(UH_BT_GAP_LE_SECURITY_AUTHORIZATION_MASK)))/**< The record is writable only after the application is granted a permission. */
#define UH_BT_GATTS_IS_WRITABLE(perm)                       (((perm) & UH_BT_GATTS_REC_PERM_WRITE_MASK)> 0) /**< Checks if the record is writable. */
/* #bt_atts_rec_perm_t format:*/


#ifdef UH_HAL_BT_SUPPORT_RW_CB
#define UH_HAL_BT_READ_BUF_LEN             512
#define UH_HAL_BT_WRITE_BUF_LEN             512
#endif

#define UH_HAL_BT_EX_WRITE_BUF_LEN             512


typedef struct {
        //0x0020~0x4000, no less then 0xA0(100ms) for SCANABLE or NONCONN
        uint16_t interval_min;
        uint16_t interval_max;
#define UH_HAL_BT_ADV_TYPE_UNDIRECT  (0)
#define UH_HAL_BT_ADV_TYPE_DIRECT    (1)
#define UH_HAL_BT_ADV_TYPE_SCANABLE  (2)
#define UH_HAL_BT_ADV_TYPE_NONCONN   (3)
#define UH_HAL_BT_ADV_TYPE_LOW_DUTY  (4)
        uint8_t type;
#define UH_HAL_BT_OWN_ADDRESS_TYPE_PUBLIC  (0)
#define UH_HAL_BT_OWN_ADDRESS_TYPE_RANDOM  (1)
        uint8_t own_address_type;
} uh_hal_bt_adv_param_t;

/**
 * @brief 处理可写的属性数据
 * @connection_hdl 客户端连接句柄
 * @type 属性数据的类型
 * @chr_val 收到的客户端写入的属性值
 * @val_len 数据长度
 * @ret 0:成功, <0:失败
 */
typedef int32_t (*uh_bt_svr_chr_w_cb_func)(
        uint16_t handle,
        const uint16_t uuid16,
        const uint8_t * chr_val,
        const int32_t val_len);
void uh_bt_hal_svr_reg_chr_w_handler(uh_bt_svr_chr_w_cb_func cb);

/**
 * @brief 读取可读的可变属性数据
 * @connection_hdl 客户端连接句柄
 * @type 属性数据的类型
 * @chr_val 属性值，函数内分配空间，由调用者释放
 * @val_len 数据长度
 * @ret 0:成功, <0:失败
 */
typedef int32_t (*uh_bt_svr_chr_r_cb_func)(
        uint16_t handle,
        const uint16_t uuid16,
        uint8_t ** chr_val,
        int32_t * val_len);
void uh_bt_hal_svr_reg_chr_r_handler(uh_bt_svr_chr_r_cb_func cb);

#define UH_BT_HAL_STATUS_CON             1
#define UH_BT_HAL_STATUS_DISCON          2
/**
 * @brief 客户端连接状态回调
 * @connection_hdl 客户端连接句柄
 * @status 状态值, 1: 已连接, 2: 已断开
 * @ret 0:成功, <0:失败
 */
typedef void (*uh_bt_con_status_cb_func)(
        void * conn_int,
        int32_t status);
/**
 * @brief 定时任务回调(<=1s)
 * @connection_hdl 客户端连接句柄
 */
typedef void (*uh_bt_svr_timer_work_cb_func)(
        void * conn_int);
void uh_bt_hal_svr_reg_con_handler(uh_bt_con_status_cb_func cb, uh_bt_svr_timer_work_cb_func timer_cb);

#define UH_BLE_DFT_MTU              512
#define UH_BLE_DFT_PEER_MTU         23
#define BLE_CONN_HANDLE_INVALID 0xFFFF  /**< Invalid Connection Handle. */

/*UUID*/
enum
{
	UH_BT_UUID_TYPE_16,
	UH_BT_UUID_TYPE_32,
	UH_BT_UUID_TYPE_128,
};

struct uh_bt_uuid_base
{
	uint8_t type; /*UH_BT_UUID_TYPE_*/
};

struct uh_bt_uuid_16
{
	struct uh_bt_uuid_base uuid;
	uint16_t val;
};

struct uh_bt_uuid_32
{
	struct uh_bt_uuid_base uuid;
	uint32_t val;
};

struct uh_bt_uuid_128
{
	struct uh_bt_uuid_base uuid;
	uint8_t val[16];
};

typedef union
{
	struct uh_bt_uuid_base uuid;
	struct uh_bt_uuid_16 u16;
	struct uh_bt_uuid_32 u32;
	struct uh_bt_uuid_128 u128;
} uh_bt_uuid_t;

#define UH_BT_ADDR_PUBLIC  0x00
#define UH_BT_ADDR_RANDOM  0x01

typedef struct
{
	uint8_t type; /*UH_BT_ADDR_*/
	uint8_t val[6];
} uh_bt_addr_t;

/*Global*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_enable(void);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int uplus_ble_task_init(void );

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_disable(void);

extern void uh_bt_set_conn_mtu(uint16_t mut_size);

extern uint16_t uh_bt_get_conn_mtu(void);

/*Advertise*/
enum
{
	UH_BT_LE_ADV_OPTIONS_NONE = 0,
	UH_BT_LE_ADV_OPTIONS_CONNECTABLE = 0x1,
	UH_BT_LE_ADV_OPTIONS_ONE_TIME = 0x2,
};

/** LE Advertising Parameters. */
typedef struct
{
	uint8_t options; /*UH_BT_LE_ADV_OPTIONS_*/

	/** Minimum Advertising Interval (N * 0.625) */
	uint16_t interval_min;

	/** Maximum Advertising Interval (N * 0.625) */
	uint16_t interval_max;

	/** Optional predefined (random) own address. Currently
	 *  the only permitted use of this is for NRPA with
	 *  non-connectable advertising.
	 */
	const uh_bt_addr_t *own_addr;
} uh_bt_le_adv_param_t;
/**
 *  @brief      LE set advertising data command.
 */
typedef struct {
    uint8_t           advertising_data_length; /**< Length of the advertising data. */
    uint8_t           advertising_data[31];    /**< Advertising data. */
}bt_hci_cmd_le_set_advertising_data_t;
/**
 *  @brief      LE set scan response data command.
 */
typedef struct {
    uint8_t           scan_response_data_length; /**< Length of the scan response data. */
    uint8_t           scan_response_data[31];    /**< Scan response data. */
}bt_hci_cmd_le_set_scan_response_data_t;

/*GAP Pair auth param*/
/**@brief GAP security parameters. */
typedef struct
{
    uint8_t               io_caps;             /**< IO capabilities, see @ref BLE_GAP_IO_CAPS. */
    uint8_t               oob;             /**< The OOB data flag.
                                                        - In LE legacy pairing, this flag is set if a device has out of band authentication data.
                                                          The OOB method is used if both of the devices have out of band authentication data.
                                                        - In LE Secure Connections pairing, this flag is set if a device has the peer device's out of band authentication data.
                                                          The OOB method is used if at least one device has the peer device's OOB data available. */
    uint8_t               auth_req;
    uint8_t               auth_pair_mode;
    uint8_t               max_key_size;              /**< Maximum encryption key size in octets between min_key_size and 16. */
} uh_hal_bt_gap_sec_params_t;

typedef struct
{
    uint8_t                                     ucBLEPowerFLg;
    uplus_ble_gap_adv_param_t                   adv_param;
    bt_hci_cmd_le_set_advertising_data_t        adv_data;
    bt_hci_cmd_le_set_scan_response_data_t      scan_rsp;
    uint16_t                                    my_mtu;
    uint16_t                                    peer_mtu;
    uint16_t                                    conn_mtu; // conn mtu
    uh_bt_svr_chr_w_cb_func                     chr_w_cb;
    uh_bt_svr_chr_r_cb_func                     chr_r_cb;
    uh_bt_con_status_cb_func                    con_cb;
    uh_bt_svr_timer_work_cb_func                timer_cb;
    uint16_t                                    conn_handle;
    uint32_t                                    conn_time;
    uint8_t                                     is_connected;
    int8_t                                      is_get_rssi;
    uh_hal_bt_gap_sec_params_t                  sec_params_auth;
    int32_t                                     notify_task_running;
    uplus_ble_gatts_db_t                        uplus_ble_gatts_db;
    uplus_timer_id_e                            send_notify_timer;
    int32_t                                     notify_send_try_cnt;
    uint16_t                                    notify_uuid;
} uh_hal_bt_gatt_svc_t;

/* EIR/AD data type definitions */
#define UH_BT_AD_TYPE_FLAGS                   0x01 /* AD flags */
#define UH_BT_AD_TYPE_UUID16_SOME             0x02 /* 16-bit UUID, more available */
#define UH_BT_AD_TYPE_UUID16_ALL              0x03 /* 16-bit UUID, all listed */
#define UH_BT_AD_TYPE_UUID32_SOME             0x04 /* 32-bit UUID, more available */
#define UH_BT_AD_TYPE_UUID32_ALL              0x05 /* 32-bit UUID, all listed */
#define UH_BT_AD_TYPE_UUID128_SOME            0x06 /* 128-bit UUID, more available */
#define UH_BT_AD_TYPE_UUID128_ALL             0x07 /* 128-bit UUID, all listed */
#define UH_BT_AD_TYPE_NAME_SHORTENED          0x08 /* Shortened name */
#define UH_BT_AD_TYPE_NAME_COMPLETE           0x09 /* Complete name */
#define UH_BT_AD_TYPE_TX_POWER                0x0a /* Tx Power */
#define UH_BT_AD_TYPE_SOLICIT16               0x14 /* Solicit UUIDs, 16-bit */
#define UH_BT_AD_TYPE_SOLICIT128              0x15 /* Solicit UUIDs, 128-bit */
#define UH_BT_AD_TYPE_SVC_AD_TYPE16           0x16 /* Service AD_TYPE, 16-bit UUID */
#define UH_BT_AD_TYPE_GAP_APPEARANCE          0x19 /* GAP appearance */
#define UH_BT_AD_TYPE_SOLICIT32               0x1f /* Solicit UUIDs, 32-bit */
#define UH_BT_AD_TYPE_SVC_AD_TYPE32           0x20 /* Service AD_TYPE, 32-bit UUID */
#define UH_BT_AD_TYPE_SVC_AD_TYPE128          0x21 /* Service AD_TYPE, 128-bit UUID */
#define UH_BT_AD_TYPE_MESH_PROV               0x29 /* Mesh Provisioning PDU */
#define UH_BT_AD_TYPE_MESH_MESSAGE            0x2a /* Mesh Networking PDU */
#define UH_BT_AD_TYPE_MESH_BEACON             0x2b /* Mesh Beacon */

#define UH_BT_AD_TYPE_MANUFACTURER_AD_TYPE    0xff /* Manufacturer Specific AD_TYPE */


typedef struct
{
	uint8_t data_len;
	uint8_t type; /*UH_BT_AD_TYPE_XXX*/
	uint8_t data[32];
} uh_bt_adv_data_t;


/**
 * @brief
 * @note
 * @param
 * @return
 */
uh_hal_bt_gatt_svc_t * uh_bt_get_svc_handle(void);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_ad_add(const uh_bt_adv_data_t *ad);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_ad_update(int ad_ind, const uh_bt_adv_data_t *ad);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_ad_del(int ad_ind);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_sd_add(const uh_bt_adv_data_t *sd);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_sd_update(int32_t sd_ind, const uh_bt_adv_data_t *sd);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_sd_del(int32_t sd_ind);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_start(void);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_le_adv_stop(void);

/*Security*/
#define UH_GAP_IO_CAP_DISPLAY_ONLY        0x00/*设备能够显示或传送6位十进制数，
                                               但没有能力表示“是”或“否”。*/
#define UH_GAP_IO_CAP_DISPLAY_YESNO	      0x01/*设备能够显示或传送6位十进制数。此外，该设备至少有两个按钮，
                                               其中一个按钮显示为“是”，另一个按钮显示为“否”
                                               或者设备具有用户可以指示“是”或“否”的机制。 */
#define UH_GAP_IO_CAP_KEYBOARD_ONLY	      0x02
#define UH_GAP_IO_CAP_NO_INPUT_NO_OUTPUT  0x03
#define UH_GAP_IO_CAP_KEYBOARD_DISPLAY    0x04
#define UH_GAP_IO_CAP_OOB                 0x05

/*Out of Band, OOB*/
#define UH_GAP_OOB_DATA_NOT_PRESENTED     0x00
#define UH_GAP_OOB_DATA_PRESENTED         0x01

#define UH_GAP_SECU_REQ_MITM                     (0x01)
#define UH_GAP_SECU_REQ_ENCRYPTED                (0x02)
#define UH_GAP_SECU_REQ_BOND                     (0x08)
#define UH_GAP_SECU_REQ_ALL                   (0x01|0x02|0x08)


/** @defgroup BOND_PAIRING_MODE_DEFINES Pairing Modes
 * @{
 */
#define UH_GAP_PAIRING_MODE_NO_PAIRING          0x00  //!< Pairing is not allowed.
#define UH_GAP_PAIRING_MODE_PAIRABLE            0x01  //!< Pairable, Wait for a pairing request from master or security request from slave.
/**
  * @}
  */

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void uh_bt_gap_pairing_feature_set(uint8_t io_cap, uint8_t oob, uint8_t secu_prop);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_gap_pairing(void *conn);

typedef struct {
        //0x0020~0x4000, no less then 0xA0(100ms) for SCANABLE or NONCONN
        uint16_t interval_min;
        uint16_t interval_max;
#define UH_BT_ADV_TYPE_UNDIRECT  (0)
#define UH_BT_ADV_TYPE_DIRECT    (1)
#define UH_BT_ADV_TYPE_SCANABLE  (2)
#define UH_BT_ADV_TYPE_NONCONN   (3)
#define UH_BT_ADV_TYPE_LOW_DUTY  (4)
        uint8_t type;
#define UH_BT_OWN_ADDRESS_TYPE_PUBLIC  (0)
#define UH_BT_OWN_ADDRESS_TYPE_RANDOM  (1)
        uint8_t own_address_type;
} uh_bt_adv_param_t;
void uh_bt_set_adv_param(uh_bt_adv_param_t *param);

#define UH_GAP_PAIRING_EVENT_PASSKEY_DISPLAY 1
#define UH_GAP_PAIRING_EVENT_PASSKEY_ENTRY   2
#define UH_GAP_PAIRING_EVENT_PASSKEY_CONFIRM 3

typedef void (* uh_pairing_cb_func)(void *conn, int event, void * ev_data);
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void uh_bt_gap_pairing_cb_register(uh_pairing_cb_func cb);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void uh_bt_gap_pairing_passkey_entry(void *conn, uint32_t passkey);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void uh_bt_gap_pairing_passkey_confirm(void *conn, int accept);

/** No encryption and no authentication. */
#define UH_BT_SECURITY_LOW 1
/** Encryption and no authentication (no MITM). */
#define UH_BT_SECURITY_MEDIUM 2
/** Encryption and authentication (MITM). */
#define UH_BT_SECURITY_HIGH   3
/** Authenticated Secure Connections */
#define UH_BT_SECURITY_FIPS   4
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void uh_bt_gap_security(void *conn, int security_level);

/*Conn*/
typedef void (* uh_connected_cb_func)(void *conn);
typedef void (* uh_disconnected_cb_func)(void *conn, int reason);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void uh_bt_conn_cb_register(uh_connected_cb_func connected_cb, uh_disconnected_cb_func disconnected_cb);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void uh_bt_conn_disconnect(void *conn, int reason);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern uint32_t uh_bt_conn_mtu_get(void * conn);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern uh_bt_addr_t * uh_bt_conn_dst_get(void * conn);

/*Gatt*/
extern void uh_bt_gatt_mtu_default_set(uint16_t mtu);

#define UH_BT_GATT_PERM_NONE          0x00
#define UH_BT_GATT_PERM_READ          0x01
#define UH_BT_GATT_PERM_WRITE         0x02
#define UH_BT_GATT_PERM_READ_ENCRYPT  0x04
#define UH_BT_GATT_PERM_WRITE_ENCRYPT 0x08
#define UH_BT_GATT_PERM_READ_AUTHEN   0x10
#define UH_BT_GATT_PERM_WRITE_AUTHEN  0x20
#define UH_BT_GATT_PERM_PREPARE_WRITE 0x40

/*Gatt service*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_service_create(uint16_t uuid16);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_service_delete(uint16_t uuid16);


extern void uh_bt_rw_cb_register(void);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_gatt_service_start(void * svc_if);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int32_t uh_bt_gatt_service_stop(void * svc_if);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_include_add(void * svc_if, void * inc_svc_if);

#define UH_BT_GATT_CHRC_BROADCAST          0x01
#define UH_BT_GATT_CHRC_READ               0x02
#define UH_BT_GATT_CHRC_WRITE_WITHOUT_RESP 0x04
#define UH_BT_GATT_CHRC_WRITE              0x08
#define UH_BT_GATT_CHRC_NOTIFY             0x10
#define UH_BT_GATT_CHRC_INDICATE           0x20
#define UH_BT_GATT_CHRC_AUTH               0x40
#define UH_BT_GATT_CHRC_EXT_PROP           0x80


extern void * uh_bt_gatt_characteristic_add_auto(void *vpgatt_service, uint16_t uuid16, uint8_t properties, uint16_t val_perms);


/*Characteristic*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_add(void *gatt_service, uint16_t uuid, uint8_t properties, uint16_t val_perms);

typedef int (* uh_read_chr_func)(void *conn, void * chr_if, void *buf, uint16_t len, uint16_t offset);
#define UH_BT_GATT_WRITE_FLAG_PREPARE 0x1
typedef int (* uh_write_chr_func)(void *conn, void * chr_if, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_op_set(void *vpgatt_service, uint16_t uuid16, uint16_t val_perms, uh_read_chr_func read, uh_write_chr_func write)
;

#define UH_BT_GATT_CEP_RELIABLE_WRITE    0x0001
#define UH_BT_GATT_CEP_WRITABLE_AUX      0x0002
/*Characteristic Extended Properties*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_cep_add(void *vpgatt_service, uint8_t properties, uint16_t val_perms);

/*Characteristic User Description*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_cud_add(void * chr_if, char * string, uint8_t perm);

#define UH_BT_GATT_CCC_NOTIFY      0x0001
#define UH_BT_GATT_CCC_INDICATE    0x0002

typedef uint32_t (*uh_gatts_rec_callback_t) (const uint8_t rw, uint16_t handle, void *data, uint16_t size, uint16_t offset);
/*Client Characteristic Configuration*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_ccc_add(void *vpgatt_service, uint8_t properties, uh_gatts_rec_callback_t notify_callback);

#define UH_BT_GATT_SCC_BROADCAST      0x0001

/*Server Characteristic Configuration*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_scc_add(void * chr_if, uint16_t flags, uint8_t perm);

/*Characteristic Presentation Format*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_cpf_add(void * chr_if,
	uint8_t format, int8_t exponent, uint16_t unit, uint8_t name_space, uint16_t description);

/*Characteristic Aggregate Format*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern void * uh_bt_gatt_characteristic_caf_add(void * chr_if, void *);

/*Indicate or Notify*/
/**
 * @brief
 * @note
 * @param
 *
 * @return
 */
extern int uh_bt_gatt_characteristic_val_report(void *conn, void * chr_if, const void *data, uint16_t len);

extern int32_t uh_bt_get_rssi(void * conn);

extern int32_t uh_bt_stop_get_rssi(void * conn);

extern void uh_bt_set_tx_power(void * conn, int32_t level);

extern void uh_bt_get_public_addr(uint8_t *addr);

/**
 * @brief Client send notify
 */
extern int32_t uh_bt_send_notify(const void * conn, uint16_t uuid16, uint8_t* chr_val, int32_t val_len);

/**
 * @brief add bt worker timer
 */
extern  void uh_bt_add_worker_timer(void);

#ifdef __cplusplus
}
#endif

#endif /*__UH_BLUETOOTH_H__*/

