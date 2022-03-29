/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 14:10:49
 * @LastEditTime: 2021-08-31 14:10:16
 * @LastEditors: jianye
 */

#define LOG_TAG_CONST       PalBle  //保持与文件名同步
#define LOG_TAG             "[PalBle]"
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
#include "le/ble_api.h"
#include "btstack/le/att.h"
#include "le_common.h"
#include "bt_common.h"
#include "include/includes.h"
#include "app_main.h"

#ifndef UPULS_GAP_MAX_CALLBACKS
#define UPULS_GAP_MAX_CALLBACKS 1
#endif
static uint8_t gap_callback_numbers = 0;

uint32_t gap_cb_queue_table[UPULS_GAP_MAX_CALLBACKS];

#ifndef UPULS_GATTS_MAX_CALLBACKS
#define UPULS_GATTS_MAX_CALLBACKS 1
#endif
static uint8_t gatts_callback_numbers = 0;

uint32_t gatts_cb_queue_table[UPULS_GATTS_MAX_CALLBACKS];

static void printf_uplus_ble_adv_param_t(uplus_ble_adv_param_t *adv_param)
{
    UPLUS_BLE_PRINTF("%s[interval_min:%d]", __func__, adv_param->interval_min);
    UPLUS_BLE_PRINTF("%s[interval_max:%d]", __func__, adv_param->interval_max);
    UPLUS_BLE_PRINTF("%s[type:%d]", __func__, adv_param->type);
    UPLUS_BLE_PRINTF("%s[own_address_type:%d]", __func__, adv_param->own_address_type);
}

/* uplus_ble_status_e uplus_ble_set_adv_param(uplus_ble_adv_param_t *adv_param)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, adv_param);
    if(adv_param == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        return UPLUS_BLE_ERR_INVALID_PARAM;
    }
    printf_uplus_ble_adv_param_t(adv_param);
    return UPLUS_BLE_SUCCESS;
} */

/**
 * @description:
 * @param  haier ble 广播类型
 * @return JL ble 广播类型
 * ADV_NONCONN_IND+1：error
 * @Author: jianye
 * @Date: 2021-08-11 17:03:18
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 */
static u8 haier_adv_type_mapp(u8 type)
{
    u8 ret_type = 0;
    if( UPLUS_BLE_ADV_TYPE_CONNECTABLE_UNDIRECTED == type){
        ret_type = ADV_IND;
    }else if(UPLUS_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED == type){
        ret_type = ADV_SCAN_IND;
    }else if(UPLUS_BLE_ADV_TYPE_NON_CONNECTABLE_UNDIRECTED == type){
        ret_type = ADV_NONCONN_IND;
    }else{
        ret_type = ADV_NONCONN_IND+1;
    }
    return ret_type;
}

static u16 uplus_handle = 0x0000;
u8 *p_save_profile = NULL;
static u32 profile_len = 0;

u32 get_uplus_profile(void)
{
    return profile_len;
}

#define UPLUS_CHAR_VAL_MAX_LEN                      32

typedef struct _uplus_service_struct_t
{
    u8 *p_svr;
    u8 service_num;
    u16 service_len;
}uplus_service_struct_t;

typedef struct _uplus_char_struct_t
{
    u8 *p_char;
    u8 char_num;
    u16 char_len;
}uplus_char_struct_t;

typedef struct _uplus_char_line1_data{
    u16 char_data_len;
    u16 char_data_pro;
    u16 char_data_handle;
    u16 char_data_type;
    u8 next_char_data_pro;
    u16 char_data_handle_val;
    u8 char_data_uuid_buf[16];
    u8 char_data_uuid_len;
} _GNU_PACKED_ uplus_char_line1_data;

typedef struct _uplus_char_line2_data{
    u16 char_data_len;
    u16 char_data_pro;
    u16 char_data_handle;
    u8 char_data_uuid_buf[16];
    u8 char_data_uuid_len;
    u16 char_data_val_type;
    u8 char_data_value[UPLUS_CHAR_VAL_MAX_LEN];
    u16 char_data_value_len;
}uplus_char_line2_data;

typedef struct _uplus_char_line3_data{
    u16 char_data_len;
    u16 char_data_pro;
    u16 char_data_handle;
    u16 char_data_uuid;
    u16 char_deta_end;
}uplus_char_line3_data;

typedef struct _uplus_service_data{
    u16 srv_data_len;
    u16 srv_data_pro;
    u16 srv_data_handle;
    u16 srv_data_type;
    u8 srv_data_uuid_buf[16];
    u8 srv_data_uuid_len;
}uplus_service_data;


#define UPLUS_SERVICE_DATA_BASIC                      0X08

#define UPLUS_CHAR_LINE1_DATA_BASIC                   0X0B
#define UPLUS_CHAR_LINE2_DATA_BASIC                   0X06
#define UPLUS_CHAR_LINE3_DATA_BASIC                   0X0A

#define UPLUS_CHAR_DATA_BASIC                         0X11
#define UPLUS_CHAR_DATA_NOTIFY_OF_INDICAL             0x0a

#define UPLUS_16BIT_UUID                              0X02
#define UPLUS_128BIT_UUID                             16

#define UPLUS_BLE_PRIMARY_SERVICE_VALUE               0X2800
#define UPLUS_BLE_SECONDARY_SERVICE_VALUE             0X2801
#define UPLUS_BLE_CHAR_VALUE                          0X2803

#define UPLUS_BLE_SERVICE_PROPERTY_VALUE              0X0002

#define UPLUS_CHAR_PROPERTY_DEFINE_VAL                0X0002
#define UPLUS_CHAR_TYPE_DEFINE_VAL                    0X2803
//LINE2
#define UPLUS_CHAR_VALUE_TYPE_DEFINE_VAL              0X00AE

//LINE3
#define UPLUS_LINE3_PROPERTY_DEFINE_VAL               0X010A
#define UPLUS_LINE3_UUID_DEFINE_VAL                   0X2902
#define UPLUS_LINE3_END_DEFINE_VAL                    0X0000

