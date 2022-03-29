/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef UPLUS_BLE_TYPE_H__
#define UPLUS_BLE_TYPE_H__

#define UPLUS_BLE_GAP_EVT_BASE          0x00
#define UPLUS_BLE_GATTS_EVT_BASE        0x40
#define UPLUS_BLE_GATTC_EVT_BASE        0x80

#define UPLUS_BLE_SERVERS_MAX           4
#define UPLUS_BLE_CHAR_MAX              6
#define UPLUS_BLE_QWRS_MAX_RCV_SIZE     128

typedef uint8_t uplus_ble_addr_t[6];
typedef uint32_t uplus_ble_cfm_t;

typedef struct {
    uint16_t begin_handle;
    uint16_t end_handle;
} uplus_ble_handle_range_t;

typedef enum {
    UPLUS_BLE_ADDRESS_TYPE_PUBLIC, // public address
    UPLUS_BLE_ADDRESS_TYPE_RANDOM, // random address
} uplus_ble_addr_type_t;

/*
	added by gaopeilin
	for dev ctrl,get alarm,
	report and so on
*/
typedef enum
{
	UPLUS_CMD_TYPE_CTRL,
	UPLUS_CMD_TYPE_STATUS,
	UPLUS_CMD_TYPE_ALARM,
	UPLUS_CMD_TYPE_STOP_ALARM,
	UPLUS_CMD_TYPE_REPORT,
	UPLUS_CMD_TYPE_GET_ALARM,
	UPLUS_CMD_TYPE_GET_ALARM_ACK,
	UPLUS_CMD_TYPE_GROUP_CMD,
	UPLUS_CMD_TYPE_INVALID_CMD,
	UPLUS_CMD_TYPE_RPT_TIME,
	UPLUS_CMD_TYPE_RPT_TIME_ACK,
	UPLUS_CMD_TYPE_RPT_CFG,
	UPLUS_CMD_TYPE_RPT_CFG_ACK,
	UPLUS_CMD_TYPE_ERR
}uplus_ble_cmd_type_t;

//end

/* GAP related */
typedef enum {
    UPLUS_BLE_SCAN_TYPE_PASSIVE,  // passive scanning
    UPLUS_BLE_SCAN_TYPE_ACTIVE,   // active scanning
} uplus_ble_gap_scan_type_t;

typedef struct {
    uint16_t scan_interval;                   // Range: 0x0004 to 0x4000 Time = N * 0.625 msec Time Range: 2.5 msec to 10.24 sec
    uint16_t scan_window;                     // Range: 0x0004 to 0x4000 Time = N * 0.625 msec Time Range: 2.5 msec to 10.24 seconds
    uint16_t timeout;                         // Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout.
} uplus_ble_gap_scan_param_t;

typedef enum {
    UPLUS_BLE_ADV_TYPE_CONNECTABLE_UNDIRECTED,      // ADV_IND
    UPLUS_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED,        // ADV_SCAN_IND
    UPLUS_BLE_ADV_TYPE_NON_CONNECTABLE_UNDIRECTED,  // ADV_NONCONN_INC
    //add by yuanlongfei
	UPLUS_BLE_ADV_TYPE_CONNECTABLE_DIRECTED_HDC, /*Connectable high duty cycle directed advertisement*/
	UPLUS_BLE_ADV_TYPE_CONNECTABLE_DIRECTED_LDC /*Connectable low duty cycle directed advertisement*/
} uplus_ble_gap_adv_type_t;

typedef struct {
    uint16_t adv_interval_min;               // Range: 0x0020 to 0x4000  Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec
    uint16_t adv_interval_max;               // Range: 0x0020 to 0x4000  Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec
	uplus_ble_gap_adv_type_t adv_type;
    uint8_t own_address_type;
	struct {
		uint8_t ch_37_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 37 */
		uint8_t ch_38_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 38 */
		uint8_t ch_39_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 39 */
	} ch_mask;
} uplus_ble_gap_adv_param_t;

typedef enum {
    ADV_DATA,           // advertising data
    SCAN_RSP_DATA,      // response data from active scanning
} uplus_ble_gap_adv_data_type_t;

