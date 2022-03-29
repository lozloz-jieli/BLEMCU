/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 14:14:12
 * @LastEditTime: 2021-08-31 09:35:22
 * @LastEditors: jianye
 */
#define LOG_TAG_CONST       HalFlash  //�������ļ���ͬ��
#define LOG_TAG             "[HalFlash]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"

#define LOG_v(t)  log_tag_const_v_ ## t
#define LOG_i(t)  log_tag_const_i_ ## t
#define LOG_d(t)  log_tag_const_d_ ## t
#define LOG_w(t)  log_tag_const_w_ ## t
#define LOG_e(t)  log_tag_const_e_ ## t


#define LOG_tag(tag, n) n(tag)

const char LOG_tag(LOG_TAG_CONST,LOG_v) AT(.LOG_TAG_CONST) = 0;
const char LOG_tag(LOG_TAG_CONST,LOG_i) AT(.LOG_TAG_CONST) = 1;
const char LOG_tag(LOG_TAG_CONST,LOG_d) AT(.LOG_TAG_CONST) = 1;
const char LOG_tag(LOG_TAG_CONST,LOG_w) AT(.LOG_TAG_CONST) = 1;
const char LOG_tag(LOG_TAG_CONST,LOG_e) AT(.LOG_TAG_CONST) = 1;

#include "app_config.h"
#include "include/includes.h"

#include "system/includes.h"
#include "asm/includes.h"
#include "generic/gpio.h"
#include "system/init.h"
#include "timer.h"
#include "app_config.h"
#include "system/includes.h"
#include "system/event.h"

typedef enum _FLASH_ERASER {
    CHIP_ERASER,
    BLOCK_ERASER, //64k
    SECTOR_ERASER,//4k
    PAGE_ERASER,  //256B
} FLASH_ERASER;

#define UPLUS_FLASH_PAGE_SIZE 			256
#define UPLUS_FLASH_ONE_PAGE_MASK 		0xFFFFFF00
#define UPLUS_FLASH_SECTOR_SIZE 		(4 * 1024)
#define UPLUS_FLASH_ONE_SECTOR_MASK 	0xFFFFF000
#define UPLUS_FLASH_DISH_INDEX_START       (0x1000)
#define UPLUS_FLASH_DISH_START          (0x1100)

//
extern bool sfc_erase(FLASH_ERASER cmd, u32 addr);
extern u32 sdfile_cpu_addr2flash_addr(u32 offset);
extern int flash_area_reset(FILE *fp);

#define USER_FILE_NAME       SDFILE_APP_ROOT_PATH"USERIF"
#define NV_MODE_FILE         0 //�̶�����һ��ָ��������һ������²��ᱻ���������flash�Ƚϴ�ķ������������
#define NV_MODE_VM           1 //��VM�棬����������ĸ��ʱȽϸߣ�����������ʱ��flash�ռ䲻����ʱ�������
#define TUYA_BLE_NV_MODE     NV_MODE_FILE
//ʹ���ļ��ĵķ�ʽ�������ݣ���Ҫ��ini�ļ��������������
/*
USERIF_ADR=AUTO;
USERIF_LEN=0x4000;
USERIF_OPT=1;
*/


struct vfs_attr code_attr = {0};
struct vfs_attr attr;

typedef struct __tuya_addr_to_vfs{
    u32 uplus_start_addr;//0
    u32 vfs_satrt_addr;//1
}uplus_addr_to_vfs;

static FILE *code_fp = NULL;
static uplus_addr_to_vfs addr_sw;