//profile end
#define UPLUS_PRPFILE_END_LEN                         2
#define UPLUS_PRPFILE_END_VAL                         0


//MARK : Attribute Property Flags
#define EX_ATT_PROPERTY_BROADCAST                        0x01
#define EX_ATT_PROPERTY_READ                             0x02
#define EX_ATT_PROPERTY_WRITE_WITHOUT_RESPONSE           0x04
#define EX_ATT_PROPERTY_WRITE                            0x08
#define EX_ATT_PROPERTY_NOTIFY                           0x10
#define EX_ATT_PROPERTY_INDICATE                         0x20
#define EX_ATT_PROPERTY_AUTHENTICATED_SIGNED_WRITE       0x40
#define EX_ATT_PROPERTY_EXTENDED_PROPERTIES              0x80

//MARK: Attribute Property Flag,BTstack extension/ value is asked from client
#define EX_ATT_PROPERTY_DYNAMIC                          0x100
//128 bit UUID used
#define EX_ATT_PROPERTY_UUID128                          0x200
//Authentication required
#define EX_ATT_PROPERTY_AUTHENTICATION_REQUIRED          0x400
//Authorization from user required
#define EX_ATT_PROPERTY_AUTHORIZATION_REQUIRED           0x800



/**
 * @description: 大端序转小端序
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-23 11:23:18
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {u8} *buf
 * @param {u32} len
 */
static uplus_ble_status_e swap_u8buf(u8 *buf,u32 len)
{
    int ret = 0;
    u8 temp_buf[16];
    for(int i=0;i<len;i++){
        temp_buf[i] = buf[len-i-1];
    }
    memcpy(buf,temp_buf,len);
    return UPLUS_BLE_SUCCESS;
}

static u16 swap_u16(u16 data)
{
    u16 temp = 0;
    temp = ((data & 0x00FF) <<8) + ((data & 0xff00) >>8);
    return temp;
}


static u8 current_uuid_num = 0;
static u8 current_char_num = 0;

uplus_ble_gatts_db_t *p_uplus_ble_service_db = NULL;

u8 uuid_sive_buf[TEST_SERVICE_NUM_MAX][TEST_CHAR_NUM_MAX];

