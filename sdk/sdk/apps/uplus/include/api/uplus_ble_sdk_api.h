/*
 *  uplus_ble_sdk_api.h
 *
 *  Created on: 2020年12月3日
 *  Author: 
 */

#ifndef __UPLUS_BLE_SDK_API_H__
#define __UPLUS_BLE_SDK_API_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief BLE SDK ERROR
 */
#define UPLUS_BLE_SDK_ERR_OK 	 			0
#define	UPLUS_BLE_SDK_ERR_COMMON 			-1
#define	UPLUS_BLE_SDK_ERR_MALLOC 			-2
#define	UPLUS_BLE_SDK_ERR_INVALID_PARAM 	-3
#define	UPLUS_BLE_SDK_ERR_TIMEOUT 			-4
#define	UPLUS_BLE_SDK_ERR_NO_KEY 			-99

/**
 * \brief 配置信息
 */
typedef struct {
	unsigned char product_type_id[65]; /*product type id.  e.g., "00000000000000008080000000041410*/
	signed char ble_dev_name[20];/*ble name .e.g. "UHOME-%d%d"*/
	signed char overall_model_number[20]; /*成品编码，用于OTA升级*/
	signed char overall_version[20];
	signed char app_version[20];
	signed char sw_type[20];/*software type .e.g., "uplugSDK1.0"*/
	signed char deviceKey_algorithm;/*DeviceKey算法: MD5 0x01; SHA256 0x02*/
	signed char deviceKey[32];/*DeviceKey*/
	unsigned char sw_ver[8+1];
	unsigned char hw_ver[8+1];
	signed char hw_type[20];
	unsigned char product_code[14];
	signed char e_proto_ver[8];
	unsigned char app_type_code[4];
}init_cfg_para_t;

/**
 * \brief 启动sdk
 * \param [in] init_cfg 配置信息
 * \return 成功返回0， 其他值表示失败
 */
int uplus_ble_sdk_start(init_cfg_para_t *init_cfg);

/**
 * \brief 更新整机固件版本
 * \param [in] version 版本信息
 * \return 成功返回0， 其他值表示失败
 */
int uplus_ble_sdk_overall_version_update(char* version);

/**
 * \brief 更新sdk配置信息
 * \param [in] init_cfg 配置信息
 * \return 成功返回0， 其他值表示失败
 */
int uplus_ble_sdk_update(init_cfg_para_t* init_cfg);

/**
 * \brief  系统重启
 * \return 无
 */
void uplus_ble_sdk_system_reset(void);

/**
 * \brief  获取sdk版本
 * \return 返回具体版本信息
 */
char* uplus_ble_sdk_ver_get(void);

/**
 * \brief  用户flash存储区初始化
 * \return 成功返回0， 其他值表示失败
 */
int uplus_ble_sdk_user_config_init(void);

/**
 * \brief  用户存储数据读取
 * \param [out] conf 用户存储数据
 * 		  [out] len 用户存储数据长度（len <= 512byte）
 * \return 成功返回0， 其他值表示失败
 */
int uplus_ble_sdk_user_config_read(unsigned char *conf, unsigned int len);

/**
 * \brief  用户存储数据写
 * \param [in] conf 用户存储数据
 * 		  [in] len 用户存储数据长度（len <= 512byte）
 * \return 成功返回0， 其他值表示失败
 */
int uplus_ble_sdk_user_config_write(unsigned char *conf, unsigned int len);

/**
 * \brief  用户存储数据读取
 * \param [in] key  加密key
 * 		  [in] salt slat值
 * 		  [in] user_dev_name 设备名字
 * 		  [in] deviceKey_algorithm 设备key算法
 * 		  [in] mac 设备mac地址
 * 		  [out] deviceKey 设备key
 * \return 成功返回0， 其他值表示失败
 */
int8_t uplus_ble_sdk_deviceKey_gets(uint8_t key[33], uint8_t salt[9], uint8_t user_dev_name, int8_t deviceKey_algorithm, uint8_t mac[6], int8_t* deviceKey);

/**
 * \brief  配置通知回调函数类型
 * \param [in] state 配置状态
 * 		  [in] err 配置错误码
 *        [IN] cb_param    回调函数回传参数置
 * \return 无
 */
typedef void (*uplus_ble_sdk_cb_config_notify)(int state, int err, void *cb_param);

/**
 * \brief  注册配置通知回调函数
 * \param  [IN] cb          回调函数
 *         [IN] cb_param    回调函数回传参数
 * \return 成功返回0， 其他值表示失败
 */