int32_t uplus_flash_init(void)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
#if (TUYA_BLE_NV_MODE == NV_MODE_FILE)
    if(code_fp){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "The file is already open");
        ret = UPLUS_BLE_ERR_BUSY;
        goto param_error;
    }

    code_fp = fopen(USER_FILE_NAME, "r+w");
    if (code_fp == NULL) {
        UPLUS_BLE_LOG_INTERNAL_ERROR("file open err!!!");
        ret = UPLUS_BLE_ERR_UNKNOWN;
        goto param_error;
    }

    fget_attrs(code_fp, &attr);
    if(attr.fsize < 2048){
        UPLUS_BLE_LOG_INTERNAL_ERROR("file size err!!!");
    }else{
        UPLUS_BLE_LOG_INTERNAL_DEBUG("flash size = 0x%08x.", attr.fsize);
    }
    addr_sw.uplus_start_addr = 0;
    addr_sw.vfs_satrt_addr = attr.sclust;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("addr %x,%x\n", attr.sclust, addr_sw.uplus_start_addr);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("uplus_ble_nv_initok\n");
#endif//(TUYA_BLE_NV_MODE == NV_MODE_FILE)

param_error:
    return ret;
}

int32_t uplus_flash_deinit(void)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    if(code_fp == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("The file is not open!!!");
        ret = UPLUS_BLE_ERR_UNKNOWN;
        goto param_error;
    }
    ret = flash_area_reset(code_fp); //������������
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[reset:%d]", __func__, ret);

param_error:
    return ret ? UPLUS_BLE_ERR_UNKNOWN : UPLUS_BLE_SUCCESS;
}


uint32_t uplus_flash_write(uint32_t addr, uint8_t *src, uint32_t num_bytes)
{


    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,addr);
#if 1
#if (TUYA_BLE_NV_MODE == NV_MODE_FILE)
    FILE *write_fp = code_fp;
    if (code_fp == NULL) {
        UPLUS_BLE_LOG_INTERNAL_ERROR("file ptr err!!!");
        ret = UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    ret = fseek(write_fp,addr , SEEK_SET);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("flash write addr=%x,ret=%d,size=%d\n",addr,ret,num_bytes);
    int r = fwrite(write_fp, src, num_bytes);//�������ݵ��Զ�������д����ǰ��Ҫ������ȷ������FF״̬���ܳɹ�д��
    if(r != num_bytes){
        UPLUS_BLE_LOG_INTERNAL_ERROR("write file error code!!! %d", r);
    }
#endif//(TUYA_BLE_NV_MODE == NV_MODE_FILE)
#endif
param_error:
    return ret;
}

static uint8_t g_flash_operate_data[UPLUS_FLASH_SECTOR_SIZE];
uint32_t uplus_flash_erase_sector(uint32_t addr)
{

    bool flag = 0;
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,addr);

    FILE *write_fp = code_fp;
    if (code_fp == NULL) {
        UPLUS_BLE_LOG_INTERNAL_ERROR("file ptr err!!!");
        ret = UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    u32 flash_addr = sdfile_cpu_addr2flash_addr(addr_sw.vfs_satrt_addr + addr);
    flag = sfc_erase(SECTOR_ERASER, flash_addr);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x], flag = %d.\n",__func__,flash_addr, flag);

param_error:
    return ret;
}

uint32_t uplus_flash_sector_write(uint32_t addr, uint8_t *src, uint32_t num_bytes)
{

#if 1

	//uint8_t tmp_buffer[UPLUS_FLASH_SECTOR_SIZE] = {0};
	uint32_t write_bytes = 0;
	uint32_t index = 0;
	uint32_t index_offset = 0;	
	uint32_t ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,addr);
#ifdef UPLUS_FLASH_DISH_INDEX_START
	if(UPLUS_FLASH_DISH_INDEX_START == addr){
		int w_r = syscfg_write(CFG_USER_DISH_INDEX,src,num_bytes);
		y_printf("HJY %s[line:%d,num_bytes:%d,ret:%d,w_r = %d]\n",__func__,__LINE__,num_bytes,ret,w_r);
		if(w_r != num_bytes){
			UPLUS_BLE_LOG_INTERNAL_ERROR("write vm error code!!! %d", ret);
			ret = UPLUS_BLE_ERR_RESOURCES;
			return ret;
		}
		return 0;
	}