typedef struct {
    uplus_ble_addr_t peer_addr;
    uplus_ble_addr_type_t addr_type;
    uplus_ble_gap_adv_data_type_t adv_type;
    int8_t rssi;
    uint8_t data[31];
    uint8_t data_len;
} uplus_ble_gap_adv_report_t;

typedef enum {
    CONNECTION_TIMEOUT = 1,
    REMOTE_USER_TERMINATED,
    LOCAL_HOST_TERMINATED
} uplus_ble_gap_disconnect_reason_t;

typedef struct {
    uint16_t min_conn_interval;    // Range: 0x0006 to 0x0C80, Time = N * 1.25 msec, Time Range: 7.5 msec to 4 seconds.
    uint16_t max_conn_interval;    // Range: 0x0006 to 0x0C80, Time = N * 1.25 msec, Time Range: 7.5 msec to 4 seconds.
    uint16_t slave_latency;        // Range: 0x0000 to 0x01F3
    uint16_t conn_sup_timeout;     // Range: 0x000A to 0x0C80, Time = N * 10 msec, Time Range: 100 msec to 32 seconds
} uplus_ble_gap_conn_param_t;

typedef enum {
    UPLUS_BLE_GAP_INVALID,
    UPLUS_BLE_GAP_PERIPHERAL,
    UPLUS_BLE_GAP_CENTRAL,
} uplus_ble_gap_role_t;

typedef struct {
    uplus_ble_addr_t peer_addr;
    uplus_ble_addr_type_t type;
    uplus_ble_gap_role_t role;
    uplus_ble_gap_conn_param_t conn_param;
} uplus_ble_gap_connect_t;

typedef struct {
    uplus_ble_gap_disconnect_reason_t reason;
} uplus_ble_gap_disconnect_t;

typedef struct {
    uplus_ble_gap_conn_param_t conn_param;
} uplus_ble_gap_connect_update_t;

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

typedef struct {
    uint16_t conn_handle;
	union {
		uplus_ble_gap_connect_t connect;
		uplus_ble_gap_disconnect_t disconnect;
		uplus_ble_gap_adv_report_t report;
		uplus_ble_gap_connect_update_t update_conn;
	};
} uplus_ble_gap_evt_param_t;

typedef enum {
    UPLUS_BLE_GAP_EVT_CONNECTED = UPLUS_BLE_GAP_EVT_BASE, /**< Generated when a connection is established.*/
    UPLUS_BLE_GAP_EVT_DISCONNET, /**< Generated when a connection is terminated.*/
    UPLUS_BLE_GAP_EVT_CONN_PARAM_UPDATED,
    UPLUS_BLE_GAP_EVT_ADV_REPORT,
} uplus_ble_gap_evt_t;

/*GATTS related*/

// GATTS database
typedef enum
{
	UPLUS_BLE_UUID_TYPE_16  = 0x00,
	UPLUS_BLE_UUID_TYPE_128 = 0x01,
} uplus_ble_uuid_type_t;

/** @defgroup BLE_GATTS_OPS GATT Server Operations */
typedef enum {
    UPLUS_BLE_GATTS_OP_INVALID               = 0x00,  /**< Invalid Operation. */
    UPLUS_BLE_GATTS_OP_WRITE_REQ             = 0x01,  /**< Write Request. */
    UPLUS_BLE_GATTS_OP_WRITE_CMD             = 0x02,  /**< Write Command. */
    UPLUS_BLE_GATTS_OP_SIGN_WRITE_CMD        = 0x03,  /**< Signed Write Command. */
    UPLUS_BLE_GATTS_OP_PREP_WRITE_REQ        = 0x04,  /**< Prepare Write Request. */
    UPLUS_BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL = 0x05,  /**< Execute Write Request: Cancel all prepared writes. */
    UPLUS_BLE_GATTS_OP_EXEC_WRITE_REQ_NOW    = 0x06,  /**< Execute Write Request: Immediately execute all prepared writes. */
} uplus_ble_gatts_op_t;

#define UPLUS_BLE_UUID_16           0
#define UPLUS_BLE_UUID_128          1
typedef struct {
	uint8_t type;                                     // UPLUS_BLE_UUID_16 = 0	UPLUS_BLE_UUID_128 = 1
    union {
        uint16_t uuid16;
#ifndef P_TELINK_8258
        uint8_t uuid128[16];
#endif
    };
} uplus_ble_uuid_t;