int uplus_ble_sdk_config_reg_notify_cb(uplus_ble_sdk_cb_config_notify cb, void* cb_param);

/**
 * \brief  启动ble配置
 * \return 无
 */
int uplus_ble_sdk_config_start(void);

/**
 * \brief  恢复出厂设置
 * \return 无
 */
void   uplus_ble_sdk_recover_factory_mode(void);

/**
 * \brief ota宏定类型
 */
#define UPLUS_BLE_SDK_OTA_CRC16_LEN					2
#define UPLUS_BLE_SDK_OTA_DIGEST_LEN				32
#define UPLUS_BLE_SDK_OTA_BASIC_STR_LEN				20
#define UPLUS_BLE_SDK_OTA_MODE_ID_LEN				20
#define UPLUS_BLE_SDK_OTA_SOFTWARE_TYPE_LEN			20
#define UPLUS_BLE_SDK_OTA_VERSION_LEN				14
#define UPLUS_BLE_SDK_OTA_USER_DEFINED_DATA_LEN		32

/**
 * \brief ota返回值
 */
typedef enum {
	UPLUS_BLE_SDK_OTA_RET_OK = 0,
	UPLUS_BLE_SDK_OTA_RET_ERROR = -1,
	UPLUS_BLE_SDK_OTA_RET_ERR_OTA_NOT_INITED = -2,
	UPLUS_BLE_SDK_OTA_RET_INVALID_PARAMETER = -3,
	UPLUS_BLE_SDK_OTA_RET_INVALID_SESSION = -4,
	UPLUS_BLE_SDK_OTA_RET_MODE_NOT_SUPPORT = -5,
	UPLUS_BLE_SDK_OTA_RET_NO_FIRMWARE_DOWNLOADED = -6,
} UPLUS_BLE_SDK_OTA_RET;

/**
 * \brief ota错误码
 */
typedef enum {
	UPLUS_BLE_SDK_OTA_ERRNUM_OK = 0,
	UPLUS_BLE_SDK_OTA_ERRNUM_SESSION_ERR,
	UPLUS_BLE_SDK_OTA_ERRNUM_TIME_OUT,
	UPLUS_BLE_SDK_OTA_ERRNUM_IMAGE_LEN_ERR,
	UPLUS_BLE_SDK_OTA_ERRNUM_IMAGE_DATA_ERR  = 5,
	UPLUS_BLE_SDK_OTA_ERRNUM_DOWNLOAD_TIMEOUT,
	UPLUS_BLE_SDK_OTA_ERRNUM_CRC_ERR,
	UPLUS_BLE_SDK_OTA_ERRNUM_DIGEST_ERR,
	UPLUS_BLE_SDK_OTA_ERRNUM_WRITE_FLASH_ERR,
} UPLUS_BLE_SDK_OTA_ERRNUM_EN;

/**
 * \brief ota状态
 */
typedef enum {
	UPLUS_BLE_SDK_OTA_STATUS_NORMAL,
	UPLUS_BLE_SDK_OTA_STATUS_CONTINUE, //保留，设备APP升级无重启需求。
	UPLUS_BLE_SDK_OTA_STATUS_FINISH
} UPLUS_BLE_SDK_OTA_STATUS_EN;

/**
 * \brief  ota模式
 */
typedef enum {
	UPLUS_BLE_SDK_OTA_MODE_UNKNOWN = 0,
	UPLUS_BLE_SDK_OTA_MODE_SDK = 1,		// DEV-SDK将处理所有OTA流程，成功后通过事件通知DEV-APP。
	UPLUS_BLE_SDK_OTA_MODE_USER,		// DEV-SDK将下载子固件包到flash，下载成功后， 通知用户读取并自行升级相关子固件。
	UPLUS_BLE_SDK_OTA_MODE_MANUAL		// 保留
} UPLUS_BLE_SDK_OTA_MODE_EN;

/**
 * \brief ota事件通知类型
 */
typedef enum {
	UPLUS_BLE_SDK_OTA_EVT_REQ,
	UPLUS_BLE_SDK_OTA_EVT_SDK_UPGRADING,
	UPLUS_BLE_SDK_OTA_EVT_DOWNLOAD_COMPLETE,
	UPLUS_BLE_SDK_OTA_EVT_FINISHED,
}UPLUS_BLE_SDK_BLE_OTA_TYPE_EN;

/**
 * \brief 固件类型
 */
