#ifndef __UH_CONFIG_H__
#define __UH_CONFIG_H__

#include "uplus_ble_type.h"

#ifdef P_JL_AC632N
#define UPLUS_BLE_SDK_VERSION	   ("ble_1.4.00")
#endif

#ifdef P_BK_3431
#define UPLUS_BLE_SDK_VERSION	   ("ble_1.3.00")
#endif

#ifdef P_TELINK_8258
#define UPLUS_BLE_SDK_VERSION	   ("ble_1.2.00")
#endif

#define SOFTWARE_TYPE              ("BLE_SDK")

#pragma pack(1)
typedef struct {
	/* basic 84 bytes */
	uint8_t 	flag;                 	//flash flag
	uint8_t 	dev_id[16];
	uint8_t 	dev_id_len;
	uint8_t 	local_key[64+1];
	uint8_t 	local_key_len;
	/* ota 113 bytes */
	uint8_t 	app_version[20]; 		// 用户软件包版本
	uint8_t 	reset_flag; 			//0x0 normal//0x01 ota rom_patch reset // 0x02 ota app_patch reset // 0x03 ota user reset
	uint8_t		ota_mode; 				// 0:normal ; 1: app-sdk upgrade; 2:user-mcu upgrade
	uint32_t	fm_head_offset; 		// sub-firmware offset;
	uint32_t	ota_progress;
	uint16_t	ota_session;
	uint8_t 	iv_attr[16];
	uint8_t		dec_key[64];
	uint8_t		key_len;
	/* sn  2 bytes*/
	uint8_t 	sn;
	uint8_t 	sn_save;

	uint8_t		reserved[57]; /*reserved*/
} uplus_ble_cfg_ctrl_t;
#pragma pack()

void uh_config_init(uplus_ble_cfg_ctrl_t* config);

int  uh_config_save(uplus_ble_cfg_ctrl_t* config);

void uh_config_factory_reset(uplus_ble_cfg_ctrl_t* config, int force_reboot);

void uh_config_sync(uplus_ble_cfg_ctrl_t* config);

#endif