typedef struct {
    uint16_t uuid16;
} uplus_ble_uuid16_t;

typedef enum {
    UPLUS_BLE_PRIMARY_SERVICE = 1,
    UPLUS_BLE_SECONDARY_SERVICE,
} uplus_ble_gatts_service_t;

typedef struct{
	uint8_t reliable_write     :1;
	uint8_t writeable          :1;
} uplus_ble_gatts_char_desc_ext_prop_t;

typedef struct{
	char *string;
	uint8_t len;
} uplus_ble_gatts_char_desc_user_desc_t;

typedef struct{
	uint8_t  format;
	uint8_t  exponent;
	uint16_t unit;
	uint8_t  name_space;
	uint16_t desc;
} uplus_ble_gatts_char_desc_cpf_t;

/*
 * NOTE: if char property contains notify , then SHOULD include cccd(client characteristic configuration descriptor automatically). The same to sccd when BROADCAST enabled
 * */
typedef struct{
	//uplus_ble_gatts_char_desc_ext_prop_t  *extend_prop;
	uplus_ble_gatts_char_desc_ext_prop_t  extend_prop;
	//uplus_ble_gatts_char_desc_cpf_t       *char_format;     // See more details at Bluetooth SPEC 4.2 [Vol 3, Part G] Page 539
	uplus_ble_gatts_char_desc_user_desc_t *user_desc;     	// read only
} uplus_ble_gatts_char_desc_db_t;

typedef struct
{
  uint16_t          value_handle;       /**< Handle to the characteristic value. */
  uint16_t          user_desc_handle;   /**< Handle to the User Description descriptor, or @ref BLE_GATT_HANDLE_INVALID if not present. */
  uint16_t          cccd_handle;        /**< Handle to the Client Characteristic Configuration Descriptor, or @ref BLE_GATT_HANDLE_INVALID if not present. */
  uint16_t          sccd_handle;        /**< Handle to the Server Characteristic Configuration Descriptor, or @ref BLE_GATT_HANDLE_INVALID if not present. */
} uplus_ble_gatts_char_handles_t;

// gatts characteristic
// default:  no authentication ; no encrption; configurable authorization

typedef struct {
	uplus_ble_uuid_t char_uuid;
	uint8_t char_property;                             // See TYPE uplus_ble_gatts_char_property for details
	uint8_t *p_value;                                  // initial characteristic value
	uint16_t char_value_len;
	uplus_ble_gatts_char_handles_t char_handle;        // [out] where the assigned handle be stored.
	bool is_variable_len;
	bool rd_author;                                    // read authorization. Enabel or Disable UPLUS_BLE_GATTS_READ_PERMIT_REQ event
	bool wr_author;                                    // write authorization. Enabel or Disable UPLUS_BLE_GATTS_WRITE_PERMIT_REQ event
    bool is_notification_enabled;
    uplus_ble_gatts_char_desc_db_t char_desc_db;
    uint16_t char_cfg;
} uplus_ble_gatts_char_db_t;

typedef struct{
	uplus_ble_gatts_service_t srv_type;                    // primary service or secondary service
	uint16_t srv_handle;                               // [out] dynamically allocated
	uplus_ble_uuid_t srv_uuid;                             // 16-bit or 128-bit uuid
	uint8_t char_num;
	uplus_ble_gatts_char_db_t *p_char_db;                  // p_char_db[charnum-1]
} uplus_ble_gatts_srv_db_t;                                // Regardless of service inclusion service

typedef struct{
	uplus_ble_gatts_srv_db_t *p_srv_db;                    // p_srv_db[srv_num]
	uint8_t srv_num;
} uplus_ble_gatts_db_t;

typedef enum {
    UPLUS_BLE_BROADCAST           = 0x01,
    UPLUS_BLE_READ                = 0x02,
    UPLUS_BLE_WRITE_WITHOUT_RESP  = 0x04,
    UPLUS_BLE_WRITE               = 0x08,
    UPLUS_BLE_NOTIFY              = 0x10,
    UPLUS_BLE_INDICATE            = 0x20,
    UPLUS_BLE_AUTH_SIGNED_WRITE   = 0x40,
} uplus_ble_gatts_char_property;

