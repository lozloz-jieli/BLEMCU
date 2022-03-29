/*
 * uh_record_api.h
 *
 *  Created on: 2021-6-30
 *      Author: 21024173
 */

#ifndef UH_RECORD_API_H_
#define UH_RECORD_API_H_


#include "include/hal/uplus_hal_flash.h"
#include "include/api/uplus_ble_sdk_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

//flash cross page read,but can not cross page write
#define UPLUS_FLASH_OLD_FIREWARE_BIN_BASE_ADDR 		(0x00000) //old fireware bin,0x00000~0x30000
#define UPLUS_FLASH_NEW_FIREWARE_BIN_BASE_ADDR 		(0x40000) //new fireware bin,0x40000~0x70000
#define UPLUS_FSTORAGE_INDEX_DEV_INFO_RECORD_BASE 	(0x30000) //data area 1, 0x30000~0x40000

#ifdef P_JL_AC632N
#define UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD		(0x1000) //data para info, uplus_ble_history_param_t,(64 byte size)
#define UPLUS_FSTORAGE_INDEX_HISTORY_DATA_RECORD_BASE 	(UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD+0x100) //user data, (16KB-256 byte size)
#define FLASH_ALL_SIZE      ( UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD+(1024 * 16) )	/*16KB for user history data*/
#define HIS_DATA_SIZE       (FLASH_ALL_SIZE - UPLUS_FSTORAGE_INDEX_HISTORY_DATA_RECORD_BASE)
#endif

#ifdef P_BK_3431
#define UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD		(0x43000) //data para info, uplus_ble_history_param_t,(64 byte size)
#define UPLUS_FSTORAGE_INDEX_HISTORY_DATA_RECORD_BASE 	(UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD+0x100) //user data, (16KB-256 byte size)
#define FLASH_ALL_SIZE      ( UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD+(1024 * 4) )	/*4KB for user history data*/
#define HIS_DATA_SIZE       (FLASH_ALL_SIZE - UPLUS_FSTORAGE_INDEX_HISTORY_DATA_RECORD_BASE)
#endif

#ifdef P_TELINK_8258
#define UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD		(0x70000) //data para info, uplus_ble_history_param_t,(64 byte size)
#define UPLUS_FSTORAGE_INDEX_HISTORY_DATA_RECORD_BASE 	(UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD+0x100) //user data, (16KB-256 byte size)
#define FLASH_ALL_SIZE      ( UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD+(1024 * 16) )	/*16KB for user history data*/
#define HIS_DATA_SIZE       (FLASH_ALL_SIZE - UPLUS_FSTORAGE_INDEX_HISTORY_DATA_RECORD_BASE)
#endif

#define UPLUS_FSTORAGE_INDEX_BASE         				  	UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD
#define UPLUS_FSTORAGE_INDEX_DEV_INFO     				   	UPLUS_FSTORAGE_INDEX_DEV_INFO_RECORD_BASE//this title for uplus_ble_dev_cfg_t, TODO
#define UPLUS_FSTORAGE_INDEX_PARAM                   	   	UPLUS_FSTORAGE_INDEX_PARAM_INFO_RECORD//this title for uplus_ble_history_param_t
#define UPLUS_FSTORAGE_INDEX_HISTORY_RECORD_BASE     		(UPLUS_FSTORAGE_INDEX_HISTORY_DATA_RECORD_BASE)

#define  UPLUS_TIME_DATA_SIZE   8;
#define  UPLUS_SN_DATA_SIZE     1;
#define  UPLUS_CRC_DATA_SIZE    2;
#define  UPLUS_DATA_LEN_SIZE    1;

typedef enum{
    UPLUS_HIST_DEV_INFO = 0,
    UPLUS_HIST_DATA_INDEX_PARAM,
	UPLUS_HIST_DATA_RECORD
}uplus_hist_data_type_e;

typedef enum{
    UPLUS_HIST_EVT_INIT,          //!< Event for flash_init.
    UPLUS_HIST_EVT_WRITE,        //!< Event for flash_record_write and flash_record_write_reserved.
    UPLUS_HIST_EVT_UPDATE,      //!< Event for flash_record_update.
    UPLUS_HIST_EVT_DEL_RECORD, //!< Event for flash_record_delete.
    UPLUS_HIST_EVT_DEL_FILE,    //!< Event for flash_file_delete.
    UPLUS_HIST_EVT_GC           //!< Event for flash_gc.
}uplus_hist_operation_type_e;

