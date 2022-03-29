/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 16:16:35
 * @LastEditTime: 2021-08-31 11:30:57
 * @LastEditors: jianye
 */
#define LOG_TAG_CONST       UPlus  //保持与文件名同步
#define LOG_TAG             "[UPlus]"
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
#include "le_common.h"
#include "string.h"
#include "uplus_trans.h"

uplus_user_ctrl_t g_user_ctrl_handle;
void uplus_ble_indicate_ack_register(uplus_indicate_ack_handler cb)
{
    g_user_ctrl_handle.indicate_ack_cb = cb;
}

void uplus_ble_indicate_ack_handle_cb()
{

    UPLUS_BLE_LOG_INFO("[%s:%d], indicate_ack_state = %d.", __func__, __LINE__, g_user_ctrl_handle.indicate_ack_state);
    uplus_clear_timer_id(UPLUS_TIMER_USER2);
    if(g_user_ctrl_handle.indicate_ack_cb)
    {
        if(g_user_ctrl_handle.indicate_ack_state == OK_INDICATE_ACK)
            g_user_ctrl_handle.indicate_ack_cb(OK_INDICATE_ACK);//success
        else
            g_user_ctrl_handle.indicate_ack_cb(ERR_INDICATE_ACK);
    }
}

void uplus_indicate_ack_handle_timer_start(uplus_u8 flag)
{

    if(flag == OK_INDICATE_ACK)
    {
        g_user_ctrl_handle.indicate_ack_state = OK_INDICATE_ACK;
        uplus_timer_restart(UPLUS_TIMER_USER3, 10);// one step = 10ms
    }
    else if(flag == ERR_INDICATE_ACK)
    {
        g_user_ctrl_handle.indicate_ack_state = ERR_INDICATE_ACK;
        uplus_timer_restart(UPLUS_TIMER_USER3, 1000);
    }
}

static void uplus_ble_cb_config_notify_callback(int state, int err, void *cb_param)
{
	UPLUS_BLE_LOG_INFO("[%s][%d]cfg_state cb\n", __func__, __LINE__);
	g_user_ctrl_handle.cfg_state = state;
}
static void uplus_ble_cb_time_ts_notify_callback(int err_num, unsigned int ts)
{
	UPLUS_BLE_LOG_INFO("[%s][%d]time_ts cb\n", __func__, __LINE__);
	g_user_ctrl_handle.ts = ts;
}

u8 data_buffer[] = {0x1,0x2,0x3,0x4};
extern int at_uart_send_packet(const u8 *packet, int size);
uplus_u8 param_sn;