int get_char_uuid(u8 svr,u8 uchar,uplus_ble_uuid_t *uuid_data)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_uplus_ble_service_db);
    if(p_uplus_ble_service_db == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    uuid_data->type = p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_uuid.type;

    if(uuid_data->type == UPLUS_BLE_UUID_16){
        uuid_data->uuid16 = p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_uuid.uuid16;
        UPLUS_BLE_LOG_INTERNAL_DEBUG("uuid 16:0x%x\n",uuid_data->uuid16);
    }else if(uuid_data->type == UPLUS_BLE_UUID_128){
        memcpy(uuid_data->uuid128,p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_uuid.uuid128,UPLUS_128BIT_UUID);
        UPLUS_BLE_LOG_INTERNAL_DEBUG("uuid 128:\n");
        UPLUS_BLE_HEXDUMP(uuid_data->uuid128,UPLUS_128BIT_UUID);
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error uuid type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

param_error:
    return ret;

}

int get_notify_indicate_char_uuid(u8 svr,u8 uchar,uplus_ble_uuid_t *uuid_data)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_uplus_ble_service_db);
    if(p_uplus_ble_service_db == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if((p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_property & EX_ATT_PROPERTY_NOTIFY) \
        || (p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_property & EX_ATT_PROPERTY_INDICATE)){

    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "the handle isn't notify or indicate!!!");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    uuid_data->type = p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_uuid.type;

    if(uuid_data->type == UPLUS_BLE_UUID_16){
        uuid_data->uuid16 = p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_uuid.uuid16;
        UPLUS_BLE_LOG_INTERNAL_DEBUG("uuid 16:0x%x\n",uuid_data->uuid16);
    }else if(uuid_data->type == UPLUS_BLE_UUID_128){
        memcpy(uuid_data->uuid128,p_uplus_ble_service_db->p_srv_db[svr].p_char_db[uchar].char_uuid.uuid128,UPLUS_128BIT_UUID);
        UPLUS_BLE_LOG_INTERNAL_DEBUG("uuid 128:\n");
        UPLUS_BLE_HEXDUMP(uuid_data->uuid128,UPLUS_128BIT_UUID);
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error uuid type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

param_error:
    return ret;
}

static u16 uplus_ble_gatts_char_data_init(uplus_ble_gatts_char_db_t *p_char_db,u8 *p_save_profile,u32 offset)
{
    int ret = UPLUS_BLE_SUCCESS;
    uplus_char_line1_data char_line1_data;
    uplus_char_line2_data char_line2_data;
    uplus_char_line3_data char_line3_data;

    u32 temp_len = 0;

    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_char_db);
    if(p_char_db == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    memset(&char_line1_data,0x00,sizeof(uplus_char_line1_data));
    memset(&char_line2_data,0x00,sizeof(uplus_char_line2_data));
    memset(&char_line3_data,0x00,sizeof(uplus_char_line3_data));    //初始结构体

    //add by yuanlf . ios
    if (p_char_db->char_property & 0x04) {

        p_char_db->char_property = p_char_db->char_property | 0x08;
    }


    /* line1 赋值 */
    char_line1_data.char_data_pro = UPLUS_CHAR_PROPERTY_DEFINE_VAL;
    uplus_handle++;
    char_line1_data.char_data_handle = uplus_handle;    //handle 值依赖于service的handle递增
    char_line1_data.char_data_type = UPLUS_CHAR_TYPE_DEFINE_VAL;
    char_line1_data.next_char_data_pro = p_char_db->char_property;
    char_line1_data.char_data_handle_val = uplus_handle+1;



    if(UPLUS_BLE_UUID_16 == p_char_db->char_uuid.type){
        char_line1_data.char_data_uuid_len = UPLUS_16BIT_UUID;
        memcpy(char_line1_data.char_data_uuid_buf,&p_char_db->char_uuid.uuid16,char_line1_data.char_data_uuid_len);
    }else if(UPLUS_BLE_UUID_128 == p_char_db->char_uuid.type){
        char_line1_data.char_data_uuid_len = UPLUS_128BIT_UUID;
        memcpy(char_line1_data.char_data_uuid_buf,p_char_db->char_uuid.uuid128,char_line1_data.char_data_uuid_len);
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error uuid type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    char_line1_data.char_data_len =UPLUS_CHAR_LINE1_DATA_BASIC + char_line1_data.char_data_uuid_len;
    temp_len = char_line1_data.char_data_len;
    /* line1 字节序转换 */
    // char_line1_data.char_data_len = swap_u16(char_line1_data.char_data_len);
    // char_line1_data.char_data_handle = swap_u16(char_line1_data.char_data_handle);
    //char_line1_data.char_data_handle_val = swap_u16(char_line1_data.char_data_handle_val);
    // swap_u8buf(char_line1_data.char_data_uuid_buf,char_line1_data.char_data_uuid_len);
    /* line1 存入profile*/
    memcpy(&p_save_profile[offset],&char_line1_data,temp_len);
    offset += temp_len;
    UPLUS_BLE_HEXDUMP(&char_line1_data,temp_len);

    /* line2 赋值 */
    char_line2_data.char_data_pro |= p_char_db->char_property;
    if(TRUE == p_char_db->is_variable_len){
        char_line2_data.char_data_pro |= EX_ATT_PROPERTY_DYNAMIC;
    }
    if(TRUE == p_char_db->rd_author){
        UPLUS_BLE_LOG_INTERNAL_WARNING("%s[%s]",__func__, "Not Supported rd_author");
    }
    if(TRUE == p_char_db->wr_author){
        UPLUS_BLE_LOG_INTERNAL_WARNING("%s[%s]",__func__, "Not Supported wr_author");
    }
    uplus_handle++;
    p_char_db->char_handle.value_handle = uplus_handle;
    char_line2_data.char_data_handle = uplus_handle;    //handle 值依赖于service的handle递增
    uuid_sive_buf[current_uuid_num][current_char_num] = uplus_handle;
    g_printf("uuid save [%d,%d] val:0x%x\n",current_uuid_num,current_char_num,uuid_sive_buf[current_uuid_num][current_char_num]);

    if(UPLUS_BLE_UUID_16 == p_char_db->char_uuid.type){
        char_line2_data.char_data_uuid_len = UPLUS_16BIT_UUID;
        memcpy(char_line2_data.char_data_uuid_buf,&p_char_db->char_uuid.uuid16,char_line2_data.char_data_uuid_len);
    }else if(UPLUS_BLE_UUID_128 == p_char_db->char_uuid.type){
        char_line2_data.char_data_uuid_len = UPLUS_128BIT_UUID;
        memcpy(char_line2_data.char_data_uuid_buf,p_char_db->char_uuid.uuid128,char_line2_data.char_data_uuid_len);
        char_line2_data.char_data_pro |= EX_ATT_PROPERTY_UUID128;
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error uuid type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    r_printf("char uuid handle[0x%x]\n",uplus_handle);
    put_buf(char_line2_data.char_data_uuid_buf,char_line2_data.char_data_uuid_len);

    char_line2_data.char_data_len = UPLUS_CHAR_LINE2_DATA_BASIC + char_line2_data.char_data_uuid_len;
    if(0 != p_char_db->char_value_len){
        char_line2_data.char_data_val_type = UPLUS_CHAR_VALUE_TYPE_DEFINE_VAL;
        memcpy(char_line2_data.char_data_value,p_char_db->p_value,p_char_db->char_value_len);
        char_line2_data.char_data_value_len = p_char_db->char_value_len + 2;
        char_line2_data.char_data_len += char_line2_data.char_data_value_len;
    }
    /* line2 字节序转换 */
    // char_line2_data.char_data_len = swap_u16(char_line2_data.char_data_len);
    // char_line2_data.char_data_pro = swap_u16(char_line2_data.char_data_pro);
    // char_line2_data.char_data_handle = swap_u16(char_line2_data.char_data_handle);
    // swap_u8buf(char_line2_data.char_data_uuid_buf,char_line2_data.char_data_uuid_len);
    /* line2 存入profile*/
    memcpy(&p_save_profile[offset],&char_line2_data,UPLUS_CHAR_LINE2_DATA_BASIC);
    offset += UPLUS_CHAR_LINE2_DATA_BASIC;
    memcpy(&p_save_profile[offset],char_line2_data.char_data_uuid_buf,char_line2_data.char_data_uuid_len);
    offset += char_line2_data.char_data_uuid_len;
    if(0 != p_char_db->char_value_len){
        memcpy(&p_save_profile[offset],&(char_line2_data.char_data_val_type),char_line2_data.char_data_value_len);
        offset += char_line2_data.char_data_value_len;
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[handle:%d]",__func__,uplus_handle);
    }

    if((p_char_db->char_property & EX_ATT_PROPERTY_NOTIFY) || (p_char_db->char_property & EX_ATT_PROPERTY_INDICATE)){
        /* line3 赋值 */
        char_line3_data.char_data_pro = UPLUS_LINE3_PROPERTY_DEFINE_VAL;
        uplus_handle++;
        //add by yuanlf
        p_char_db->char_handle.cccd_handle = uplus_handle;
        char_line3_data.char_data_handle = uplus_handle;    //handle 值依赖于service的handle递增
        char_line3_data.char_data_uuid = UPLUS_LINE3_UUID_DEFINE_VAL;
        char_line3_data.char_deta_end = UPLUS_LINE3_END_DEFINE_VAL;
        char_line3_data.char_data_len = UPLUS_CHAR_LINE3_DATA_BASIC;

        temp_len = char_line3_data.char_data_len;
        /* line3 字节序转换 */
        // char_line3_data.char_data_len = swap_u16(char_line3_data.char_data_len);
        // char_line3_data.char_data_handle = swap_u16(char_line3_data.char_data_handle);
        /* line2 存入profile*/
        memcpy(&p_save_profile[offset],&char_line3_data,temp_len);
        offset += temp_len;
    }else{
        UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%s]",__func__, "don't have notify or indacate");
    }
    return offset;

param_error:
    if(UPLUS_BLE_SUCCESS == ret)
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[error:%d]",__func__,ret);
    return offset;
}



/**
 * @description: 为每个服务下面的特征生产特征
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-19 17:35:03
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_ble_gatts_srv_db_t} *p_srv_database
 */
static u32 uplus_ble_gatts_service_data_init(uplus_ble_gatts_srv_db_t *uplus_ble_service_database,u8 *p_save_profile,u32 offset)
{
    int ret = 0;

    uplus_service_data uplus_svr_dat;
    uplus_char_struct_t *p_save_char = NULL;

    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, uplus_ble_service_database);
    if(uplus_ble_service_database == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    memset(&uplus_svr_dat,0x00,sizeof(uplus_service_data));      //初始化结构体

    if(UPLUS_BLE_PRIMARY_SERVICE==uplus_ble_service_database->srv_type){  //区分主服务和次服务
        uplus_svr_dat.srv_data_type = UPLUS_BLE_PRIMARY_SERVICE_VALUE;
    }else if(UPLUS_BLE_SECONDARY_SERVICE==uplus_ble_service_database->srv_type){
        uplus_svr_dat.srv_data_type = UPLUS_BLE_SECONDARY_SERVICE_VALUE;
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error srv type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    uplus_handle ++;
    uplus_ble_service_database->srv_handle = uplus_handle;
    uplus_svr_dat.srv_data_handle = uplus_ble_service_database->srv_handle;   //赋值服务的handle



    if(UPLUS_BLE_UUID_16 == uplus_ble_service_database->srv_uuid.type){   //区分16位 128位UUID
        uplus_svr_dat.srv_data_uuid_len = UPLUS_16BIT_UUID;
        memcpy(uplus_svr_dat.srv_data_uuid_buf,&uplus_ble_service_database->srv_uuid.uuid16,uplus_svr_dat.srv_data_uuid_len);
        //swap_u8buf(uplus_svr_dat.srv_data_uuid_buf,UPLUS_16BIT_UUID);
    }else if(UPLUS_BLE_UUID_128 == uplus_ble_service_database->srv_uuid.type){
        uplus_svr_dat.srv_data_uuid_len = UPLUS_128BIT_UUID;
        memcpy(uplus_svr_dat.srv_data_uuid_buf,uplus_ble_service_database->srv_uuid.uuid128,uplus_svr_dat.srv_data_uuid_len);
        //swap_u8buf(uplus_svr_dat.srv_data_uuid_buf,UPLUS_128BIT_UUID);
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error uuid type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    y_printf("service uuid handle[0x%x]\n",uplus_handle);
    put_buf(uplus_svr_dat.srv_data_uuid_buf,uplus_svr_dat.srv_data_uuid_len);

    uplus_svr_dat.srv_data_pro = UPLUS_BLE_SERVICE_PROPERTY_VALUE;  //配置服务PROPERTY

    uplus_svr_dat.srv_data_len = UPLUS_SERVICE_DATA_BASIC + uplus_svr_dat.srv_data_uuid_len;

    memcpy(&p_save_profile[offset],&uplus_svr_dat,uplus_svr_dat.srv_data_len);
    offset += uplus_svr_dat.srv_data_len;

    for(int j=0;j<uplus_ble_service_database->char_num;j++){
        current_char_num = j;
        offset = uplus_ble_gatts_char_data_init(&(uplus_ble_service_database->p_char_db[j]),p_save_profile,offset);
    }

    return offset;

param_error:
    UPLUS_BLE_LOG_INTERNAL_ERROR("%s[error:%d]",__func__,ret);
    return 0;
}


/**
 * @description: 计算profile数组的长度
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-23 10:23:37
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_ble_gatts_char_db_t} *p_uplus_ble_char_db
 */
static u32 uplus_calculation_char_len(uplus_ble_gatts_char_db_t *p_uplus_ble_char_db)
{
    int ret = 0;
    u32 ret_char_len = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_uplus_ble_char_db);
    if(p_uplus_ble_char_db == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(UPLUS_BLE_UUID_16 == p_uplus_ble_char_db->char_uuid.type){
        ret_char_len += UPLUS_CHAR_DATA_BASIC + 2*UPLUS_16BIT_UUID;
    }else if(UPLUS_BLE_UUID_128 == p_uplus_ble_char_db->char_uuid.type){
        ret_char_len += UPLUS_CHAR_DATA_BASIC + 2*UPLUS_128BIT_UUID;
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error uuid type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    if(p_uplus_ble_char_db->char_value_len != 0){
        ret_char_len += p_uplus_ble_char_db->char_value_len + 2;    //add char value
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[handle:%d]",__func__,uplus_handle);
    }

    if((p_uplus_ble_char_db->char_property & EX_ATT_PROPERTY_NOTIFY)
    || (p_uplus_ble_char_db->char_property & EX_ATT_PROPERTY_INDICATE) ){
        ret_char_len += UPLUS_CHAR_DATA_NOTIFY_OF_INDICAL;
    }
    return ret_char_len;
param_error:
    UPLUS_BLE_LOG_INTERNAL_ERROR("%s[error:%d]",__func__,ret);
    return 0;

}


static u32 uplus_calculation_srv_len(uplus_ble_gatts_srv_db_t *p_uplus_ble_srv_db)
{
    int ret = 0;
    u32 ret_srv_len = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_uplus_ble_srv_db);
    if(p_uplus_ble_srv_db == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(UPLUS_BLE_UUID_16 == p_uplus_ble_srv_db->srv_uuid.type){
        ret_srv_len += UPLUS_SERVICE_DATA_BASIC + UPLUS_16BIT_UUID;
    }else if(UPLUS_BLE_UUID_128 == p_uplus_ble_srv_db->srv_uuid.type){
        ret_srv_len += UPLUS_SERVICE_DATA_BASIC + UPLUS_128BIT_UUID;
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "error uuid type");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    for(int i=0;i<p_uplus_ble_srv_db->char_num;i++){
        ret_srv_len += uplus_calculation_char_len(&(p_uplus_ble_srv_db->p_char_db[i]));
    }
    return ret_srv_len;

param_error:
    UPLUS_BLE_LOG_INTERNAL_ERROR("%s[error:%d]",__func__,ret);
    return 0;
}

static u32 uplus_calculation_proflie_len(uplus_ble_gatts_db_t *p_uplus_ble_gatts_db)
{
    int ret = 0;
    u32 ret_profile_len = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_uplus_ble_gatts_db);
    if(p_uplus_ble_gatts_db == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    for(int i=0;i<(p_uplus_ble_gatts_db->srv_num);i++){
        ret_profile_len += uplus_calculation_srv_len(&(p_uplus_ble_gatts_db->p_srv_db[i]));
    }
    ret_profile_len += UPLUS_PRPFILE_END_LEN;
    return ret_profile_len;

param_error:
    UPLUS_BLE_LOG_INTERNAL_ERROR("%s[error:%d]",__func__,ret);
    return 0;
}


/**
 * @description:
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 10:18:38
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_ble_gatts_db_t} *uplus_ble_service_database
 */
uplus_ble_status_e uplus_ble_gatts_service_init(uplus_ble_gatts_db_t *uplus_ble_service_database)
{
    int ret = 0;
    u32 offset = 0;
    current_uuid_num = 0;
    current_char_num = 0;
    memset(uuid_sive_buf,0x00,sizeof(uuid_sive_buf));

    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, uplus_ble_service_database);
    if(uplus_ble_service_database == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    p_uplus_ble_service_db = uplus_ble_service_database;

    profile_len = uplus_calculation_proflie_len(uplus_ble_service_database);

    p_save_profile = malloc(profile_len);
    if(p_save_profile == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_RESOURCES");
        ret =  UPLUS_BLE_ERR_RESOURCES;
        goto param_error;
    }
    memset(p_save_profile,0x00,profile_len);

    for(int i=0;i<uplus_ble_service_database->srv_num;i++){
        current_uuid_num = i;
        offset = uplus_ble_gatts_service_data_init(&(uplus_ble_service_database->p_srv_db[i]),p_save_profile,offset);

    }
    if((offset+UPLUS_PRPFILE_END_LEN) != profile_len){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "profile buf init error!!!");
        ret =  UPLUS_BLE_ERROR;
       // goto param_error;
    }
    UPLUS_BLE_LOG_INTERNAL_DEBUG("profile_len:%d,offset:%d\n",profile_len,offset+UPLUS_PRPFILE_END_LEN);
    att_server_change_profile(p_save_profile);

param_error:
    return ret;
}
/**
 * @description: GAP 初始化
 * @param  {NULL}
 * @return {NULL}
 * @Author: jianye
 * @Date: 2021-08-11 14:36:52
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 */
void uplus_ble_gap_init(void)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
#if 1
    for(uint8_t i= 0; i<UPULS_GAP_MAX_CALLBACKS; i++)
    {
        gap_cb_queue_table[i] = 0;
    }

    for(uint8_t i= 0; i<UPULS_GATTS_MAX_CALLBACKS; i++)
    {
        gatts_cb_queue_table[i] = 0;
    }
#endif

    att_server_change_profile(p_save_profile);
    bt_ble_init();
}



// 广播周期 (unit:0.625ms)
#define ADV_INTERVAL_MIN          (80)
#define MS_TO_UNITS(ms) (ms * 1000 / 625)
uplus_ble_status_e uplus_ble_set_adv_param(uplus_ble_adv_param_t *adv_param)
{
	if(app_var.adv_close == 1){
		log_info("app_var.adv_close");
		return 0xff;
	}
    int ret = 0;
    u8 adv_interval = 0;
    u8 adv_type;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, adv_param);
    if(adv_param == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;

    }
    if(MS_TO_UNITS(adv_param->interval_min) >= ADV_INTERVAL_MIN){
        adv_interval = MS_TO_UNITS(adv_param->interval_min);
    }else if(MS_TO_UNITS(adv_param->interval_max) >= ADV_INTERVAL_MIN){
        adv_interval = MS_TO_UNITS(adv_param->interval_max);
    }else{
        adv_interval = 0;
        ret  = UPLUS_BLE_ERR_INVALID_LENGTH;
        goto param_error;
    }

    adv_type = haier_adv_type_mapp(adv_param->type);
    if(adv_type > ADV_NONCONN_IND){
        ret  = UPLUS_BLE_ERR_GATT_INVALID_ATT_TYPE;
        goto param_error;
    }

//add by yuanlf
#if 1
    unsigned int cur_state = 0;
    unsigned int if_close_adv = 0;
    extern unsigned int uplus_get_ble_work_state(void);
    cur_state =  uplus_get_ble_work_state();
    if(cur_state == 0x0020)
    {
        bt_ble_adv_enable(0);
        //uplus_delay_ms(50);
        if_close_adv = 1;
    }

#endif // 1

    ret = ble_op_set_adv_param(adv_interval,adv_type, ADV_CHANNEL_ALL);
    if(!ret){
        ret = UPLUS_BLE_ERR_UNKNOWN;
    }

    if(if_close_adv)
        ble_module_enable(1);
param_error:
    return ret;

}

uplus_ble_status_e uplus_ble_gap_address_get(uplus_ble_addr_t mac)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, mac);
    if(mac == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    ret = le_controller_get_mac(mac);

param_error:
    return ret ? UPLUS_BLE_ERR_GAP_INVALID_BLE_ADDR : UPLUS_BLE_SUCCESS;
}

/**
 * @description: Function implementation
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-11 15:50:42
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_ble_gap_scan_type_t} scan_type
 * @param {uplus_ble_gap_scan_param_t} scan_param
 */
uplus_ble_status_e uplus_ble_gap_scan_start(uplus_ble_gap_scan_type_t scan_type,uplus_ble_gap_scan_param_t scan_param)
{
    int ret = 0;
    UPLUS_BLE_LOG_DEBUG("%s\n",__func__);
    ret = UPLUS_BLE_SUCCESS;
    return ret;
}

/**
 * @description: Function implementation
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-11 15:51:20
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 */
uplus_ble_status_e uplus_ble_gap_scan_stop(void)
{
    int ret = 0;
    UPLUS_BLE_LOG_DEBUG("%s\n",__func__);
    ble_module_enable(0);
    ret = UPLUS_BLE_SUCCESS;
    return ret;
}

uplus_ble_status_e uplus_ble_gap_adv_start(uplus_ble_gap_adv_param_t *p_adv_param)
{
	if(app_var.adv_close == 1){
		log_info("app_var.adv_close");
		return 0xff;
	}

    int ret = 0;
    u8 adv_interval = 0;
    u8 adv_type;
    u8 adv_channel = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_adv_param);
    if(p_adv_param == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    if(MS_TO_UNITS(p_adv_param->adv_interval_min) >= ADV_INTERVAL_MIN){
        adv_interval = MS_TO_UNITS(p_adv_param->adv_interval_min);
    }else if(MS_TO_UNITS(p_adv_param->adv_interval_max) >= ADV_INTERVAL_MIN){
        adv_interval = MS_TO_UNITS(p_adv_param->adv_interval_max);
    }else{
        adv_interval = 0;
        ret  = UPLUS_BLE_ERR_INVALID_LENGTH;
        goto param_error;
    }
    adv_type = haier_adv_type_mapp(p_adv_param->adv_type);
    if(adv_type > ADV_NONCONN_IND){
        ret  = UPLUS_BLE_ERR_GATT_INVALID_ATT_TYPE;
        goto param_error;
    }

    if(!(p_adv_param->ch_mask.ch_37_off)){
        adv_channel |= ~(p_adv_param->ch_mask.ch_37_off);
    }
    if(!(p_adv_param->ch_mask.ch_38_off)){
        adv_channel |= ~(p_adv_param->ch_mask.ch_38_off);
    }
    if(!(p_adv_param->ch_mask.ch_39_off)){
        adv_channel |= ~(p_adv_param->ch_mask.ch_39_off);
    }

    extern void external_set_adv_param(u32 interval,u8 type,u8 channel);
    external_set_adv_param(adv_interval,adv_type,adv_channel);

    ble_module_enable(1);


param_error:
    return ret;
}

bool external_set_adv_data(u8 *data, u8 len);
bool external_set_rsp_data(u8 *data, u8 len);

/**
 * @description: 扫描参数没有实现
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 10:39:47
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint8_t const *} p_data
 * @param {uint8_t} dlen
 * @param {uint8_t const} *p_sr_data
 * @param {uint8_t} srdlen
 */
uplus_ble_status_e uplus_ble_gap_adv_data_set(uint8_t const * p_data, uint8_t dlen,uint8_t const *p_sr_data,uint8_t srdlen)
{
	if(app_var.adv_close == 1){
		log_info("app_var.adv_close");
		return 0xff;
	}
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, p_data);
    if((p_data == NULL) || (p_sr_data == NULL)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    if((dlen > ADV_RSP_PACKET_MAX) || (srdlen > ADV_RSP_PACKET_MAX)){
        UPLUS_BLE_LOG_INTERNAL_ERROR("***adv_data overflow!!!!!!\n");
        ret = UPLUS_BLE_ERR_INVALID_LENGTH;
        goto param_error;
    }

//add by yuanlf
#if 1
    unsigned int cur_state = 0;
    unsigned int if_close_adv = 0;
    extern unsigned int uplus_get_ble_work_state(void);
    cur_state =  uplus_get_ble_work_state();
    if(cur_state == 0x0020)
    {
        bt_ble_adv_enable(0);
        //uplus_delay_ms(50);
        if_close_adv = 1;
    }

#endif // 1

    bool ret_adv = external_set_adv_data(p_data, dlen);//uplus更新广播数据，保存一下
    if(!ret_adv){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    bool ret_rsp = external_set_rsp_data(p_sr_data, srdlen);//uplus更新广播数据，保存一下
    if(!ret_rsp){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    UPLUS_BLE_LOG_INTERNAL_DEBUG("adv_data(%d):", dlen);
    UPLUS_BLE_HEXDUMP(p_data, dlen);

    UPLUS_BLE_LOG_INTERNAL_DEBUG("rsp_data(%d):", srdlen);
    UPLUS_BLE_HEXDUMP(p_sr_data, srdlen);

    int cmd_result = 0;
    cmd_result = ble_op_set_rsp_data(srdlen,p_sr_data);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s: cmd_result1 = %d", __func__, cmd_result);
    cmd_result = ble_op_set_adv_data(dlen,p_data);
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s: cmd_result2 = %d", __func__, cmd_result);
    //uplus_delay_ms(50);

    if(if_close_adv)
        ble_module_enable(1);
param_error:
    return ret;

}

uplus_ble_status_e uplus_ble_gap_adv_stop(void)
{
	if(app_var.adv_close == 1){
		log_info("app_var.adv_close");
		return 0xff;
	}

    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    extern void bt_ble_adv_enable(u8 enable);
    bt_ble_adv_enable(0);
    return UPLUS_BLE_SUCCESS;
}

/**
 * @description: Function implementation
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 10:48:46
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_ble_gap_scan_param_t} scan_param
 * @param {uplus_ble_gap_connect_t} conn_param
 */
uplus_ble_status_e uplus_ble_gap_connect(uplus_ble_gap_scan_param_t scan_param,uplus_ble_gap_connect_t conn_param)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, conn_param);
    return UPLUS_BLE_SUCCESS;
}

/**
 * @description:
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 11:19:02
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint16_t} conn_handle
 * @param {uplus_ble_gap_conn_param_t} conn_params
 */
uplus_ble_status_e uplus_ble_gap_update_conn_params(uint16_t conn_handle,uplus_ble_gap_conn_param_t conn_params)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, conn_params);
    if(&conn_params == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }


    UPLUS_BLE_LOG_INTERNAL_DEBUG("update_request:-%d-%d-%d-%d-\n", conn_params.min_conn_interval, conn_params.max_conn_interval, \
                                conn_params.slave_latency, conn_params.conn_sup_timeout);
    if (conn_handle) {
        ble_op_conn_param_request(conn_handle, &conn_params);
    }


