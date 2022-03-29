#include "system/includes.h"
#include "asm/includes.h"
#include "generic/gpio.h"
#include "system/init.h"
#include "timer.h"
#include "app_config.h"
#include "system/includes.h"
#include "system/event.h"


const char log_tag_const_v_FLASH_IF AT(.LOG_TAG_CONST) = LIB_DEBUG & FALSE;
const char log_tag_const_i_FLASH_IF AT(.LOG_TAG_CONST) = LIB_DEBUG & TRUE;
const char log_tag_const_d_FLASH_IF AT(.LOG_TAG_CONST) = LIB_DEBUG & TRUE;
const char log_tag_const_w_FLASH_IF AT(.LOG_TAG_CONST) = LIB_DEBUG & TRUE;
const char log_tag_const_e_FLASH_IF AT(.LOG_TAG_CONST) = LIB_DEBUG & TRUE;

#define LOG_TAG_CONST       FLASH_IF
#define LOG_TAG             "[FLASH_IF]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"

///自定义flash区域空间
//  #按照芯片flash大小，最后4K保留，往上取地址
//  #USERIF_ADR=0x50000;
//  USERIF_ADR=0x5F000;
//  #USERIF_ADR=AUTO;
//  USERIF_LEN=128K;
//  USERIF_OPT=1;
///isd_config.ini 配置的地址要对齐
///4K对齐才能SECTOR_ERASER
///64K对齐才能BLOCK_ERASER

typedef enum _FLASH_ERASER {
    CHIP_ERASER,
    BLOCK_ERASER, //64k
    SECTOR_ERASER,//4k
    PAGE_ERASER,  //256B
} FLASH_ERASER;
extern bool sfc_erase(FLASH_ERASER cmd, u32 addr);
extern u32 sdfile_cpu_addr2flash_addr(u32 offset);
extern void clr_wdt(void);

FILE *flash_area_init(const char *path)
{
    log_info("%s", path);
    FILE *fp = NULL;
    struct vfs_attr attr = {0};
    fp = fopen(path, "r+w");
    if(fp == NULL){
        log_info("%s[%s]", __func__, "open fail!!!");
        return NULL;
    }
    fget_attrs(fp, &attr);
    log_info("%s[cpu_addr:0x%x,fsize:%dK]", __func__,attr.sclust,attr.fsize/1024);
    u32 flash_addr = sdfile_cpu_addr2flash_addr(attr.sclust);
    log_info("%s[flash_addr:0x%x,fsize:%dK]", __func__,flash_addr,attr.fsize/1024);
    return fp;
}

int flash_area_reset(FILE *fp)
{
    log_info("%s[0x%x]", __func__, fp);
    if(fp == NULL){
        return -1;
    }
    struct vfs_attr attr = {0};
    fget_attrs(fp, &attr);
    u32 erase_total_size = attr.fsize;
    u32 erase_addr = sdfile_cpu_addr2flash_addr(attr.sclust);
    u32 erase_size = 4096;
    u32 erase_cmd = SECTOR_ERASER;
    log_info("%s[0x%x %dK]", __func__, erase_addr, erase_total_size/1024);
    log_info("%s 64K align:%s", __func__, erase_addr%0x10000 ? "flase":"true");
    log_info("%s 4K align:%s",  __func__, erase_addr%0x1000 ? "flase":"true");
    while (erase_total_size) {
        clr_wdt();
        //擦除区域操作
        sfc_erase(erase_cmd, erase_addr);
        erase_addr += erase_size;
        erase_total_size -= erase_size;
    }
    /* sfc_erase(BLOCK_ERASER, erase_addr); */
    /* sfc_erase(SECTOR_ERASER, erase_addr); */
    //擦除完成后把文件指针定位到可写的位置
    fseek(fp, 0, SEEK_SET);
    return 0;
}

u32 flash_area_size(FILE *fp)
{
    log_info("%s[0x%x]", __func__, fp);
    if(fp == NULL){
        return -1;
    }
    struct vfs_attr attr = {0};
    fget_attrs(fp, &attr);
    return attr.fsize;
}

///使用范例
#define FLASH_AREA_NAME  SDFILE_APP_ROOT_PATH"USERIF" //名称要与ini文件对应
static u8 buf[4*1024] __attribute__((aligned(4)));
FILE *zone_fp = NULL;
void flash_area_test(void)
{
    log_info("%s", __func__);

    zone_fp = flash_area_init(FLASH_AREA_NAME);
    if(zone_fp == NULL){
        return;
    }
    int ret = 0;
    FILE *fp = zone_fp;
    int start_addr = 0;
    u32 size = flash_area_size(fp);
    memset(buf, 0x00, sizeof(buf));
    fseek(fp, 0, SEEK_SET);
    start_addr = fpos(fp);
    ret = fread(fp, buf, sizeof(buf));
    log_info("%s[fread:0x%x %d]", __func__, start_addr, ret);
    put_buf(buf, 16);

    for(u16 i = 0;i < sizeof(buf); i++){
        buf[i] = i;
    }
    fseek(fp, 0, SEEK_SET);
    start_addr = fpos(fp);
    ret = fwrite(fp, buf, sizeof(buf)); //写第1个扇区4K数据
    log_info("%s[fwrite:0x%x %d]", __func__, start_addr, ret);
    put_buf(buf+1024, 16); //打印1k后16B

    fseek(fp, 64*1024 - 4096, SEEK_SET);
    start_addr = fpos(fp);
    ret = fwrite(fp, buf, sizeof(buf)); //写最后的扇区4K数据
    log_info("%s[fwrite:0x%x %d]", __func__, start_addr, ret);
    put_buf(buf+1024, 16); //打印1k后16B

    memset(buf, 0x00, sizeof(buf));
    fseek(fp, 0, SEEK_SET);
    start_addr = fpos(fp);
    ret = fread(fp, buf, sizeof(buf)); //读第1个扇区4K数据
    log_info("%s[fread:0x%x %d]", __func__, start_addr, ret);
    put_buf(buf+1024, 16);
    fseek(fp, 64*1024 - 4096, SEEK_SET);
    start_addr = fpos(fp);
    ret = fread(fp, buf, sizeof(buf));//读最后的扇区4K数据
    log_info("%s[fread:0x%x %d]", __func__, start_addr, ret);
    put_buf(buf+1024, 16);

    ret = flash_area_reset(fp); //擦除整个区域
    log_info("%s[reset:%d]", __func__, ret);

    memset(buf, 0x00, sizeof(buf));
    fseek(fp, 0, SEEK_SET);
    start_addr = fpos(fp);
    ret = fread(fp, buf, sizeof(buf)); //读第1个扇区4K数据
    log_info("%s[fread:0x%x %d]", __func__, start_addr, ret);
    put_buf(buf, 16);
    put_buf(buf+256, 16);
    fseek(fp, 64*1024 - 4096, SEEK_SET);
    start_addr = fpos(fp);
    ret = fread(fp, buf, sizeof(buf));//读最后的扇区4K数据
    log_info("%s[fread:0x%x %d]", __func__, start_addr, ret);
    put_buf(buf, 16);
    put_buf(buf+256, 16);
}