static void uplus_ble_epp_data_from_external_to_tx(uplus_ble_sdk_epp_rx_event_t evt, uplus_ble_sdk_epp_rx_evt_param_t* param)
{
	uplus_send_storage_flg_e enumStorageFlg;
	uplus_ble_sdk_cmd_type_e enumTypeCmd;
	uplus_ble_sdk_cmd_type_e u_cmd_type;
	uplus_ble_sdk_ctrl_param_t *pstParam = NULL;
	unsigned int  time_stamp = 0;
	static uplus_u32 ucTimes = 0;
	//signed char ret = 0;
	uplus_u8 data_invalid[2] = {0x00, 0x00};
	uplus_u8 data_alarm_ack[8] = {0x0F,0x5A,0x00,0x00,0x00,0x00,0x00,0x03};
//	uplus_u8 data_status_2[25] = {0x01, 0x1a, 0x14, 0x00, 0x64, 0x00, 0x00, 0x14, 0x00, 0x03, 0x01, 0x1e, 0x00};
//	uplus_u8 data_status_1[25] = {0x01, 0x1a, 0x14, 0x00, 0x64, 0x00, 0x00, 0x14, 0x00, 0x03, 0x01, 0x1e, 0x00};

	y_f_printf("uplus_ble_epp_data_from_external_to_tx");     //鎵撳嵃寮�濮?
	printf("0x%x",param->ctrl.cmd_type);
	printf_buf(data_buffer, sizeof(data_buffer));            //娴嬭瘯鎵撳嵃鎺ュ彛   0x1,0x2,0x3,0x4
//	printf_buf((u8 *)param->ctrl.ctrl_param.sn,4);
	printf("0x%x",param->ctrl.ctrl_param.sn);
	printf_buf(param->ctrl.ctrl_param.data,param->ctrl.ctrl_param.data_len);

	printf("0x%x",param->ctrl.ctrl_param.data_len);
#if 0
	at_uart_send_packet(&param->ctrl.cmd_type,1);                                      //鍙戦�佸埌涓插彛
	at_uart_send_packet(&param->ctrl.ctrl_param.sn,1);                                      //鍙戦�佸埌涓插彛
	at_uart_send_packet(param->ctrl.ctrl_param.data,param->ctrl.ctrl_param.data_len);           //鍙戦�佸埌涓插彛
	at_uart_send_packet(&param->ctrl.ctrl_param.data_len,1);                                      //鍙戦�佸埌涓插彛
#endif

	r_f_printf("uplus_ble_epp_data_from_external_to_tx");		//鎵撳嵃缁撴潫

	enumTypeCmd = param->ctrl.cmd_type;
	enumStorageFlg = UPLUS_SEND_ONLY;
	pstParam = uplus_tool_malloc(sizeof(uplus_ble_sdk_ctrl_param_t));
	if (NULL == pstParam) {
		goto free_res;
	}
	memset(pstParam,0,sizeof(uplus_ble_sdk_ctrl_param_t));
	pstParam->sn = param->ctrl.ctrl_param.sn;
	param_sn = param->ctrl.ctrl_param.sn;
	uplus_ble_sdk_time_ts_get(&time_stamp);

	UPLUS_BLE_LOG_INFO("=rx cmd %u=\n", param->ctrl.cmd_type);
	if (param->ctrl.ctrl_param.data) {
		UPLUS_BLE_LOG_HEXDUMP(param->ctrl.ctrl_param.data, param->ctrl.ctrl_param.data_len);
	}

	switch(enumTypeCmd) {
	case UPLUS_BLE_SDK_CMD_TYPE_CTRL:
		UPLUS_Parse(param->ctrl.ctrl_param.data,param->ctrl.ctrl_param.data_len);
		break;

	case UPLUS_BLE_SDK_CMD_TYPE_GET_ALARM:
		g_f_printf("UPLUS_BLE_SDK_CMD_TYPE_GET_ALARM");
		u_cmd_type = UPLUS_BLE_SDK_CMD_TYPE_GET_ALARM_ACK;
		pstParam->data_len = 8;
		pstParam->data= uplus_tool_malloc(pstParam->data_len);
		if (pstParam->data == NULL) {
			goto free_res;
		}
		memset(pstParam->data,0,pstParam->data_len);
		memcpy(pstParam->data,data_alarm_ack,pstParam->data_len);
		break;

 	case UPLUS_BLE_SDK_CMD_TYPE_STOP_ALARM:
		g_f_printf("UPLUS_BLE_SDK_CMD_TYPE_STOP_ALARM");
		u_cmd_type = UPLUS_BLE_SDK_CMD_TYPE_STOP_ALARM_ACK;
		pstParam->data_len = 0;
		pstParam->data= NULL;
		break;

	case UPLUS_BLE_SDK_CMD_TYPE_TRANSMIT_TRANSPARENTLY:
		g_f_printf("UPLUS_BLE_SDK_CMD_TYPE_TRANSMIT_TRANSPARENTLY");
		u_cmd_type = UPLUS_BLE_SDK_CMD_TYPE_TRANSMIT_TRANSPARENTLY_ACK;
		r_printf("the daat is %d",pstParam->data);

        printf_buf(pstParam->data, pstParam->data_len);
		pstParam->data_len = 0;
		pstParam->data= NULL;
		break;

	default:
		g_f_printf("UPLUS_BLE_SDK_CMD_TYPE_ACK");
		u_cmd_type =  UPLUS_BLE_SDK_CMD_TYPE_ACK;
		pstParam->data_len = 0;
		pstParam->data= NULL;
		break;
	}

	y_f_printf("-------------------------u_cmd_type:%x--------------------------------",u_cmd_type);
	printf("0x%x",pstParam->sn);
	printf_buf(pstParam->data,pstParam->data_len);
	printf("0x%x",pstParam->data_len);
	printf("%d",time_stamp);
	uplus_ble_sdk_epp_tx_func(enumStorageFlg, u_cmd_type,pstParam, time_stamp);					//鍥炶皟鍏虫帀
#if 0
	at_uart_send_packet(&pstParam->sn,1);                                      //鍙戦�佸埌涓插彛
	at_uart_send_packet(pstParam->data,pstParam->data_len);            //鍙戦�佸埌涓插彛
	at_uart_send_packet(&pstParam->data_len,1);                                      //鍙戦�佸埌涓插彛
	at_uart_send_packet(&time_stamp,4);                                      //鍙戦�佸埌涓插彛
#endif
	r_f_printf("---------------------------------------------------------");

free_res:
	if (pstParam->data) {
		uplus_tool_free(pstParam->data);
		pstParam->data = NULL;
	}

	if(pstParam) {
		uplus_tool_free(pstParam);
		pstParam = NULL;
	}

	g_f_printf("uplus_ble_epp_data_from_external_to_tx");
	return ;
}