param_error:
    return ret;

}

extern void ble_set_fix_pwr(u8 fix);;
uplus_ble_status_e uplus_ble_gap_tx_power_set(uint16_t conn_handle,int8_t radio_tx_pwr)
{
    int8_t fix = radio_tx_pwr;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    if(radio_tx_pwr > 10){
        UPLUS_BLE_LOG_INTERNAL_ERROR("The set power exceeds the Maximum\n");
        fix = 10;
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("The set power exceeds the Minimum\n");
        fix = 0;
    }
    ble_set_fix_pwr(fix);
    return UPLUS_BLE_SUCCESS;
}

/**
 * @description: Function implementation
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 14:22:32
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint16_t} conn_handle
 * @param {uint8_t} threshold_dbm
 * @param {uint8_t} skip_count
 */
uplus_ble_status_e uplus_ble_gap_rssi_start(uint16_t conn_handle,uint8_t threshold_dbm, uint8_t skip_count)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return UPLUS_BLE_SUCCESS;
}

/**
 * @description: Function implementation
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 14:24:18
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint16_t} conn_handle
 */
uplus_ble_status_e uplus_ble_gap_rssi_stop(uint16_t conn_handle)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return UPLUS_BLE_SUCCESS;
}

/**
 * @description: Function implementation
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 14:27:08
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint16_t} conn_handle
 * @param {int8_t } *p_rssi
 * @param {uint8_t} *p_ch_index
 */