typedef enum {
	UPLUS_OTA_TYPE_MCU = 1,
	UPLUS_OTA_TYPE_VOICE = 2,
	UPLUS_OTA_TYPE_WIFI = 3,
	UPLUS_OTA_TYPE_BLE_MESH = 4,
	UPLUS_OTA_TYPE_USERDEF = 5,
	UPLUS_OTA_TYPE_BLE_SDK = 6,
} UPLUS_BLE_SDK_OTA_FIRMWARE_TYPE_EN;

#pragma pack(1)
/**
 * \brief 整机固件基础信息
 */
typedef struct _uplus_sdk_ota_basic_info_ {
	unsigned short	pkg_type;
	unsigned short	pkg_verno;
	unsigned short	pkg_crc;
	unsigned char	pkg_digest[UPLUS_BLE_SDK_OTA_DIGEST_LEN];
	unsigned int	firmware_desc_offset;
	unsigned int	firmware_desc_len;
	unsigned short	firmware_desc_crc;
	unsigned char	reserved[12];
	unsigned int	firmware_content_len;
}uplus_ble_sdk_ota_pkg_basic_info_t;

/**
 * \brief 整机固件描述信息
 */
typedef struct _uplus_ble_sdk_ota_firmware_desc_ {
	unsigned char	mode_id[UPLUS_BLE_SDK_OTA_MODE_ID_LEN];
	unsigned char	ver_type[UPLUS_BLE_SDK_OTA_SOFTWARE_TYPE_LEN];
	unsigned char	version[UPLUS_BLE_SDK_OTA_VERSION_LEN];
	unsigned char	custom_data[UPLUS_BLE_SDK_OTA_USER_DEFINED_DATA_LEN];
	unsigned char	reserved[30];
	unsigned short	sub_firmware_desc_crc;
	unsigned int	sub_firmware_desc_offset;
	unsigned int	sub_firmware_desc_len;
	unsigned short	sub_firmware_num;
} uplus_ble_sdk_ota_firmware_desc_t;

/**
 * \brief 整机子固件描述信息
 */
typedef struct _uplus_ble_sdk_ota_sub_firmware_desc_ {
	short	type; // @uplus_sdk_ota_firmware_type_e; like UPLUS_OTA_TYPE_XXX
	unsigned char	sw_id[UPLUS_BLE_SDK_OTA_SOFTWARE_TYPE_LEN];
	unsigned char	version[UPLUS_BLE_SDK_OTA_VERSION_LEN];
	unsigned int	offset;
	unsigned int   data_len;
	unsigned short	crc;
	unsigned char	digest[UPLUS_BLE_SDK_OTA_DIGEST_LEN];
	unsigned char	custom_data[UPLUS_BLE_SDK_OTA_USER_DEFINED_DATA_LEN];
	unsigned char	reserved[18];
} uplus_ble_sdk_ota_sub_firmware_desc_t;

/**
 * \brief  固件包整包定义
 */
typedef struct _uplus_ble_sdk_ota_pkt_desc_ {
	uplus_ble_sdk_ota_firmware_desc_t			fm_desc;
	uplus_ble_sdk_ota_sub_firmware_desc_t		sub_fm_desc[0];
}uplus_ble_sdk_ota_pkt_desc_t;

#pragma pack()
/**
 * \brief ota事件参数
 */
typedef struct _uplus_ble_sdk_ota_evt_ {
	UPLUS_BLE_SDK_BLE_OTA_TYPE_EN	type;
	int					error_num;
	unsigned int					session;
	unsigned int					reserved;
	union  {
		/* data */
		uplus_ble_sdk_ota_pkt_desc_t			*	OTA_REQ_desc;
		uplus_ble_sdk_ota_pkt_desc_t			*	OTA_SDK_UPGRADING_info;
		uplus_ble_sdk_ota_sub_firmware_desc_t	*	OTA_DOWNLOAD_info;
		uplus_ble_sdk_ota_sub_firmware_desc_t	*	OTA_FINISHED_info;
	} evt;
} uplus_ble_sdk_ota_evt_t;

/**
 * \brief ota事件通知回调函数类型
 * \param [in] evt 事件参数
 *        [in] user_data 用户回传参数
* \return 成功返回true， 其他值返回false
 */
typedef int (*uplus_ble_sdk_ota_event_notify)(const uplus_ble_sdk_ota_evt_t *evt, void * userdata);

/**
 * \brief  注册ota事件通知函数
 * \param [in] cb 事件通知函数
 *        [in] user_data 用户回传参数
 * \return 成功返回0， 其他值返回失败值
 */
int uplus_ble_sdk_ota_evt_register(uplus_ble_sdk_ota_event_notify cb, void * userdata);