void uplus_report(uplus_u8 *buff, uplus_u8 len, uplus_u8 isHist)
{
//	return;
    log_info_hexdump(buff, len);

	uplus_send_storage_flg_e enumStorageFlg;
	uplus_ble_sdk_cmd_type_e enumTypeCmd;
	uplus_ble_sdk_cmd_type_e u_cmd_type;
	uplus_ble_sdk_ctrl_param_t *pstParam = NULL;
	unsigned int  time_stamp = 0;
	static uplus_u32 ucTimes = 0;

	enumStorageFlg = UPLUS_SEND_ONLY;

	u_cmd_type = UPLUS_BLE_SDK_CMD_TYPE_STATUS;
	if(isHist){
		enumStorageFlg = UPLUS_SEND_AND_STORAGE;
//		u_cmd_type = UPLUS_BLE_SDK_CMD_TYPE_EVENT_REPORT;
////	u_cmd_type = UPLUS_BLE_SDK_CMD_TYPE_TRANSMIT_TRANSPARENTLY;
//
	}


	pstParam = uplus_tool_malloc(sizeof(uplus_ble_sdk_ctrl_param_t));
	if (NULL == pstParam) {
		goto free_res2;
	}
	memset(pstParam,0,sizeof(uplus_ble_sdk_ctrl_param_t));
	pstParam->sn = param_sn;

	pstParam->data_len = len;
	pstParam->data= uplus_tool_malloc(pstParam->data_len);
	if (pstParam->data == NULL) {
		goto free_res2;
	}
	memset(pstParam->data,0,pstParam->data_len);
	memcpy(pstParam->data, buff, pstParam->data_len);

	uplus_ble_sdk_time_ts_get(&time_stamp);

	uplus_ble_sdk_epp_tx_func(enumStorageFlg, u_cmd_type,pstParam, time_stamp);

free_res2:
	if (pstParam->data) {
		uplus_tool_free(pstParam->data);
		pstParam->data = NULL;
	}

	if(pstParam) {
		uplus_tool_free(pstParam);
		pstParam = NULL;
	}
}

//uplus_u8 rpt_data[] = {0x01, 0x1a, 0x14, 0x00, 0x64, 0x00, 0x00, 0x14, 0x00, 0x03, 0x01, 0x1e, 0x00};
static void uplus_user_rpt_status(uplus_send_storage_flg_e flg)
{
	T_AppFrame rpt_data;
	uplus_ble_sdk_ctrl_param_t param;
    unsigned int  time_stamp = 0;

	uplus_ble_sdk_time_ts_get(&time_stamp);
	memset(&param, 0, sizeof(uplus_ble_sdk_ctrl_param_t));
//	param.data_len = sizeof(rpt_data);
	param.data= (uint8_t *)&rpt_data;
	param.data_len = UPLUS_GetStateData((uint8_t *)&rpt_data);

	uplus_ble_sdk_epp_tx_func(flg, UPLUS_BLE_SDK_CMD_TYPE_REPORT, &param, time_stamp);
}



// void uplus_send_data(u8 *data)
// {
	// uplus_u8 rpt_data[] = {0};
	// memcpy(rpt_data,data,sizeof(data));
	// y_f_printf("uplus_send_data");
	// printf_buf(rpt_data, sizeof(rpt_data));
	// uplus_user_rpt_status(UPLUS_SEND_ONLY);
// }

void uplus_user_rpt_status_extern()
{
    uplus_user_rpt_status(UPLUS_SEND_AND_STORAGE);
}
void uplus_user_rpt_event(void)
{
	uplus_ble_sdk_ctrl_param_t param;
	uplus_u8 event_id[2] = {0x18, 0x1F};
	unsigned int time_stamp = 0;
	static uplus_u8 event_sn =0;

	param.data = event_id;
    param.data_len = sizeof(event_id);

	uplus_ble_sdk_time_ts_get(&time_stamp);
	memset(&param, 0, sizeof(uplus_ble_sdk_ctrl_param_t));
	param.sn = event_sn++;
	param.data_len = sizeof(event_id);
	param.data= event_id;

	uplus_ble_sdk_epp_tx_func(UPLUS_SEND_AND_STORAGE, UPLUS_BLE_SDK_CMD_TYPE_EVENT_REPORT, &param, time_stamp);
    UPLUS_BLE_LOG_INFO("user rpt evt sn %u", param.sn);
}

static void uplus_user_rpt_status_timer_func(void *ctx)
{
	if (2 == g_user_ctrl_handle.connect_state) {
		//uplus_user_rpt_status(UPLUS_SEND_AND_STORAGE);
		uplus_user_rpt_status(UPLUS_SEND_ONLY);
	}
	uplus_timer_stop(g_user_ctrl_handle.rpt_timer_id);

}
static void uplus_ble_cb_conn_state_notify_callback(unsigned char conn_state, void* cb_param)
{
	UPLUS_BLE_LOG_INFO("conn_state cb.\n");
	g_user_ctrl_handle.connect_state = conn_state;
	if (2 == g_user_ctrl_handle.connect_state) {
		//uplus_user_rpt_status(UPLUS_SEND_ONLY);
		//uplus_user_rpt_event();
		uplus_timer_restart(g_user_ctrl_handle.rpt_timer_id, 5*1000);
		//extern void ble_send_data_to_app_indicate(void);
		//sys_timeout_add(NULL, ble_send_data_to_app_indicate, 5000);
	}
}

