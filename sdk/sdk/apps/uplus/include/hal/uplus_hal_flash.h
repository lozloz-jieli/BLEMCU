/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_HAL_FLASH_H__
#define __UPLUS_HAL_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif


int32_t uplus_flash_init(void);

int32_t uplus_flash_deinit(void);

uint32_t uplus_flash_sector_write(uint32_t addr, uint8_t *src, uint32_t num_bytes);

uint32_t uplus_flash_write(uint32_t addr, uint8_t *src, uint32_t num_bytes);

uint32_t uplus_flash_read(uint32_t offset, uint8_t *des, uint32_t num_bytes);

uint32_t uplus_flash_erase_sector(uint32_t addr);

uint32_t uplus_ota_fw_write(uint32_t offset, uint8_t *src, uint32_t num_bytes);

uint32_t uplus_ota_fw_read(uint32_t offset, uint8_t *des, uint32_t num_bytes);

uint32_t uplus_ota_fw_erase(uint32_t offset);

enum{
	UPLUS_OTA_FW_UPGRADE_OK = 0x00,
	UPLUS_OTA_FW_UPGRADE_FAILED,
};
int32_t uplus_ota_fw_set_flag(uint8_t fw_flag);

#ifdef __cplusplus
}
#endif

#endif