typedef enum {
    UPLUS_BLE_GATTS_EVT_WRITE = UPLUS_BLE_GATTS_EVT_BASE,      // When this event is called, the characteristic has been modified.
	UPLUS_BLE_GATTS_EVT_READ,
    UPLUS_BLE_GATTS_EVT_READ_PERMIT_REQ,                   // If charicteristic's rd_auth = TRUE, this event will be generated.
    UPLUS_BLE_GATTS_EVT_WRITE_PERMIT_REQ,                  // If charicteristic's wr_auth = TRUE, this event will be generated, meanwhile the char value hasn't been modified. uplus_ble_gatts_rw_auth_reply().
	UPLUS_BLE_GATTS_EVT_IND_CONFIRM
} uplus_ble_gatts_evt_t;


/** @defgroup BLE_GATTS_AUTHORIZE_TYPES GATT Server Authorization Types
 * @{ */
#define BLE_GATTS_AUTHORIZE_TYPE_INVALID    0x00  /**< Invalid Type. */
#define BLE_GATTS_AUTHORIZE_TYPE_READ       0x01  /**< Authorize a Read Operation. */
#define BLE_GATTS_AUTHORIZE_TYPE_WRITE      0x02  /**< Authorize a Write Request Operation. */

/*
 * UPLUS_BLE_GATTS_EVT_WRITE and UPLUS_BLE_GATTS_EVT_WRITE_PERMIT_REQ events callback
 * parameters
 * NOTE: Stack SHOULD decide whether to response to gatt client. And if need to reply, just reply success or failure according to [permit]
 * */
typedef struct {
    uint16_t value_handle; // char value_handle
    uplus_ble_uuid_t uuid;      /**< Attribute UUID. */
    uplus_ble_gatts_op_t op;    /**< Type of write operation, see @ref BLE_GATTS_OPS. */
    uint16_t offset;
    uint8_t *data;
    uint16_t len;
} uplus_ble_gatts_write_t;

/*
 * UPLUS_BLE_GATTS_EVT_READ_PERMIT_REQ event callback parameters
 * NOTE: Stack SHOULD decide to reply the char value or refuse according to [permit]
 * */
typedef struct {
    uint16_t value_handle;  // char value handle
    uplus_ble_uuid_t  uuid;      /**< Attribute UUID. */
    uint16_t offset;             /**< Offset for the read operation. */
	uint8_t  *data;
    uint8_t  len;
} uplus_ble_gatts_read_t;

/*
 * GATTS event callback parameters union
 * */
typedef struct {
	uint16_t conn_handle;
	union {
		uplus_ble_gatts_write_t write;
		uplus_ble_gatts_read_t read;
	};
} uplus_ble_gatts_evt_param_t;


/**@brief GATT Authorization parameters. */
typedef struct
{
  uint16_t          gatt_status;        /**< GATT status code for the operation, see @ref BLE_GATT_STATUS_CODES. */
  uint8_t           update : 1;         /**< If set, data supplied in p_data will be used to update the attribute value.
                                             Please note that for @ref BLE_GATTS_AUTHORIZE_TYPE_WRITE operations this bit must always be set,
                                             as the data to be written needs to be stored and later provided by the application. */
  uint16_t          offset;             /**< Offset of the attribute value being updated. */
  uint16_t          len;                /**< Length in bytes of the value in p_data pointer, see @ref BLE_GATTS_ATTR_LENS_MAX. */
  uint8_t const    *p_data;             /**< Pointer to new value used to update the attribute value. */
} ble_gatts_authorize_params_t;

/**@brief GATT Read or Write Authorize Reply parameters. */
typedef struct
{
  uint8_t                               type;   /**< Type of authorize operation, see @ref BLE_GATTS_AUTHORIZE_TYPES. */
  union {
    ble_gatts_authorize_params_t        read;   /**< Read authorization parameters. */
    ble_gatts_authorize_params_t        write;  /**< Write authorization parameters. */
  } params;                                     /**< Reply Parameters. */
} ble_gatts_rw_authorize_reply_params_t;

/*GATTC related*/

/*
 * GATTC event
 * */
