/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_BLE_API_H__
#define __UPLUS_BLE_API_H__

typedef enum {
	UPLUS_BLE_TX_POWER_N40DBM,
	UPLUS_BLE_TX_POWER_N20DBM,
	UPLUS_BLE_TX_POWER_N16DBM,
	UPLUS_BLE_TX_POWER_N15DBM,
	UPLUS_BLE_TX_POWER_N12DBM,
	UPLUS_BLE_TX_POWER_N8DBM,
	UPLUS_BLE_TX_POWER_N6DBM,
	UPLUS_BLE_TX_POWER_N5DBM,
	UPLUS_BLE_TX_POWER_N4DBM,
	UPLUS_BLE_TX_POWER_N3DBM,
	UPLUS_BLE_TX_POWER_N2DBM,
	UPLUS_BLE_TX_POWER_N1DBM,
	UPLUS_BLE_TX_POWER_0DBM,
	UPLUS_BLE_TX_POWER_1DBM,
	UPLUS_BLE_TX_POWER_3DBM,
	UPLUS_BLE_TX_POWER_4DBM,
	UPLUS_BLE_TX_POWER_5DBM,
	UPLUS_BLE_TX_POWER_7_5DBM,
	UPLUS_BLE_TX_POWER_8DBM
} uplus_ble_tx_power_e;

typedef enum {
	UPLUS_BLE_SLAVE_ROLE=0,
	UPLUS_BLE_MASTER_ROLE
} uplus_ble_role_type_e;

#define UPLUS_BLE_ADV_TYPE_UNDIRECT (0)
#define UPLUS_BLE_ADV_TYPE_DIRECT (1)
#define UPLUS_BLE_ADV_TYPE_SCANABLE (2)
#define UPLUS_BLE_ADV_TYPE_NONCONN (3)
#define UPLUS_BLE_ADV_TYPE_LOW_DUTY (4)

#define UPLUS_BLE_OWN_ADDRESS_TYPE_PUBLIC (0)
#define UPLUS_BLE_OWN_ADDRESS_TYPE_RANDOM (1)

#define UPLUS_BLE_GAP_IO_CAP_DISPLAY_ONLY 0x00
#define UPLUS_BLE_GAP_IO_CAP_DISPLAY_YESNO 0x01
#define UPLUS_BLE_GAP_IO_CAP_KEYBOARD_ONLY 0x02
#define UPLUS_BLE_GAP_IO_CAP_NO_INPUT_NO_OUTPUT 0x03
#define UPLUS_BLE_GAP_IO_CAP_KEYBOARD_DISPLAY 0x04
#define UPLUS_BLE_GAP_IO_CAP_OOB 0x05

/*Out of Band, OOB*/
#define UPLUS_BLE_GAP_OOB_DATA_NOT_PRESENTED 0x00
#define UPLUS_BLE_GAP_OOB_DATA_PRESENTED 0x01

#define UPLUS_BLE_GAP_SECU_REQ_MITM 0x01
#define UPLUS_BLE_GAP_SECU_REQ_ENCRYPTED 0x02
#define UPLUS_BLE_GAP_SECU_REQ_BOND 0x08
#define UPLUS_BLE_GAP_SECU_REQ_ALL (0x01|0x02|0x08)

/** defgroup BOND_PAIRING_MODE_DEFINES Pairing Mode*/
#define UPLUS_BLE_GAP_PAIRING_MODE_NO_PAIRING 0x00 //!< Pairing is not allowed.
#define UPLUS_BLE_GAP_PAIRING_MODE_PAIRABLE 0x01 //

typedef struct {
	//0x0020~0x4000, no less then 0xA0(100ms) for SCANABLE or NONCONN
	uint16_t interval_min;
	uint16_t interval_max;
	uint8_t type;
	uint8_t own_address_type;
} uplus_ble_adv_param_t;

/**
 * \brief 初始化服务
 *  	其中添加service时需将创建服务返回的handle回写到
 * 		uplus_ble_gatts_srv_db_t结构体对应的srv_handle;
 *
 * 		其中添加char时需将创建char返回的handle回写到
 * 		uplus_ble_gatts_char_db_t结构体下
 * 		uplus_ble_gatts_char_handles_t对应的value_handle;
 * 		如果char使能notify, 必须将创建的cccd handler回写到
 * 		cccd_handle，并且创建的descriptor源采用2902
 */