uplus_ble_status_e uplus_ble_gap_rssi_get(uint16_t conn_handle, int8_t  *p_rssi, uint8_t *p_ch_index)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d]\n",__func__,conn_handle);
    return UPLUS_BLE_SUCCESS;
}

uplus_ble_status_e uplus_ble_gap_disconnect(uint16_t conn_handle)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d]\n",__func__,conn_handle);
    if(conn_handle){
        ble_op_disconnect(conn_handle);
    }else{
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
    }
    return ret;
}
/**
 * @description: gap 事件发送到uplus
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 16:34:47
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_ble_gap_evt_t} evt
 */
int uplus_ble_cb_gap_event_send(uplus_ble_gap_evt_t evt,uplus_ble_gap_evt_param_t*param)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, param);
     if(param == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    uplus_gap_evt_handler_t func;
    if(gap_cb_queue_table[0])
    {
        func = (uplus_gap_evt_handler_t) gap_cb_queue_table[0];
        func(evt,param);
    }else{
        UPLUS_BLE_LOG_INTERNAL_WARNING("%s[%s]",__func__, "No registered function");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
    }
param_error:
    return UPLUS_BLE_SUCCESS;

}


void uplus_ble_gap_register(uplus_gap_evt_handler_t gap_evt_handler,uplus_ble_role_type_e user_type)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x,%d]",__func__, gap_evt_handler,user_type);
    if(gap_evt_handler == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(UPLUS_BLE_SLAVE_ROLE == user_type){
        OS_ENTER_CRITICAL();
        if (gap_callback_numbers >= UPULS_GAP_MAX_CALLBACKS) {
            ret = UPLUS_BLE_ERR_RESOURCES;
        } else {
            gap_cb_queue_table[gap_callback_numbers] = (uint32_t)gap_evt_handler;
            gap_callback_numbers++;

            ret = UPLUS_BLE_SUCCESS;
        }
        OS_EXIT_CRITICAL();
    }else if(UPLUS_BLE_MASTER_ROLE == user_type){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "Host is not supported");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_USER_TYPE");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

param_error:
    if(ret)
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%d]",__func__,ret);
}