typedef enum {
    // this event generated in responses to a discover_primary_service procedure.
    UPLUS_BLE_GATTC_EVT_PRIMARY_SERVICE_DISCOVER_RESP = UPLUS_BLE_GATTC_EVT_BASE,
    // this event generated in responses to a discover_charicteristic_by_uuid
    // procedure.
    UPLUS_BLE_GATTC_EVT_CHR_DISCOVER_BY_UUID_RESP,
    // this event generated in responses to a discover_char_clt_cfg_descriptor
    // procedure.
    UPLUS_BLE_GATTC_EVT_CCCD_DISCOVER_RESP,
    // this event generated in responses to a read_charicteristic_value_by_uuid
    // procedure.
    UPLUS_BLE_GATTC_EVT_READ_CHAR_VALUE_BY_UUID_RESP,
    // this event generated in responses to a
    // write_charicteristic_value_with_response procedure.
    UPLUS_BLE_GATTC_EVT_WRITE_RESP,
	// this event is generated when peer gatts device send a notification.
	UPLUS_BLE_GATTC_EVT_NOTIFICATION,
	// this event is generated when peer gatts device send a indication.
	UPLUS_BLE_GATTC_EVT_INDICATION,
    // this event generated in responses to a discover_charicteristic procedure.
    UPLUS_BLE_GATTC_EVT_CHR_DISCOVER_RESP,
} uplus_ble_gattc_evt_t;

/*
 * UPLUS_BLE_GATTC_EVT_PRIMARY_SERVICE_DISCOVER_RESP event callback parameters
 * */
typedef struct {
    uplus_ble_handle_range_t primary_srv_range;
    uplus_ble_uuid_t srv_uuid;
    bool succ; // true : exist the specified primary service and return correctly
} uplus_ble_gattc_prim_srv_disc_rsp_t;

/*
 * UPLUS_BLE_GATTC_EVT_CHR_DISCOVER_BY_UUID_RESP event callback parameters
 * */
typedef struct {
    uint16_t     value_handle;
    uplus_ble_uuid_t char_uuid;
    bool succ; // true: exist the specified characteristic and return correctly
} uplus_ble_gattc_char_disc_rsp_t;

/*
 * UPLUS_BLE_GATTC_EVT_CCCD_DISCOVER_RESP event callback parameters
 * */
typedef struct {
    uint16_t desc_handle;
    bool succ; // true: exit cccd and return correctly
} uplus_ble_gattc_clt_cfg_desc_disc_rsp;

/*
 * UPLUS_BLE_GATTC_EVT_READ_CHAR_VALUE_BY_UUID_RESP event callback paramters
 * */
typedef struct {
    uint16_t char_value_handle;
    uint8_t len;
    uint8_t* data;
    bool succ; // true: exist the specified characteristic and return correctly
} uplus_ble_gattc_read_char_value_by_uuid_rsp;

/*
 * UPLUS_BLE_GATTC_EVT_WRITE_RESP event callback parameters
 *  */
typedef struct {
    bool succ;
} uplus_ble_gattc_write_rsp;

/*
 * UPLUS_BLE_GATTC_EVT_NOTIFICATION or UPLUS_BLE_GATTC_EVT_INDICATION event callback parameters
 *  */
typedef struct {
    uint16_t handle;
	uint8_t  len;
	uint8_t  *pdata;
} uplus_ble_gattc_notification_or_indication_t;

/*
 * GATTC callback parameters union
 * */
typedef struct {
	uint16_t conn_handle;
	union {
		uplus_ble_gattc_prim_srv_disc_rsp_t srv_disc_rsp;
		uplus_ble_gattc_char_disc_rsp_t char_disc_rsp;
		uplus_ble_gattc_read_char_value_by_uuid_rsp read_char_value_by_uuid_rsp;
		uplus_ble_gattc_clt_cfg_desc_disc_rsp clt_cfg_desc_disc_rsp;
		uplus_ble_gattc_write_rsp write_rsp;
		uplus_ble_gattc_notification_or_indication_t notification;
	};
} uplus_ble_gattc_evt_param_t;

/* TIMER related */
typedef void (*uplus_ble_timer_handler)(void*);