/**
 * \brief  获取当前mtu
 * \param [in] conn_handle 连接句柄
 * \return 返回UPLUS_BLE_SDK_OTA_RET_OK表示成功，其他值表示失败
 */
UPLUS_BLE_SDK_OTA_RET uplus_ble_sdk_ota_status_notify(UPLUS_BLE_SDK_OTA_STATUS_EN status, UPLUS_BLE_SDK_OTA_ERRNUM_EN err_num);

/**
 * \brief  获取当前mtu
 * \param [in] conn_handle 连接句柄
 * \return 返回UPLUS_BLE_SDK_OTA_RET_OK表示成功，其他值表示失败
 */
UPLUS_BLE_SDK_OTA_RET uplus_ble_sdk_ota_upgrade_firmware(unsigned int session, UPLUS_BLE_SDK_OTA_MODE_EN mode, uplus_ble_sdk_ota_sub_firmware_desc_t *sub_firmware_desc);

/**
 * \brief  读取ota固件
 * \param [in] offset 数据偏移
 *        [in] buffer 数据缓存
 * 		  [in] buffer 数据长度
 * \return 成功返回0， 其他值返回失败值
 */
int uplus_ble_sdk_ota_read_firmware(unsigned int offset, unsigned char *buffer, unsigned int len);

/**
 * \brief 事件类型
 */
typedef enum{
	UPLUS_BLE_SDK_ARCH_EVT_DEV_CTRL,
} uplus_ble_sdk_epp_rx_event_t;

/**
 * \brief 命令类型
 */
typedef enum {
	UPLUS_BLE_SDK_CMD_TYPE_CTRL,/*控制*/
	UPLUS_BLE_SDK_CMD_TYPE_STATUS,/*控制响应状态*/
	UPLUS_BLE_SDK_CMD_TYPE_ALARM,/*告警*/
	UPLUS_BLE_SDK_CMD_TYPE_STOP_ALARM,/*停止告警*/
	UPLUS_BLE_SDK_CMD_TYPE_STOP_ALARM_ACK,
	UPLUS_BLE_SDK_CMD_TYPE_REPORT,/*状态上报*/
	UPLUS_BLE_SDK_CMD_TYPE_GET_ALARM,/*获取告警*/
	UPLUS_BLE_SDK_CMD_TYPE_GET_ALARM_ACK,/*获取告警响应*/
	UPLUS_BLE_SDK_CMD_TYPE_SINGLE_ATTRIBUTE_REPORT,/*单属性上报*/
	UPLUS_BLE_SDK_CMD_TYPE_EVENT_REPORT,/*事件*/
	UPLUS_BLE_SDK_CMD_TYPE_TRANSMIT_TRANSPARENTLY,/*透传*/
	UPLUS_BLE_SDK_CMD_TYPE_TRANSMIT_TRANSPARENTLY_ACK,
	UPLUS_BLE_SDK_CMD_TYPE_ACK,
	UPLUS_BLE_SDK_CMD_TYPE_INVILD_ACK,
	UPLUS_BLE_SDK_CMD_TYPE_ERR
} uplus_ble_sdk_cmd_type_e;

/**
 * \brief 控制数据类型
 */
typedef struct {
	unsigned int sn;		/*控制和控制响应消息的sn，单上报消息为0*/
	unsigned char *data;	/*epp的payload数据，不包含FF FF Epp头和crc和校验，不进行epp加密，广播态上报间隔需>=500ms*/
	unsigned int data_len;	/*epp得payload长度，连接态状态上报或控制响应上报最大长度为245，其它情况最大长度为244*/
} uplus_ble_sdk_ctrl_param_t;

/**
 * \brief 控制参数类型
 */
typedef struct {
	uplus_ble_sdk_cmd_type_e cmd_type;
	uplus_ble_sdk_ctrl_param_t ctrl_param;
} uplus_ble_sdk_arch_ctrl_t;

/**
 * \brief 接收事件参数
 */
typedef struct{
	union {
		uplus_ble_sdk_arch_ctrl_t ctrl;
	};
} uplus_ble_sdk_epp_rx_evt_param_t;

/**
 * \brief  epp接收回调函数类型
 * \param [in] evt 事件类型
 * 		  [in] param 事件参数
 * \return 无
 */

typedef void (*uplus_ble_sdk_epp_rx_callback_t)(uplus_ble_sdk_epp_rx_event_t evt, uplus_ble_sdk_epp_rx_evt_param_t* param);

/**
 * \brief  注册epp接收函数
 * \param [in] conn_handle 连接句柄
 * \return 成功返回0， 其他值返回失败值
 */