typedef enum {
    UPLUS_HIST_SUCCESS,                    //!< The operation completed successfully.
    UPLUS_HIST_ERR_OPERATION_TIMEOUT,  //!< Error. The operation timed out.
    UPLUS_HIST_ERR_NOT_INITIALIZED,      //!< Error. The module has not been initialized.
    UPLUS_HIST_ERR_UNALIGNED_ADDR,     //!< Error. The input data is not aligned to a word boundary.
    UPLUS_HIST_ERR_INVALID_ARG,          //!< Error. The parameter contains invalid data.
    UPLUS_HIST_ERR_NULL_ARG,             //!< Error. The parameter is NULL.
    UPLUS_HIST_ERR_NO_OPEN_RECORDS,     //!< Error. The record is not open, so it cannot be closed.
    UPLUS_HIST_ERR_NO_SPACE_IN_FLASH,   //!< Error. There is no space in flash memory.
    UPLUS_HIST_ERR_NO_SPACE_IN_QUEUES,  //!< Error. There is no space in the internal queues.
    UPLUS_HIST_ERR_RECORD_TOO_LARGE,   //!< Error. The record exceeds the maximum allowed size.
    UPLUS_HIST_ERR_NOT_FOUND,            //!< Error. The record was not found.
    UPLUS_HIST_ERR_NO_PAGES,              //!< Error. No flash pages are available.
    UPLUS_HIST_ERR_USER_LIMIT_REACHED,  //!< Error. The maximum number of users has been reached.
    UPLUS_HIST_ERR_CRC_CHECK_FAILED,    //!< Error. The CRC check failed.
    UPLUS_HIST_ERR_BUSY,                   //!< Error. The underlying flash subsystem was busy.
    UPLUS_HIST_ERR_INTERNAL               //!< Error. An internal error occurred.
} uplus_hist_error_code_e;


typedef struct{
	uplus_hist_data_type_e    data_type;
	uint32_t                record_id;
	uint8_t*                data_buff;
	uint16_t                 data_len;
	uint32_t                 write_offset;
	uint32_t                 read_offset;
}uplus_hist_data_t;

typedef struct{
	uplus_hist_operation_type_e    type;
	uplus_hist_error_code_e       result;
}uplus_flash_operation_t;

typedef struct
{
	int8_t dev_id[16];
	uint8_t dev_id_len;
	int8_t product_type_id[64+1];
	uint8_t local_key[64+1];
	uint8_t local_key_len;
}uplus_ble_dev_cfg_t;

typedef struct
{
	uint16_t record_num;
	uint8_t is_sync;// status is synchronization
//	uint8_t sync_offset; //synchronization  record
	uint32_t write_offset;  //
	uint32_t read_offset;   //���ƫ��
	uint32_t final_write_offset;  //the final write_offset when write is full
	uint32_t start_read_offset;    //the start read_offset when get the hist record data
	uint8_t record_head;   //read index
	uint8_t record_tail;   //write index
	uint8_t once_full;
	uint8_t reserve;
}uplus_ble_history_record_t;

typedef struct
{
	uint8_t factory_flag;
	uint16_t record_num;
	uint32_t write_offset;
	uint32_t read_offset;
	uint32_t final_write_offset;
	uint8_t record_head;
	uint8_t record_tail;
	uint8_t once_full;
}uplus_ble_history_param_t;

#define MAX_EACH_HISTORY_RECORD_DATA_LEN 254

//#define UH_STROGE_DEBUG
#define    DENY_SET 						-1
#define    SAVE_ERR 						-2

extern uplus_ble_history_record_t *gv_record_parm;
extern uplus_ble_dev_cfg_t *gv_dev_cfg;

typedef void(* uplus_flash_handler_t)(uplus_hist_data_type_e data_type, uplus_flash_operation_t* p_flash_operation);
uplus_hist_error_code_e uplus_storage_record_read(uint32_t read_offset, uint8_t *out, uint16_t out_len);
uplus_hist_error_code_e uplus_storage_record_write(uint32_t write_offset, uint8_t *in, uint16_t in_len);
uplus_hist_error_code_e uplus_storage_record_delete(uplus_hist_data_type_e record_type);
int uplus_storage_reset(void);

//uint32_t uplus_svc_hist_get_info_cb(uint32_t *hist_tot, void * user_ctx);

void uh_update_fstorage_param(void);
int32_t uplus_ble_updata_dev_param(void);
int32_t	uplus_ble_update_hist_record_param(void);
int32_t uplus_hist_record_send(void);
void uplus_ble_cfg_update_period(void);
void uplus_ble_hist_sync_record(void);

uplus_ble_status_e uplus_ble_hist_timeout_timer_create(void);
uplus_hist_error_code_e uplus_ble_history_record_write(uplus_ble_sdk_cmd_type_e hist_data_type, uint32_t hist_time, uint8_t* p_data, uint16_t len);
uplus_hist_error_code_e uplus_init_hist_record_param(void);
uplus_hist_error_code_e actual_read_record_count(uint32_t base_read_offset, uint16_t actual_read_cnt, uint32_t* fin_read_offset);
void uplus_ble_arch_event_callback(uplus_ble_arch_event_t evt, uplus_ble_arch_evt_param_t* param);

void uplus_history_data_factory_reset(void);
void uplus_user_data_factory_reset(void);

int32_t uplus_user_config_read(uint8_t *conf, uint32_t len);
int32_t uplus_user_config_write(uint8_t *conf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* UH_RECORD_API_H_ */