typedef enum {
    UPLUS_BLE_TIMER_SINGLE_SHOT,
    UPLUS_BLE_TIMER_REPEATED,
} uplus_ble_timer_mode;

/* IIC related */
typedef enum {
    IIC_100K = 1,
    IIC_400K,
} iic_freq_t;

typedef struct {
    uint8_t scl_port;
	uint8_t scl_pin;
	uint8_t scl_extra_conf;
	uint8_t sda_port;
    uint8_t sda_pin;
	uint8_t sda_extra_conf;
    iic_freq_t freq;
} iic_config_t;

typedef enum {
    IIC_EVT_XFER_DONE,
    IIC_EVT_ADDRESS_NACK,
    IIC_EVT_DATA_NACK
} iic_event_t;

typedef enum {
    UPLUS_BLE_SUCCESS      = 0x00,
    UPLUS_BLE_ERROR,
    UPLUS_BLE_ERR_INTERNAL,
    UPLUS_BLE_ERR_NOT_FOUND,
    UPLUS_BLE_ERR_NO_EVENT,
    UPLUS_BLE_ERR_NO_MEM,
    UPLUS_BLE_ERR_INVALID_ADDR,     // Invalid pointer supplied
    UPLUS_BLE_ERR_INVALID_PARAM,    // Invalid parameter(s) supplied.
    UPLUS_BLE_ERR_INVALID_STATE,    // Invalid state to perform operation.
    UPLUS_BLE_ERR_INVALID_LENGTH,
    UPLUS_BLE_ERR_DATA_SIZE,
    UPLUS_BLE_ERR_TIMEOUT,
    UPLUS_BLE_ERR_BUSY,
    UPLUS_BLE_ERR_RESOURCES,
    UPLUS_BLE_ERR_INVALID_CONN_HANDLE,
    UPLUS_BLE_ERR_ATT_INVALID_ATT_HANDLE,
    UPLUS_BLE_ERR_GAP_INVALID_BLE_ADDR,
    UPLUS_BLE_ERR_GATT_INVALID_ATT_TYPE,
    UPLUS_BLE_ERR_UNKNOWN, // other ble stack errors
} uplus_ble_status_e;

/** @defgroup BLE_GATT_STATUS_CODES GATT Status Codes
 * @{ */