void uplus_user_ble_init()
{
  	init_cfg_para_t *init_cfg = NULL;
    uint8_t mac[6];

    uh_bt_get_public_addr(mac);
	UPLUS_BLE_LOG_INFO("mac addr = %02x %02x %02x %02x %02x %02x \n" , mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

	init_cfg = (init_cfg_para_t *)uplus_tool_malloc(sizeof(init_cfg_para_t));
	if (NULL == init_cfg) {
		return;
	}
    memset(init_cfg, 0, sizeof(init_cfg_para_t));
	memcpy(init_cfg->product_type_id, USER_TYPE_ID , sizeof(USER_TYPE_ID));
	sprintf((char *)init_cfg->ble_dev_name, "%s%02x%02x", USER_DEV_NAME, mac[1], mac[0]);
	memcpy(init_cfg->overall_model_number, USER_OVERALL_MODE_NUMBER, strlen(USER_OVERALL_MODE_NUMBER));
	memcpy(init_cfg->overall_version, USER_OVERALL_VERSION, strlen(USER_OVERALL_VERSION));
	memcpy(init_cfg->app_version, USER_SOFTWARE_VER, strlen(USER_SOFTWARE_VER));
	memcpy(init_cfg->sw_type, USER_SOFTWARE_TYPE, strlen(USER_SOFTWARE_TYPE));
	memcpy(init_cfg->sw_ver, USER_SOFTWARE_VER, strlen(USER_SOFTWARE_VER));
	memcpy(init_cfg->hw_ver, USER_HARDWARE_VER, strlen(USER_HARDWARE_VER));
	memcpy(init_cfg->product_code, USER_PRODUCT_CODE, strlen(USER_PRODUCT_CODE));
	memcpy(init_cfg->hw_type, USER_HARD_TYPE, sizeof(USER_HARD_TYPE));

    uplus_flash_init();
    extern int uplus_ble_sdk_user_config_init(void);
    uplus_ble_sdk_user_config_init();

	uplus_ble_sdk_start(init_cfg);

	uplus_ble_sdk_dlps_reg_config_cb(NULL);

	uplus_ble_sdk_epp_rx_register(uplus_ble_epp_data_from_external_to_tx);
	uplus_ble_sdk_config_reg_notify_cb(uplus_ble_cb_config_notify_callback, NULL);
	uplus_ble_sdk_time_ts_reg_notify_cb(uplus_ble_cb_time_ts_notify_callback, NULL);
	uplus_ble_sdk_conn_state_reg_notify_cb(uplus_ble_cb_conn_state_notify_callback, NULL);

	g_user_ctrl_handle.rpt_timer_id = UPLUS_USER_RSV1_TIMER;
	uplus_timer_create(g_user_ctrl_handle.rpt_timer_id,
		UPLUS_TIMER_REPEATED, 30* 1000, uplus_user_rpt_status_timer_func);

    extern void uplus_user_ota_init(void);
	uplus_user_ota_init();

	///////////////
	uplus_tool_free(init_cfg);

	//uplus_ble_sdk_config_start();
	//return;
}


int test_ble_profile_init(void);
int test_ble_adv_start(void);
int test_ble_adv_param(void);
void uplus_gap_evt_handler(uplus_ble_gap_evt_t evt,uplus_ble_gap_evt_param_t*param);
int uplus_gatts_evt_handler(uplus_ble_gatts_evt_t evt,uplus_ble_gatts_evt_param_t* param);
void test_uplug_uart_init(void);
void uplus_uart_handler(uplus_uart_data_t*  p_uart_data_pram);
void test_uplus_uart_stop(void);
void test_uplus_timer_cb(void);
void test_uplus_gpio(void);
int uplus_flash_area_test(void);


#define UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE 680
#define UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE_SUM (UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE+2+2)
/*
*  flag = 0:-  flag = 1:+
*/
#define UPLUS_INDICATE_SEND_PLUS 1
#define UPLUS_INDICATE_SEND_SUB  0
void uplus_indicate_send_data_num_set(int flag)
{

    if(flag)
    {
        g_user_ctrl_handle.uplus_indicate_send_data_num += UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE;
        UPLUS_BLE_LOG_INFO("uplus_indicate_send_data_num_set plus");
    }
    else
    {
        if(g_user_ctrl_handle.uplus_indicate_send_data_num >= UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE)
        {
            g_user_ctrl_handle.uplus_indicate_send_data_num -= UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE;
        }
        else
        {
            g_user_ctrl_handle.uplus_indicate_send_data_num = 0;
        }
        UPLUS_BLE_LOG_INFO("uplus_indicate_send_data_num_set sub");
    }

}
static void uplus_indicate_send_rate_test_cb(void)
{
    int ret = 0;
    float rate = 0;
    int index = 0;
    float msecond = 0;
    float second = 0;
    uplus_ble_sdk_ctrl_param_t param;
    unsigned int  time_stamp = 0;
	uplus_u8 rpt_data[UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE_SUM];
    static unsigned short tt_sn = 0;
	rpt_data[0] = 0xFF;
    rpt_data[1] = 0xFF;
    rpt_data[2] = (UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE >> 8)&0x00ff;//暂定大端
    rpt_data[3] = (UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE & 0x00FF);

    uplus_clear_timer_id(UPLUS_TIMER_USER2);
    for(index = 4; index < UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE_SUM; index++)
    {
        rpt_data[index] = index%10;
    }

    if (0 < g_user_ctrl_handle.connect_state) {

        msecond = uplus_systick_uptime_get() - g_user_ctrl_handle.uplus_indicate_send_data_base_time;
        second = msecond/1000 + 1;

        uplus_ble_sdk_time_ts_get(&time_stamp);
        memset(&param, 0, sizeof(uplus_ble_sdk_ctrl_param_t));
        param.data_len = UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE;
        param.data= rpt_data;

		//ret = uplus_ble_sdk_epp_tx_func(UPLUS_SEND_ONLY, UPLUS_BLE_SDK_CMD_TYPE_REPORT, &param, time_stamp);
		extern int32_t cae_ble_svr_gatt_stp_send_async(uint32_t sn , const uint8_t *data, uint32_t len, uint8_t is_priority);
        //ret = cae_ble_svr_gatt_stp_send_async(111, rpt_data, UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE_SUM, 0);
        ret = ucom_ble_svr_tt_send_async(0x2001, UPLUS_INDACATE_SINGLE_SEND_DATA_SIZE_SUM, rpt_data, 0, tt_sn, NULL);
/*

* @brief 发送透传数据，只发给uSDK
* @param type：测试数据类型（0x2001）
* @param len：测试数据长度
* @param data：测试数据
* @param is_enc：1加密数据   0非加密
* @param idx：该次发送的index
* @param cb：发送的结果回调，建议每次使用相同回调


ucom_int32_t ucom_ble_svr_tt_send_async(ucom_uint16_t type,
                                            ucom_uint16_t len,
                                            ucom_uint8_t *data,
                                            ucom_uint8_t is_enc,
                                            ucom_uint16_t idx,
                                            ucom_ble_svr_tt_send_cb cb);
*/
        if(ret)//full
        {
            uplus_timer_restart(UPLUS_TIMER_USER2, 500);
        }
        else
        {
            uplus_indicate_send_data_num_set(UPLUS_INDICATE_SEND_PLUS);
            uplus_timer_restart(UPLUS_TIMER_USER2, 50);
            tt_sn++;
        }
        rate = g_user_ctrl_handle.uplus_indicate_send_data_num/(second);
        UPLUS_BLE_LOG_INFO("Hp[%d %d %d]", xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize(), xPortGetPhysiceMemorySize);
		UPLUS_BLE_LOG_INFO("INDICATE_SEND_RATE = %d(Byte/s), second = %d(s), sum_data = %d, ret = %d.", (int)rate, (int)second, g_user_ctrl_handle.uplus_indicate_send_data_num, ret);

	}
	else
    {
        g_user_ctrl_handle.uplus_indicate_send_data_num = 0;
        g_user_ctrl_handle.uplus_indicate_send_data_base_time = uplus_systick_uptime_get();
        uplus_timer_restart(UPLUS_TIMER_USER2, 3*1000);
    }
    uplug_hal_wdt_feed();
}
#if 1
/*haier*/
int uplus_main_init(void)
{

    return 0;
}

int uplus_main(void)
{

    uplus_user_ble_init();
    uplus_timer_create(UPLUS_TIMER_USER2,UPLUS_TIMER_SINGLE_SHOT,50,uplus_indicate_send_rate_test_cb);
    uplus_timer_create(UPLUS_TIMER_USER3,UPLUS_TIMER_SINGLE_SHOT,50,uplus_ble_indicate_ack_handle_cb);
    return 0;
}
#else
int uplus_main(void)
{
    UPLUS_BLE_PRINTF("%s[%d] / %s", __FILE__, __LINE__, __func__);

    test_ble_adv_param();       //初始化广播参数
    uplus_ble_gap_init();       //ble 初始化
    test_ble_adv_start();       //开始广播

    uplus_ble_addr_t uplus_addr;
    uplus_ble_gap_address_get(uplus_addr); //获取地址
    UPLUS_BLE_PRINTF("\n-----ble 's address-----");
    UPLUS_BLE_HEXDUMP(uplus_addr,sizeof(uplus_addr));
    test_uplug_uart_init();
    uplus_timer_create(UPLUS_TIMER_USER,UPLUS_TIMER_SINGLE_SHOT,10000,test_uplus_uart_stop);
    uplus_timer_start(UPLUS_TIMER_USER);
    uplus_timer_create(UPLUS_TIMER_USER1,UPLUS_TIMER_REPEATED,1000,test_uplus_timer_cb);
    uplus_timer_start(UPLUS_TIMER_USER1);

    uplus_flash_area_test2();
    test_uplus_gpio();

    uplus_ble_gap_register(uplus_gap_evt_handler,UPLUS_BLE_SLAVE_ROLE);
    uplus_ble_gatts_register(uplus_gatts_evt_handler);
    uplus_uart_regester(uplus_uart_handler);

    extern int uplus_ble_sdk_user_config_init(void);
    uplus_ble_sdk_user_config_init();
    return 0;
}
#endif // 1


typedef  void(* uplus_uart_handler_t )( uplus_uart_data_t*  p_uart_data_pram);


/********************************timer test******************************/
void test_uplus_timer_cb(void)
{
    static u8 timer_cnt;
    UPLUS_BLE_PRINTF("%s[timer cnt:%d]",__func__,timer_cnt);

    if(timer_cnt == 12){
        uplus_timer_restart(UPLUS_TIMER_USER1,1500);
        timer_cnt ++;
    }else if(timer_cnt >= 18){
        uplus_timer_delete(UPLUS_TIMER_USER1);
        timer_cnt = 0;
    }else{
        timer_cnt++;
    }

}






/********************************drive test******************************/

void uplus_uart_handler(uplus_uart_data_t*  p_uart_data_pram)
{
    UPLUS_BLE_PRINTF("%s[uart id:%d]",__func__,p_uart_data_pram->uart_id);
    UPLUS_BLE_HEXDUMP(p_uart_data_pram->data_buff,p_uart_data_pram->data_len);

}

uplus_uart_t uart_init_pram;

void test_uplug_uart_init(void)
{

    UPLUS_BLE_PRINTF("%s[%d] / %s", __FILE__, __LINE__, __func__);
    uart_init_pram.uart_id = 0;
    uart_init_pram.baudrate = UPLUS_UART_BAUDRATE_9600;
    uart_init_pram.databit = UPLUS_UART_DATABIT_8;
    uart_init_pram.stopbit = UPLUS_UART_STOPBIT_1;
    uart_init_pram.parity = UPLUS_UART_PARITY_NO;
    uart_init_pram.hwflow_ctl = UPLUS_UART_HWFLOW_ENABLE;

    uplug_uart_init(&uart_init_pram);

}

void test_uplus_uart_stop(void)
{
    UPLUS_BLE_PRINTF("%s[%d] / %s", __FILE__, __LINE__, __func__);
    uplus_uart_deinit(0);
}






/********************************demo test******************************/
extern void set_notify_ready(void);
static u8* ble_name = "UPLUS(BLE)";
int uplus_gatts_evt_handler(uplus_ble_gatts_evt_t evt,uplus_ble_gatts_evt_param_t* param)
{
    int ret = 0;
    uint16_t att_value_len = 0;
    UPLUS_BLE_PRINTF("%s[event:%d]",__func__,evt);
    switch (evt)
    {
    case UPLUS_BLE_GATTS_EVT_WRITE:

         switch (param->write.value_handle)      //开notify 例子
        {
        case 0x0e:
            set_notify_ready();
            log_info("\n------write ccc:%04x,%02x\n", param->write.value_handle, param->write.data[0]);
            att_set_ccc_config(param->write.value_handle, param->write.data[0]);
            break;

        default:
            break;
        }

        break;
    case UPLUS_BLE_GATTS_EVT_READ:
        att_value_len = strlen(ble_name);
        if ((param->read.offset >= att_value_len) || (param->read.offset + param->read.offset) > att_value_len) {
            break;
        }

        if (param->read.data) {
            memcpy(param->read.data, ble_name, param->read.len);
            att_value_len = param->read.len;
            log_info("\n------read gap_name: %s \n", ble_name);
        }

        ret = att_value_len;
        break;


        break;

    default:
        break;
    }

    return ret;
}

void uplus_gap_evt_handler(uplus_ble_gap_evt_t evt,uplus_ble_gap_evt_param_t*param)
{
    UPLUS_BLE_PRINTF("%s[event:%d]",__func__,evt);
    switch (evt)
    {
    case UPLUS_BLE_GAP_EVT_CONNECTED:
        UPLUS_BLE_PRINTF("type:%d,role:%d,interval:%d,latency:%d,timeout:%d\n",param->connect.type,param->connect.role,param->connect.conn_param.min_conn_interval, \
        param->connect.conn_param.slave_latency,param->connect.conn_param.conn_sup_timeout);
        UPLUS_BLE_HEXDUMP(param->connect.peer_addr,6);
        break;

    case UPLUS_BLE_GAP_EVT_DISCONNET:
        break;

    case UPLUS_BLE_GAP_EVT_CONN_PARAM_UPDATED:
        break;

    case UPLUS_BLE_GAP_EVT_ADV_REPORT:  //主机没有实现
        break;

    default:
        break;
    }

}

int test_ble_adv_start(void)
{
    UPLUS_BLE_PRINTF("%s[%d] / %s", __FILE__, __LINE__, __func__);

    uplus_ble_gap_adv_param_t uplus_ble_gap_adv_param;
    uplus_ble_gap_adv_param.adv_interval_min = 160;
    uplus_ble_gap_adv_param.adv_interval_max = 320;
    uplus_ble_gap_adv_param.adv_type = UPLUS_BLE_ADV_TYPE_CONNECTABLE_UNDIRECTED;
    uplus_ble_gap_adv_param.ch_mask.ch_37_off &= ~BIT(0);
    uplus_ble_gap_adv_param.ch_mask.ch_38_off &= ~BIT(0);
    uplus_ble_gap_adv_param.ch_mask.ch_39_off &= ~BIT(0);

    printf("chdata[0x%x,0x%x,0x%x]",uplus_ble_gap_adv_param.ch_mask.ch_37_off,uplus_ble_gap_adv_param.ch_mask.ch_38_off,uplus_ble_gap_adv_param.ch_mask.ch_39_off);
    //printf("chmask[0x%x]",*uplus_ble_gap_adv_param.ch_mask);

    uplus_ble_gap_adv_start(&uplus_ble_gap_adv_param);
    return 0;
}

/********************************adv param demo test******************************/
int test_ble_adv_param(void)
{
    u8 p_data[] = {0x02,0x01,0x06,0x03,0x03,0x30,0xAF};
    u8 p_sr_data[] = {0x0B,0x09,0x75,0x70,0x6C,0x75,0x73,0x28,0x42,0x4C,0x45,0x29 };
    UPLUS_BLE_PRINTF("%s[%d] / %s", __FILE__, __LINE__, __func__);
    uplus_ble_adv_param_t adv_param;
    adv_param.interval_min = 160;
    adv_param.interval_max = 320;
    adv_param.type = UPLUS_BLE_ADV_TYPE_CONNECTABLE_UNDIRECTED;
    adv_param.own_address_type = 1; //设置地址类型
    uplus_ble_set_adv_param(&adv_param);

    uplus_ble_gap_adv_data_set(p_data,sizeof(p_data),p_sr_data,sizeof(p_sr_data));
    return 0;
}


/********************************profile demo test******************************/
// PRIMARY_SERVICE, 0x1800
// CHARACTERISTIC,  2A00, READ | DYNAMIC, VALUE 0x1234,

// SECONDARY_SERVICE, FE00
// CHARACTERISTIC,  FE01, READ | DYNAMIC,value 0x1234,

// PRIMARY_SERVICE, FF00
// CHARACTERISTIC,  FF01, READ | DYNAMIC,value 0x1234, descriptors 0x56565
// CHARACTERISTIC,  FF02, READ | WRITE | DYNAMIC,descriptors 0x56565
// CHARACTERISTIC,  FF03, READ | NOTIFY,
// CHARACTERISTIC,  FF04, WRITE | NOTIFY | DYNAMIC,

// PRIMARY_SERVICE, 00001016-d102-11e1-9b23-00025b00a5a5
// CHARACTERISTIC,  00001013-d102-11e1-9b23-00025b00a5a5, WRITE | READ
// CHARACTERISTIC,  00001018-d102-11e1-9b23-00025b00a5a5, WRITE
// CHARACTERISTIC,  00001014-d102-11e1-9b23-00025b00a5a5, DYNAMIC | READ | INDICATE
// CHARACTERISTIC,  00001011-d102-11e1-9b23-00025b00a5a5, READ | NOTIFY

u8 char_val_test[2] = {0x12,0x34};
uplus_ble_gatts_db_t gatts_test;
uplus_ble_gatts_srv_db_t srv_bd_test[4];
uplus_ble_gatts_char_db_t char1_bd_test[1];
uplus_ble_gatts_char_db_t char2_bd_test[1];
uplus_ble_gatts_char_db_t char3_bd_test[4];
uplus_ble_gatts_char_db_t char4_bd_test[1];

int test_ble_profile_init(void)
{
    UPLUS_BLE_PRINTF("%s[%d] / %s", __FILE__, __LINE__, __func__);


    u8 uuid128_test1[16] = {0x00,0x00,0x10,0x16,0xd1,0x02,0x11,0xe1,0x9b,0x23,0x00,0x02,0x5b,0x00,0xa5,0xa5};
    u8 uuid128_test2[16] = {0x00,0x00,0x10,0x13,0xd1,0x02,0x11,0xe1,0x9b,0x23,0x00,0x02,0x5b,0x00,0xa5,0xa5};

    memset(srv_bd_test,0x00,sizeof(srv_bd_test));
    memset(char1_bd_test,0x00,sizeof(char1_bd_test));
//service1
    char1_bd_test[0].char_uuid.type = 0;
    char1_bd_test[0].char_uuid.uuid16 = 0x2A00;
    char1_bd_test[0].char_property = 0x02;
    char1_bd_test[0].p_value = char_val_test;
    char1_bd_test[0].char_value_len = 2;
    char1_bd_test[0].char_handle.value_handle = 0x03;
    char1_bd_test[0].char_handle.user_desc_handle = 0;
    char1_bd_test[0].char_handle.cccd_handle = 0;
    char1_bd_test[0].char_handle.sccd_handle = 1;
    char1_bd_test[0].is_variable_len = 1;
    char1_bd_test[0].rd_author = 1;
    char1_bd_test[0].wr_author = 1;

    srv_bd_test[0].srv_type = UPLUS_BLE_PRIMARY_SERVICE;
    srv_bd_test[0].srv_handle = 0x0001;
    srv_bd_test[0].srv_uuid.type = 0;
    srv_bd_test[0].srv_uuid.uuid16 = 0x1800;
    srv_bd_test[0].char_num = 1;
    srv_bd_test[0].p_char_db = char1_bd_test;

//service2
    char2_bd_test[0].char_uuid.type = 0;
    char2_bd_test[0].char_uuid.uuid16 = 0xFE01;
    char2_bd_test[0].char_property = 0x02;
    char2_bd_test[0].p_value = char_val_test;
    char2_bd_test[0].char_value_len = 2;
    char2_bd_test[0].char_handle.value_handle = 0x06;
    char2_bd_test[0].char_handle.user_desc_handle = 0;
    char2_bd_test[0].char_handle.cccd_handle = 0;
    char2_bd_test[0].char_handle.sccd_handle = 1;
    char2_bd_test[0].is_variable_len = 1;
    char2_bd_test[0].rd_author = 1;
    char2_bd_test[0].wr_author = 1;

    srv_bd_test[1].srv_type = UPLUS_BLE_SECONDARY_SERVICE;
    srv_bd_test[1].srv_handle = 0x0004;
    srv_bd_test[1].srv_uuid.type = 0;
    srv_bd_test[1].srv_uuid.uuid16 = 0xFE00;
    srv_bd_test[1].char_num = 1;
    srv_bd_test[1].p_char_db = char2_bd_test;

//serice3
    char3_bd_test[0].char_uuid.type = 0;
    char3_bd_test[0].char_uuid.uuid16 = 0xFF01;
    char3_bd_test[0].char_property = 0x02;
    char3_bd_test[0].p_value = char_val_test;
    char3_bd_test[0].char_value_len = 0;
    char3_bd_test[0].char_handle.value_handle = 0x09;
    char3_bd_test[0].char_handle.user_desc_handle = 0;
    char3_bd_test[0].char_handle.cccd_handle = 0;
    char3_bd_test[0].char_handle.sccd_handle = 1;
    char3_bd_test[0].is_variable_len = 1;
    char3_bd_test[0].rd_author = 1;
    char3_bd_test[0].wr_author = 1;

    char3_bd_test[1].char_uuid.type = 0;
    char3_bd_test[1].char_uuid.uuid16 = 0xFF02;
    char3_bd_test[1].char_property = 0x0a;
    char3_bd_test[1].p_value = char_val_test;
    char3_bd_test[1].char_value_len = 0;
    char3_bd_test[1].char_handle.value_handle = 0x0b;
    char3_bd_test[1].char_handle.user_desc_handle = 0;
    char3_bd_test[1].char_handle.cccd_handle = 0;
    char3_bd_test[1].char_handle.sccd_handle = 1;
    char3_bd_test[1].is_variable_len = 1;
    char3_bd_test[1].rd_author = 1;
    char3_bd_test[1].wr_author = 1;

    char3_bd_test[2].char_uuid.type = 0;
    char3_bd_test[2].char_uuid.uuid16 = 0xFF03;
    char3_bd_test[2].char_property = 0x12;
    char3_bd_test[2].p_value = char_val_test;
    char3_bd_test[2].char_value_len = 2;
    char3_bd_test[2].char_handle.value_handle = 0x0d;
    char3_bd_test[2].char_handle.user_desc_handle = 0;
    char3_bd_test[2].char_handle.cccd_handle = 0;
    char3_bd_test[2].char_handle.sccd_handle = 1;
    char3_bd_test[2].is_variable_len = 0;
    char3_bd_test[2].rd_author = 1;
    char3_bd_test[2].wr_author = 1;

    char3_bd_test[3].char_uuid.type = 0;
    char3_bd_test[3].char_uuid.uuid16 = 0xFF04;
    char3_bd_test[3].char_property = 0x18;
    char3_bd_test[3].p_value = char_val_test;
    char3_bd_test[3].char_value_len = 0;
    char3_bd_test[3].char_handle.value_handle = 0x10;
    char3_bd_test[3].char_handle.user_desc_handle = 0;
    char3_bd_test[3].char_handle.cccd_handle = 0;
    char3_bd_test[3].char_handle.sccd_handle = 1;
    char3_bd_test[3].is_variable_len = 1;
    char3_bd_test[3].rd_author = 1;
    char3_bd_test[3].wr_author = 1;

    srv_bd_test[2].srv_type = UPLUS_BLE_PRIMARY_SERVICE;
    srv_bd_test[2].srv_handle = 0x0007;
    srv_bd_test[2].srv_uuid.type = 0;
    srv_bd_test[2].srv_uuid.uuid16 = 0xFF00;
    srv_bd_test[2].char_num = 4;
    srv_bd_test[2].p_char_db = char3_bd_test;

//serice4
    char4_bd_test[0].char_uuid.type = 1;
    memcpy(char4_bd_test[0].char_uuid.uuid128,uuid128_test2,16);
    char4_bd_test[0].char_property = 0x2a;
    char4_bd_test[0].p_value = char_val_test;
    char4_bd_test[0].char_value_len = 0;
    char4_bd_test[0].char_handle.value_handle = 0x14;
    char4_bd_test[0].char_handle.user_desc_handle = 0;
    char4_bd_test[0].char_handle.cccd_handle = 0;
    char4_bd_test[0].char_handle.sccd_handle = 1;
    char4_bd_test[0].is_variable_len = 1;
    char4_bd_test[0].rd_author = 1;
    char4_bd_test[0].wr_author = 1;

    srv_bd_test[3].srv_type = UPLUS_BLE_PRIMARY_SERVICE;
    srv_bd_test[3].srv_handle = 0x0012;
    srv_bd_test[3].srv_uuid.type = 1;

    memcpy(srv_bd_test[3].srv_uuid.uuid128,uuid128_test1,16);
    srv_bd_test[3].char_num = 1;
    srv_bd_test[3].p_char_db = char4_bd_test;

    gatts_test.p_srv_db = srv_bd_test;
    gatts_test.srv_num = 4;


    UPLUS_BLE_PRINTF("gatts_test:0x%x,0x%x",gatts_test,&gatts_test);
    uplus_ble_gatts_service_init(&gatts_test);


    return 0;

}