char uplus_ble_sdk_epp_rx_register(uplus_ble_sdk_epp_rx_callback_t func_cb);

/**
 * \brief 发送数据标志
 */
typedef enum {
    UPLUS_SEND_ONLY = 1, 	/*仅发送数据*/
	UPLUS_DATA_STORAGE_ONLY,/*仅存储数据到历史数据*/
    UPLUS_SEND_AND_STORAGE	/*发送并储数据到历史数据*/
} uplus_send_storage_flg_e;

/**
 * \brief epp发送函数
 * \param [in] flg 发送标志
 * 		  [in] cmd_type 命令类型
 *		  [in] param 发送参数
 *		  [in] time_stamp 发送时间戳
 * \return 成功返回0， 其他值返回失败值
 */
signed char uplus_ble_sdk_epp_tx_func (uplus_send_storage_flg_e flg, uplus_ble_sdk_cmd_type_e cmd_type, uplus_ble_sdk_ctrl_param_t *param, unsigned int time_stamp);

/**
 * \brief 连接状态回调函数类型
 */
typedef void (*uplus_ble_sdk_connect_state_notice_cb)(unsigned char conn_state, void* cb_param);

/**
 * \brief  注册连接状态回调函数
 * \param  [in] cb 回调函数
 * 		   [in] cb_param 回传参数
 * \return 成功返回0， 其他值返回失败
 */
int uplus_ble_sdk_conn_state_reg_notify_cb(uplus_ble_sdk_connect_state_notice_cb cb, void* cb_param);

/**
 * \brief 获取蓝牙连接状态
 * \return 返回蓝牙连接状态
 */
unsigned char uplus_ble_sdk_get_conn_state(void);

/**
 * \brief  获取时间回调函数类型
 * \param [in] err_num 错误码
 * 		  [in] ts 时间
 * \return 无
 */
typedef void (*uplus_ble_sdk_time_ts_cb)(int err_num, unsigned int ts);

/**
 * \brief  注册时间获取通知函数
 * \param [in] param 时间参数
 * 		  [in] cb_param 回传参数柄
 * \return 成功返回0， 其他值返回失败
 */
int uplus_ble_sdk_time_ts_reg_notify_cb(uplus_ble_sdk_time_ts_cb cb, void* cb_param);

/**
 * \brief dlps模式
 */
typedef enum {
	UPLUS_BLE_SDK_DLPS_CONFIG_ENABLE 	= 0x01,
	UPLUS_BLE_SDK_DLPS_CONFIG_DISABLE 	= 0x02,
} uplus_ble_sdk_dlps_mode_e;

/**
 * \brief dlps设置参数结构体类型
 */
typedef struct {
	uplus_ble_sdk_dlps_mode_e mode;
} uplus_ble_sdk_dlps_config_t;

/**
 * \brief  dlps配置参数
 * \param [in] param 配置参数
 * 		  [in] cb_param 回传参数
 * \return 无
 */
typedef void (*uplus_ble_sdk_dlps_config_cb)(uplus_ble_sdk_dlps_config_t *param);

/**
 * \brief  注册dlps配置回调函数
 * \param  [in] cb 回调函数
 * 		   [in] cb_param 回调函数回传参数
 * \return 成功返回0， 其他值返回失败
 */
int uplus_ble_sdk_dlps_reg_config_cb(uplus_ble_sdk_dlps_config_cb cb);

/**
 * \brief 时间枚举类型
 */
typedef enum {
    EVENT_LOCALKEY,
    EVENT_TS,
    EVENT_OTHER,
} time_type_e;

/**
 * \brief 设置系统时间
 * \param [in] ts 时间参数
 * 		  [in] time_type 时间类型
 * \return 成功返回0， 其他值返回失败
 */
 char uplus_ble_sdk_time_ts_get(unsigned int *pts);

/**
 * \brief 设置系统时间
 * \param [in] ts 时间参数
 * 		  [in] time_type 时间类型
 * \return 成功返回0， 其他值返回失败
 */
char uplus_ble_sdk_time_ts_set(unsigned int ts, time_type_e time_type);

/**
 * \brief 获取基准时间
 * \param [out] pts 时间参数
 * \return 成功返回0， 其他值返回失败
 */
char uplus_ble_sdk_time_ts_base_get(unsigned int *pts);

/**
 * \brief 设置基准时间
 * \param [in] ts 时间参数
 * \return 成功返回0， 其他值返回失败
 */
char uplus_ble_sdk_time_ts_base_set(unsigned int ts);

#ifdef __cplusplus
}
#endif

#endif