#define BLE_GATT_STATUS_SUCCESS                           0x0000  /**< Success. */
#define BLE_GATT_STATUS_UNKNOWN                           0x0001  /**< Unknown or not applicable status. */
#define BLE_GATT_STATUS_ATTERR_INVALID                    0x0100  /**< ATT Error: Invalid Error Code. */
#define BLE_GATT_STATUS_ATTERR_INVALID_HANDLE             0x0101  /**< ATT Error: Invalid Attribute Handle. */
#define BLE_GATT_STATUS_ATTERR_READ_NOT_PERMITTED         0x0102  /**< ATT Error: Read not permitted. */
#define BLE_GATT_STATUS_ATTERR_WRITE_NOT_PERMITTED        0x0103  /**< ATT Error: Write not permitted. */
#define BLE_GATT_STATUS_ATTERR_INVALID_PDU                0x0104  /**< ATT Error: Used in ATT as Invalid PDU. */
#define BLE_GATT_STATUS_ATTERR_INSUF_AUTHENTICATION       0x0105  /**< ATT Error: Authenticated link required. */
#define BLE_GATT_STATUS_ATTERR_REQUEST_NOT_SUPPORTED      0x0106  /**< ATT Error: Used in ATT as Request Not Supported. */
#define BLE_GATT_STATUS_ATTERR_INVALID_OFFSET             0x0107  /**< ATT Error: Offset specified was past the end of the attribute. */
#define BLE_GATT_STATUS_ATTERR_INSUF_AUTHORIZATION        0x0108  /**< ATT Error: Used in ATT as Insufficient Authorization. */
#define BLE_GATT_STATUS_ATTERR_PREPARE_QUEUE_FULL         0x0109  /**< ATT Error: Used in ATT as Prepare Queue Full. */
#define BLE_GATT_STATUS_ATTERR_ATTRIBUTE_NOT_FOUND        0x010A  /**< ATT Error: Used in ATT as Attribute not found. */
#define BLE_GATT_STATUS_ATTERR_ATTRIBUTE_NOT_LONG         0x010B  /**< ATT Error: Attribute cannot be read or written using read/write blob requests. */
#define BLE_GATT_STATUS_ATTERR_INSUF_ENC_KEY_SIZE         0x010C  /**< ATT Error: Encryption key size used is insufficient. */
#define BLE_GATT_STATUS_ATTERR_INVALID_ATT_VAL_LENGTH     0x010D  /**< ATT Error: Invalid value size. */
#define BLE_GATT_STATUS_ATTERR_UNLIKELY_ERROR             0x010E  /**< ATT Error: Very unlikely error. */
#define BLE_GATT_STATUS_ATTERR_INSUF_ENCRYPTION           0x010F  /**< ATT Error: Encrypted link required. */
#define BLE_GATT_STATUS_ATTERR_UNSUPPORTED_GROUP_TYPE     0x0110  /**< ATT Error: Attribute type is not a supported grouping attribute. */
#define BLE_GATT_STATUS_ATTERR_INSUF_RESOURCES            0x0111  /**< ATT Error: Encrypted link required. */
#define BLE_GATT_STATUS_ATTERR_RFU_RANGE1_BEGIN           0x0112  /**< ATT Error: Reserved for Future Use range #1 begin. */
#define BLE_GATT_STATUS_ATTERR_RFU_RANGE1_END             0x017F  /**< ATT Error: Reserved for Future Use range #1 end. */
#define BLE_GATT_STATUS_ATTERR_APP_BEGIN                  0x0180  /**< ATT Error: Application range begin. */
#define BLE_GATT_STATUS_ATTERR_APP_END                    0x019F  /**< ATT Error: Application range end. */
#define BLE_GATT_STATUS_ATTERR_RFU_RANGE2_BEGIN           0x01A0  /**< ATT Error: Reserved for Future Use range #2 begin. */
#define BLE_GATT_STATUS_ATTERR_RFU_RANGE2_END             0x01DF  /**< ATT Error: Reserved for Future Use range #2 end. */
#define BLE_GATT_STATUS_ATTERR_RFU_RANGE3_BEGIN           0x01E0  /**< ATT Error: Reserved for Future Use range #3 begin. */
#define BLE_GATT_STATUS_ATTERR_RFU_RANGE3_END             0x01FC  /**< ATT Error: Reserved for Future Use range #3 end. */
#define BLE_GATT_STATUS_ATTERR_CPS_WRITE_REQ_REJECTED     0x01FC  /**< ATT Common Profile and Service Error: Write request rejected. */
#define BLE_GATT_STATUS_ATTERR_CPS_CCCD_CONFIG_ERROR      0x01FD  /**< ATT Common Profile and Service Error: Client Characteristic Configuration Descriptor improperly configured. */
#define BLE_GATT_STATUS_ATTERR_CPS_PROC_ALR_IN_PROG       0x01FE  /**< ATT Common Profile and Service Error: Procedure Already in Progress. */
#define BLE_GATT_STATUS_ATTERR_CPS_OUT_OF_RANGE           0x01FF  /**< ATT Common Profile and Service Error: Out Of Range. */
/** @} */


typedef void (*uplus_ble_handler_t) (void* arg);

typedef enum{
	UPLUS_BLE_ARCH_EVT_GATTS_SRV_INIT_CMP,
	UPLUS_BLE_ARCH_EVT_RECORD_WRITE,
	UPLUS_BLE_ARCH_EVT_RECORD_DELETE,
	UPLUS_BLE_ARCH_EVT_DEV_CTRL,
} uplus_ble_arch_event_t;

typedef struct{
	uplus_ble_status_e status;
	uplus_ble_gatts_db_t *p_gatts_db;
}uplus_ble_arch_gatts_srv_init_cmp_t;

typedef struct{
	uint16_t data_type;
	uplus_ble_status_e status;
}uplus_ble_arch_record_t;

typedef struct
{
	uint32_t sn;
	uint8_t *data;
	uint32_t data_len;
}uplus_ctrl_param_t;

typedef struct
{
	uplus_ble_cmd_type_t cmd_type;
	uplus_ctrl_param_t ctrl_param;
}uplus_ble_arch_ctrl_t;