/**
 * @description: gatts 事件发送到uplus
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 16:55:18
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uplus_ble_gatts_evt_t} evt
 */
int uplus_ble_cb_gatts_event_send(uplus_ble_gatts_evt_t evt,uplus_ble_gatts_evt_param_t*param)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, param);
     if(param == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    uplus_gatts_evt_handler_t func;
    if(gap_cb_queue_table[0])
    {
        func = (uplus_gatts_evt_handler_t) gatts_cb_queue_table[0];
        ret = func(evt,param);

    }else{
        UPLUS_BLE_LOG_INTERNAL_WARNING("%s[%s]",__func__, "No registered function");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
    }
param_error:
    if(ret != 0)
        ret = 13;//invalid attribute val len
    return ret;

}

void uplus_ble_gatts_register(uplus_gatts_evt_handler_t gatts_evt_handler)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, gatts_evt_handler);
    if(gatts_evt_handler == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    OS_ENTER_CRITICAL();
    if (gatts_callback_numbers == UPULS_GATTS_MAX_CALLBACKS) {
        ret = UPLUS_BLE_ERR_RESOURCES;
    } else {
        gatts_cb_queue_table[gatts_callback_numbers] = (uint32_t)(gatts_evt_handler);
        gatts_callback_numbers++;
        ret = UPLUS_BLE_SUCCESS;
    }
    OS_EXIT_CRITICAL();


param_error:
    if(ret)
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%d]",__func__,ret);
}