#endif

	while(num_bytes) {
		if ((addr & UPLUS_FLASH_ONE_SECTOR_MASK) != ((addr + num_bytes - 1) & UPLUS_FLASH_ONE_SECTOR_MASK))
        {
#if 1
			memset(g_flash_operate_data, 0, sizeof(g_flash_operate_data));
			uplus_flash_read(addr & UPLUS_FLASH_ONE_SECTOR_MASK, g_flash_operate_data, UPLUS_FLASH_SECTOR_SIZE);
			//break;
			ret = uplus_flash_erase_sector(addr & UPLUS_FLASH_ONE_SECTOR_MASK);    //erase all   one sector
			if (ret != 0)
				return  ret;

			write_bytes = ((addr+UPLUS_FLASH_SECTOR_SIZE) & UPLUS_FLASH_ONE_SECTOR_MASK) - addr;  //rest of the one sector num
			index_offset = addr & 0x00000FFF;
			for (index = index_offset; index < UPLUS_FLASH_SECTOR_SIZE; index++) {
				g_flash_operate_data[index] = *src;
				src++;
			}

			uplus_flash_write(addr & UPLUS_FLASH_ONE_SECTOR_MASK, g_flash_operate_data, UPLUS_FLASH_SECTOR_SIZE);// write one sector
			log_info("sector_1 write_bytes %d addr %08x\n", write_bytes, addr);
			addr += write_bytes;
			num_bytes -= write_bytes;     //the next secotr data need to write and go to else 
#else
            break;
#endif
		}
		else
        {

#if 1
			write_bytes = num_bytes;
			memset(g_flash_operate_data, 0, sizeof(g_flash_operate_data));

			uplus_flash_read(addr & UPLUS_FLASH_ONE_SECTOR_MASK, g_flash_operate_data, UPLUS_FLASH_SECTOR_SIZE);
			ret = uplus_flash_erase_sector(addr & UPLUS_FLASH_ONE_SECTOR_MASK);
			if (ret != 0)
				return ret;

			index_offset = addr & 0x00000FFF;
			for (index = index_offset; index < (index_offset+write_bytes); index++) {
				g_flash_operate_data[index] = *src;
				src++;
			}
#endif
			uplus_flash_write(addr & UPLUS_FLASH_ONE_SECTOR_MASK, g_flash_operate_data, UPLUS_FLASH_SECTOR_SIZE);// write one sector
			num_bytes -= write_bytes;
			log_info("sector_2 write_bytes %d addr %08x\n", write_bytes, addr);
			break;
		}
	}
#endif
	return 0;
	param_error:
		return ret;

}

uint32_t uplus_flash_read(uint32_t offset, uint8_t *des, uint32_t num_bytes)
{
    int ret = 0;
	
#ifdef UPLUS_FLASH_DISH_INDEX_START
	if((UPLUS_FLASH_DISH_INDEX_START == offset) && (num_bytes != UPLUS_FLASH_SECTOR_SIZE)){
		int r_r = syscfg_read(CFG_USER_DISH_INDEX,des,num_bytes);
		y_printf("HJY %s[line:%d,num_bytes:%d,ret:%d]\n",__func__,__LINE__,num_bytes,ret);
		
		put_buf(des,ret);
		if(r_r != num_bytes){
			UPLUS_BLE_LOG_INTERNAL_ERROR("read vm error code!!! %d", ret);
			ret = UPLUS_BLE_ERR_RESOURCES;
			goto param_error;
		}
		goto param_error;
	}
#endif
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,offset);
#if (TUYA_BLE_NV_MODE == NV_MODE_FILE)
    FILE *read_fp = code_fp;
    if (code_fp == NULL) {
        UPLUS_BLE_LOG_INTERNAL_ERROR("file ptr err!!!");
        ret = UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    fseek(read_fp,offset, SEEK_SET);
    int r = fread(read_fp, des, num_bytes);//�ļ�ģʽ��ȡ�Զ���������
    if(r != num_bytes){
        UPLUS_BLE_LOG_INTERNAL_ERROR("read file error code!!! %d",r);
        ret = UPLUS_BLE_ERR_RESOURCES;
    }
    log_info("%s[fread:0x%x %d], ret = %d", __func__, addr_sw.uplus_start_addr+offset, r, ret);
    //UPLUS_BLE_HEXDUMP(des, num_bytes);
#endif//(TUYA_BLE_NV_MODE == NV_MODE_FILE)

param_error:
    return ret;
}