typedef struct{
	union {
		uplus_ble_arch_gatts_srv_init_cmp_t srv_init_cmp;
		uplus_ble_arch_record_t record;
		uplus_ble_arch_ctrl_t ctrl;
	};
}uplus_ble_arch_evt_param_t;

//Added by christin 2020_4_9 begin

/**@brief Connection Parameters Module event handler type. */
/**@brief Connection Parameters Module event type. */
typedef enum
{
    BLE_CONN_PARAMS_EVT_FAILED,                                     //!< Negotiation procedure failed.
    BLE_CONN_PARAMS_EVT_SUCCEEDED                                   //!< Negotiation procedure succeeded.
} ble_conn_params_evt_type_t;

/**@brief Connection Parameters Module event. */
typedef struct
{
    ble_conn_params_evt_type_t evt_type;                            //!< Type of event.
    uint16_t                   conn_handle;                         //!< Connection the event refers to.
} ble_conn_params_evt_t;
typedef void (*ble_conn_params_evt_handler_t) (ble_conn_params_evt_t * p_evt);

/**@brief Type definition for error handler function that will be called in case of an error in
 *        a service or a service library module. */
typedef void (*ble_srv_error_handler_t) (uint32_t nrf_error);
/**@brief Connection Parameters Module init structure. This contains all options and data needed for
 *        initialization of the connection parameters negotiation module. */
typedef struct
{
    uplus_ble_gap_conn_param_t *       p_conn_params;                    //!< Pointer to the connection parameters desired by the application. When calling ble_conn_params_init, if this parameter is set to NULL, the connection parameters will be fetched from host.
    uint32_t                           first_conn_params_update_delay;   //!< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (in number of timer ticks).
    uint32_t                           next_conn_params_update_delay;    //!< Time between each call to sd_ble_gap_conn_param_update after the first (in number of timer ticks). Recommended value 30 seconds as per BLUETOOTH SPECIFICATION Version 4.0.
    uint8_t                            max_conn_params_update_count;     //!< Number of attempts before giving up the negotiation.
    uint16_t                           start_on_notify_cccd_handle;      //!< If procedure is to be started when notification is started, set this to the handle of the corresponding CCCD. Set to BLE_GATT_HANDLE_INVALID if procedure is to be started on connect event.
    bool                               disconnect_on_fail;               //!< Set to TRUE if a failed connection parameters update shall cause an automatic disconnection, set to FALSE otherwise.
    ble_conn_params_evt_handler_t      evt_handler;                      //!< Event handler to be called for handling events in the Connection Parameters.
    ble_srv_error_handler_t            error_handler;                    //!< Function to be called in case of an error.
} ble_conn_params_init_t;
//Added by christin 2020_4_9 end

/**@brief Queued Write Example Service event types. */
typedef enum
{
    UPLUS_BLE_QWRS_CHECK_RCVD_DATA,  /* On this event, the application shall only verify if the data are correct.*/
    UPLUS_BLE_QWRS_NEW_DATA_RCVD,    /* On this event, the application can act upon the received data*/
} uplus_ble_qwrs_evt_type;

/**@brief Queued Write Example Service event. */
typedef struct
{
    uplus_ble_qwrs_evt_type evt_type;                        //!< Type of event.
    uint16_t              rcv_length;
    uint8_t               rcvd_data[UPLUS_BLE_QWRS_MAX_RCV_SIZE];
} uplus_ble_qwrs_evt_t;

typedef void (*uplus_ble_gap_callback_t)(uplus_ble_gap_evt_t evt,
    uplus_ble_gap_evt_param_t* param);

typedef int32_t (*uplus_ble_gatts_callback_t)(uplus_ble_gatts_evt_t evt,
    uplus_ble_gatts_evt_param_t* param);

typedef void (*uplus_ble_gattc_callback_t)(uplus_ble_gattc_evt_t evt,
    uplus_ble_gattc_evt_param_t* param);
#if 0
typedef uint16_t (*uplus_ble_qwrs_callback_t)(uplus_ble_qwrs_evt_t *p_evt,
    uint16_t attr_handle);

typedef void (*uplus_ble_arch_callback_t)(uplus_ble_arch_event_t evt,
		uplus_ble_arch_evt_param_t* param);
#endif

#endif