#define UPLUS_ATT_OP_NOTIFY     1
#define UPLUS_ATT_OP_INDICATE   2

extern int app_send_user_data(u16 handle, u8 *data, u16 len, u8 handle_type);

/**
 * @description: Function implementation
 * @param  {*}
 * @return {*}
 * @Author: jianye
 * @Date: 2021-08-13 17:39:11
 * @LastEditTime: Do not edit
 * @LastEditors: jianye
 * @param {uint16_t} conn_handle
 * @param {uint16_t} srv_handle
 * @param {uint16_t} char_value_handle
 * @param {uint8_t} offset
 * @param {uint8_t*} p_value
 * @param {int} len
 * @param {uint8_t} type
 */

uplus_ble_status_e uplus_ble_gatts_notify_or_indicate(uint16_t conn_handle,uint16_t srv_handle,uint16_t char_value_handle,uint8_t offset,uint8_t* p_value,int len,uint8_t type)
{
    int ret = 0;
    unsigned int time_ms = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[handle:%d]\n",__func__,char_value_handle);
    if(p_value == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

    if(UPLUS_ATT_OP_NOTIFY == type){
        ret = app_send_user_data(char_value_handle, &p_value[offset], len, ATT_OP_NOTIFY);
        UPLUS_BLE_LOG_INTERNAL_DEBUG("%s, ret = %d", __func__, ret);
    }else if(UPLUS_ATT_OP_INDICATE == type){

        time_ms = uplus_systick_uptime_get();
        UPLUS_BLE_LOG_INTERNAL_DEBUG("%s, time_ms1 = %d", __func__, time_ms);
        ret = app_send_user_data(char_value_handle, &p_value[offset], len, ATT_OP_INDICATE);//5ms
        time_ms = uplus_systick_uptime_get();
        UPLUS_BLE_LOG_INTERNAL_DEBUG("%s, time_ms2 = %d", __func__, time_ms);

        extern void uplus_indicate_ack_handle_timer_start(uint8_t flag);
        if(!ret)//success
        {
           uplus_indicate_ack_handle_timer_start(ERR_INDICATE_ACK);
        }
        else
        {
            //do nothing
        }

    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "the error type!!!");
        ret = UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }

param_error:
    return ret;
}