uplus_ble_status_e uplus_ble_gatts_service_init(
		uplus_ble_gatts_db_t *uplus_ble_service_database);

/**
 * \brief
 */
void uplus_ble_gap_init(void);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_set_adv_param(
		uplus_ble_adv_param_t *adv_param);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_address_get(
		uplus_ble_addr_t mac);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_scan_start(
		uplus_ble_gap_scan_type_t scan_type,
		uplus_ble_gap_scan_param_t scan_param);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_scan_stop(void);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_adv_start(
		uplus_ble_gap_adv_param_t *p_adv_param);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_adv_data_set(
		uint8_t const * p_data,
		uint8_t dlen,
		uint8_t const *p_sr_data,
		uint8_t srdlen);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_adv_stop(void);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_connect(
		uplus_ble_gap_scan_param_t scan_param,
		uplus_ble_gap_connect_t conn_param);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_update_conn_params(
		uint16_t conn_handle,
		uplus_ble_gap_conn_param_t conn_params);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_tx_power_set(
		uint16_t conn_handle,
        int8_t radio_tx_pwr);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_rssi_start(
		uint16_t conn_handle,
        uint8_t threshold_dbm,
        uint8_t skip_count);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_rssi_stop(
		uint16_t conn_handle);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_rssi_get(
		uint16_t conn_handle,
        int8_t  *p_rssi,
        uint8_t *p_ch_index);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gap_disconnect(
		uint16_t conn_handle);

/**
 * \brief gap事件回调函数
 */
typedef void(*uplus_gap_evt_handler_t)(
		uplus_ble_gap_evt_t evt,
		uplus_ble_gap_evt_param_t*param);

/**
 * \brief 注册gap事件回调函数
 */
void uplus_ble_gap_register(
	uplus_gap_evt_handler_t gap_evt_handler,
	uplus_ble_role_type_e user_type);

/**
 * \brief gatts事件回调函数
 * \return 0 表示处理成功，其他值表示错误.
 * 			 其中如果是写事件，需将返回值返回给peer；
 * 			 如果读事件：
 * 				返回值0，结构体uplus_ble_gatts_evt_param_t中
 * 			 	uplus_ble_gatts_read_t的data和len是读返回的数据，发送完后，需将
 * 			 	data指向的内存free；
 * 				返回值是其他值，将返回值返回给对方；
 */
typedef int (*uplus_gatts_evt_handler_t)(
		uplus_ble_gatts_evt_t evt,
		uplus_ble_gatts_evt_param_t* param);


/**
 * \brief 注册gatts事件回调函数
 */
void uplus_ble_gatts_register(
		uplus_gatts_evt_handler_t gatts_evt_handler);


#define UPLUS_NOTIFY_TYPE			1
#define UPLUS_INDICATION_TYPE		2
/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gatts_notify_or_indicate(
		uint16_t conn_handle,
		uint16_t srv_handle,
		uint16_t char_value_handle,
		uint8_t offset,
		uint8_t* p_value,
		int len,
		uint8_t type);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gatts_value_set(
	uint16_t srv_handle,
	uint16_t value_handle,
	uint8_t offset,
	uint8_t* p_value,
	uint16_t len);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gatts_value_get(
		uint16_t srv_handle,
		uint16_t char_handle,
		uint8_t* pdata,
		uint16_t *p_len);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gatts_mtu_default_set(
		uint16_t att_mtu);

/**
 * \brief
 */
uint16_t uplus_ble_gatts_mtu_get(uint16_t conn_handle);

/**
 * \brief
 */
uplus_ble_status_e uplus_ble_gatts_rw_auth_reply(
		uint16_t conn_handle,
		uint8_t status,
		uint16_t char_value_handle,
		uint16_t offset,
		uint8_t* p_value,
		int len,
		uint8_t type);

/**
 * \brief  蓝牙配对特性设置
 * \param [in] io_caps
 * 		  [in] auth_oob
 * 		  [in] auth_req
 * 		  [in] auth_pair_mode
 * \return 成功返回 UPLUS_BLE_SUCCESS
 * 		   失败返回其他值
 */
uplus_ble_status_e uplus_ble_pairing_feature_set(
    uint8_t io_caps ,
    uint8_t auth_oob ,
    uint8_t auth_req ,
    uint8_t auth_pair_mode);
#endif