/**
 * @description:
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-16 14:01:55
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint32_t} offset
 * @param {uint8_t} *src
 * @param {uint32_t} num_bytes
 */
 #define OTA_FW_START_FADDR 0x44000

 int uplus_dual_bank_update_write_complete_cb(void *priv)
{

    //д��ɣ�TODO
    return 0;
}

uint32_t uplus_ota_fw_write(uint32_t offset, uint8_t *src, uint32_t num_bytes)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    //ret = uplus_flash_sector_write(OTA_FW_START_FADDR + offset, src, num_bytes);
    extern u32 dual_bank_update_write(void *data, u16 len, int (*write_complete_cb)(void *priv));
    dual_bank_update_write(src, num_bytes, uplus_dual_bank_update_write_complete_cb);

param_error:
    return ret;
}

uint32_t uplus_ota_fw_read(uint32_t offset, uint8_t *des, uint32_t num_bytes)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    //ret = uplus_flash_read(OTA_FW_START_FADDR + offset, des, num_bytes);

param_error:
    return ret;
}


uint32_t uplus_ota_fw_erase(uint32_t offset)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);

    //uplus_flash_erase_sector(OTA_FW_START_FADDR + offset);
param_error:
    return ret;
}

static u8 buf[4096] __attribute__((aligned(4)));
int uplus_flash_area_test(void)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);

    ret = uplus_flash_init();
    if(ret != UPLUS_BLE_SUCCESS){
        UPLUS_BLE_LOG_INTERNAL_ERROR("flash init err!!!");
        ret = UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    FILE *fp = code_fp;
    memset(buf, 0x00, sizeof(buf));

    uplus_flash_read(0,buf,64);

    for(u16 i = 0;i < sizeof(buf); i++){
        buf[i] = i;
    }

    uplus_flash_sector_write(0,buf,64);

    uplus_flash_read(0,buf,64);

    uplus_flash_deinit();

    uplus_flash_read(0,buf,64);



param_error:
    return ret;
}

static u8 buf2[4096] __attribute__((aligned(4)));
int uplus_flash_area_test2(void)
{
    int ret = 0;
    bool flag;
    uint32_t addr = 0x00;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);

    ret = uplus_flash_init();
    if(ret != UPLUS_BLE_SUCCESS){
        UPLUS_BLE_LOG_INTERNAL_ERROR("flash init err!!!");
        ret = UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    FILE *fp = code_fp;
    memset(buf2, 0x00, sizeof(buf2));


    for(u16 i = 0;i < sizeof(buf2); i++){
        buf2[i] = 0x5A;
    }

    flag = uplus_flash_erase_sector(addr);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s, flag = %d.\n",__func__, flag);

    uplus_flash_write(addr,buf2,UPLUS_FLASH_SECTOR_SIZE);
    uplus_flash_read(addr,buf2,UPLUS_FLASH_SECTOR_SIZE);
    //uplus_flash_sector_write(0,buf2,UPLUS_FLASH_SECTOR_SIZE);

    for(u16 i = 0;i < sizeof(buf2); i++){
        buf2[i] = 0x33;
    }
    //uplus_flash_sector_write(0,buf2,UPLUS_FLASH_SECTOR_SIZE);
    flag = uplus_flash_erase_sector(addr);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s, flag = %d.\n",__func__, flag);

    uplus_flash_write(addr,buf2,UPLUS_FLASH_SECTOR_SIZE);
    uplus_flash_read(addr,buf2,UPLUS_FLASH_SECTOR_SIZE);

param_error:
    return ret;
}