uplus_ble_status_e uplus_ble_gatts_value_set(uint16_t srv_handle,uint16_t value_handle,uint8_t offset,uint8_t* p_value,uint16_t len)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return UPLUS_BLE_SUCCESS;
}

uplus_ble_status_e uplus_ble_gatts_value_get(uint16_t srv_handle,uint16_t char_handle,uint8_t* pdata,uint16_t *p_len)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return UPLUS_BLE_SUCCESS;
}


uplus_ble_status_e uplus_ble_gatts_mtu_default_set(uint16_t att_mtu)
{
    int ret = 0;
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[%d]",__func__, att_mtu);
    if(att_mtu > 517){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_LENGTH");
        ret = UPLUS_BLE_ERR_INVALID_LENGTH;
        goto param_error;
    }
    extern void update_mtu_default(u16 mtu);
    update_mtu_default(att_mtu);
param_error:
    return ret;

}

extern uint16_t gatts_get_mtu(void);
uint16_t uplus_ble_gatts_mtu_get(uint16_t conn_handle)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]",__func__, conn_handle);
    if(conn_handle){
        return gatts_get_mtu();
    }else{
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_CONN_HANDLE");
        return UPLUS_BLE_ERR_INVALID_CONN_HANDLE;
    }
}

uplus_ble_status_e uplus_ble_gatts_rw_auth_reply(uint16_t conn_handle,uint8_t status,uint16_t char_value_handle,uint16_t offset,uint8_t* p_value,int len,uint8_t type)
{
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return UPLUS_BLE_SUCCESS;
}

uplus_ble_status_e uplus_ble_pairing_feature_set(
    uint8_t io_caps ,
    uint8_t auth_oob ,
    uint8_t auth_req ,
    uint8_t auth_pair_mode)
{

    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s\n",__func__);
    return UPLUS_BLE_SUCCESS;
}

unsigned short uplus_ble_is_connected()
{
    return bt_ble_is_connected();
}

//add by yuanlf
/////////////////////////////lock//////////////////////////////////////
/**
 * @name:
 * @test: test font
 * @msg:
 * @param {uplus_pthread_mutex_t *} p_mutex
 * @param {void *} not_used
 * @return {*}
 */
typedef void * uplus_pthread_mutex_t;
int uplus_pthread_mutex_init(uplus_pthread_mutex_t * p_mutex,void * not_used)
{
    int ret = 0;
    if(p_mutex == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,p_mutex);

    OS_SEM * p_mutex_tmp  = malloc(sizeof(OS_SEM));

    g_printf("%s[line:%d,p_mutex:0x%x]\n",__func__,__LINE__,p_mutex_tmp);


    if(p_mutex_tmp == NULL){
        UPLUS_BLE_PRINTF("%s[%s]",__func__,"malloc error!!!");
        ret =  UPLUS_BLE_ERR_NOT_FOUND;
        goto param_error;
    }
    ret = os_sem_create(p_mutex_tmp,1);
    *p_mutex = p_mutex_tmp;

param_error:
    return ret;
}

int uplus_pthread_mutex_destroy(uplus_pthread_mutex_t * p_mutex)
{
    int ret = 0;
    if(*p_mutex == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,*p_mutex);

    ret = os_sem_del((OS_SEM *)*p_mutex,0);
    free(*p_mutex);

param_error:
    return ret;
}

int uplus_pthread_mutex_trylock(uplus_pthread_mutex_t * p_mutex)
{
    int ret = 0;
    if(*p_mutex == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,*p_mutex);
    ret = os_sem_accept((OS_SEM *)*p_mutex);

param_error:
    return ret;
}
int uplus_pthread_mutex_lock(uplus_pthread_mutex_t * p_mutex)
{
    int ret = 0;
    if(*p_mutex == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
    //UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,*p_mutex);
    ret = os_sem_pend((OS_SEM *)*p_mutex,0);

param_error:
    return ret;
}
int uplus_pthread_mutex_unlock(uplus_pthread_mutex_t * p_mutex)
{
    int ret = 0;
    if(*p_mutex == NULL){
        UPLUS_BLE_LOG_INTERNAL_ERROR("%s[%s]",__func__, "UPLUS_BLE_ERR_INVALID_PARAM");
        ret =  UPLUS_BLE_ERR_INVALID_PARAM;
        goto param_error;
    }
   // UPLUS_BLE_LOG_INTERNAL_DEBUG("%s[0x%x]\n",__func__,*p_mutex);
    ret = os_sem_post((OS_SEM *)*p_mutex);

param_error:
    return ret;
}
