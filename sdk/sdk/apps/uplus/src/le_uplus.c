/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 20:31:37
 * @LastEditTime: 2021-09-08 10:59:13
 * @LastEditors: jianye
 */

#include "system/app_core.h"
#include "system/includes.h"

#include "app_config.h"
#include "app_action.h"

#include "btstack/btstack_task.h"
#include "btstack/bluetooth.h"
#include "user_cfg.h"
#include "vm.h"
#include "btcontroller_modules.h"
#include "bt_common.h"
#include "3th_profile_api.h"

#include "include/le_uplus.h"
#include "le_common.h"

#include "rcsp_bluetooth.h"
#include "JL_rcsp_api.h"
#include "custom_cfg.h"
#include "include/includes.h"

#include "app_main.h"


#define LOG_TAG_CONST       Le_UPlus  //保持与文件名同步
#define LOG_TAG             "[Le_UPlus]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE


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

#if (TCFG_BLE_DEMO_SELECT == DEF_BLE_DEMO_UPLAS_TRANS)

//TRANS ANCS
#define TRANS_ANCS_EN  			  	 0
#if TRANS_ANCS_EN
#include "btstack/btstack_event.h"
#endif

#define TEST_SEND_DATA_RATE          0  //测试上行发送数据
#define TEST_SEND_HANDLE_VAL         ATT_CHARACTERISTIC_ae02_01_VALUE_HANDLE
/* #define TEST_SEND_HANDLE_VAL         ATT_CHARACTERISTIC_ae05_01_VALUE_HANDLE */
#define EXT_ADV_MODE_EN              0

#define TEST_AUDIO_DATA_UPLOAD       0 //测试文件上传


#if LE_DEBUG_PRINT_EN

extern void printf_buf(u8 *buf, u32 len);
/* #define log_info          printf */
//#define log_info(x, ...)  printf("[LE_TRANS]" x " ", ## __VA_ARGS__)
//#define log_info_hexdump  printf_buf
#else
#define log_info(...)
#define log_info_hexdump(...)
#endif


/* #define LOG_TAG_CONST       BT_BLE */
/* #define LOG_TAG             "[LE_S_DEMO]" */
/* #define LOG_ERROR_ENABLE */
/* #define LOG_DEBUG_ENABLE */
/* #define LOG_INFO_ENABLE */
/* #define LOG_DUMP_ENABLE */
/* #define LOG_CLI_ENABLE */
/* #include "debug.h" */

//------
//ATT发送的包长,    note: 20 <=need >= MTU
#define ATT_LOCAL_MTU_SIZE    (250)                   //
//ATT缓存的buffer大小,  note: need >= 20,可修改
#define ATT_SEND_CBUF_SIZE        (512)                   //

static u16 save_mtu = 0;

static u16 att_local_mtu_size = ATT_LOCAL_MTU_SIZE;


#define ATT_RAM_BUFSIZE           0x1000    /* 0x1000 4kB */    //(ATT_CTRL_BLOCK_SIZE + ATT_LOCAL_MTU_SIZE + ATT_SEND_CBUF_SIZE)                   //note:
//共配置的RAM

void update_mtu_default(u16 mtu)
{
    att_local_mtu_size = mtu;
    ble_vendor_set_default_att_mtu(att_local_mtu_size);
}
static u8 att_ram_buffer[ATT_RAM_BUFSIZE] __attribute__((aligned(4)));
//---------------

#if TEST_SEND_DATA_RATE
static u32 test_data_count;
static u32 server_timer_handle = 0;
static u8 test_data_start;
#endif

/*
 打开流控使能后,确定使能接口 att_server_flow_enable 被调用
 然后使用过程 通过接口 att_server_flow_hold 来控制流控开关
 注意:流控只能控制对方使用带响应READ/WRITE等命令方式
 例如:ATT_WRITE_REQUEST = 0x12
 */
#define ATT_DATA_RECIEVT_FLOW           0//流控功能使能

//---------------
// 广播周期 (unit:0.625ms)
#define ADV_INTERVAL_MIN          (160*5)

#define HOLD_LATENCY_CNT_MIN  (3)  //(0~0xffff)
#define HOLD_LATENCY_CNT_MAX  (15) //(0~0xffff)
#define HOLD_LATENCY_CNT_ALL  (0xffff)

static volatile hci_con_handle_t con_handle;

//加密设置
/* static const uint8_t sm_min_key_size = 7; */

//连接参数更新请求设置
//是否使能参数请求更新,0--disable, 1--enable
static const uint8_t connection_update_enable = 1; ///0--disable, 1--enable
//当前请求的参数表index
static uint8_t connection_update_cnt = 0; //

#define BUFFER_OK   6
#define BUFFER_NO   6
#define BUFF_OFF    6

static u8 buffer_ok[BUFFER_OK] = {0x02,0x40,0x50,0x01,0x01,0x3a};
static u8 buffer_no[BUFFER_OK] = {0x02,0x40,0x50,0x01,0x02,0x3a};
static u8 buffer_off[BUFF_OFF] = {0x02,0x40,0x50,0xff,0x03,0x00};

extern int at_uart_send_packet(const u8 *packet, int size);

#if 0
//参数表
static const struct conn_update_param_t connection_param_table[] = {
    {16, 24, 10, 600},//11
    {12, 28, 10, 600},//3.7
    {8,  20, 10, 600},
    /* {12, 28, 4, 600},//3.7 */
    /* {12, 24, 30, 600},//3.05 */
};

#else//yuanlf
static const struct conn_update_param_t connection_param_table[] = {
    //{6, 9, 0, 400},//11
    {28, 28, 0, 400},//11
    {10, 28, 10, 600},//3.7
    {8,  20, 10, 600},
    /* {12, 28, 4, 600},//3.7 */
    /* {12, 24, 30, 600},//3.05 */
};

#endif
//共可用的参数组数
#define CONN_PARAM_TABLE_CNT      (sizeof(connection_param_table)/sizeof(struct conn_update_param_t))

#if (ATT_RAM_BUFSIZE < 64)
#error "adv_data & rsp_data buffer error!!!!!!!!!!!!"
#endif

//用户可配对的，这是样机跟客户开发的app配对的秘钥
/* const u8 link_key_data[16] = {0x06, 0x77, 0x5f, 0x87, 0x91, 0x8d, 0xd4, 0x23, 0x00, 0x5d, 0xf1, 0xd8, 0xcf, 0x0c, 0x14, 0x2b}; */
#define EIR_TAG_STRING   0xd6, 0x05, 0x08, 0x00, 'J', 'L', 'A', 'I', 'S', 'D','K'
static const char user_tag_string[] = {EIR_TAG_STRING};

static u8 adv_data_len;
static u8 adv_data[ADV_RSP_PACKET_MAX];//max is 31
static u8 scan_rsp_data_len;
static u8 scan_rsp_data[ADV_RSP_PACKET_MAX];//max is 31

/* #define adv_data       &att_ram_buffer[0] */
/* #define scan_rsp_data  &att_ram_buffer[32] */

static char gap_device_name[BT_NAME_LEN_MAX] = "jl_ble_test";
static u8 gap_device_name_len = 0; //名字长度，不包含结束符
static u8 ble_work_state = 0;      //ble 状态变化
static u8 adv_ctrl_en;             //广播控制

static u8 test_read_write_buf[4];

static void (*app_recieve_callback)(void *priv, void *buf, u16 len) = NULL;
static void (*app_ble_state_callback)(void *priv, ble_state_e state) = NULL;
static void (*ble_resume_send_wakeup)(void) = NULL;
static u32 channel_priv;

static int app_send_user_data_check(u16 len);
static int app_send_user_data_do(void *priv, u8 *data, u16 len);
 int app_send_user_data(u16 handle, u8 *data, u16 len, u8 handle_type);

// Complete Local Name  默认的蓝牙名字

//------------------------------------------------------
//广播参数设置
static void advertisements_setup_init();
static int set_adv_enable(void *priv, u32 en);
static int get_buffer_vaild_len(void *priv);
extern const char *bt_get_local_name();
extern void clr_wdt(void);
extern void sys_auto_shut_down_disable(void);
extern void sys_auto_shut_down_enable(void);
extern u8 get_total_connect_dev(void);
//------------------------------------------------------
//uplus
u32 get_uplus_profile(void);
extern u8 *p_save_profile;

uplus_ble_gap_evt_t uplus_evt;
uplus_ble_gap_evt_param_t uplus_evt_paramm;
extern int uplus_ble_cb_gap_event_send(uplus_ble_gap_evt_t evt,uplus_ble_gap_evt_param_t*param);

uplus_ble_gatts_evt_t uplus_gatts_evt;
uplus_ble_gatts_evt_param_t uplue_gatts_evt_param;

extern u8 uuid_sive_buf[TEST_SERVICE_NUM_MAX][TEST_CHAR_NUM_MAX];
extern int get_char_uuid(u8 svr,u8 uchar,uplus_ble_uuid_t *uuid_data);
extern int uplus_ble_cb_gatts_event_send(uplus_ble_gatts_evt_t evt,uplus_ble_gatts_evt_param_t*param);
extern int get_notify_indicate_char_uuid(u8 svr,u8 uchar,uplus_ble_uuid_t *uuid_data);

//------------------------------------------------------
//NACS
#if TRANS_ANCS_EN
#define ANCS_SUBEVENT_CLIENT_NOTIFICATION                           0xF1
void ancs_client_init(void);
void ancs_client_register_callback(btstack_packet_handler_t callback);
const char *ancs_client_attribute_name_for_id(int id);
void ancs_set_notification_buffer(u8 *buffer, u16 buffer_size);

//ancs info buffer
#define ANCS_INFO_BUFFER_SIZE  (1024)
static u8 ancs_info_buffer[ANCS_INFO_BUFFER_SIZE];
#endif

//------------------------------------------------------
#if TEST_AUDIO_DATA_UPLOAD
static const u8 test_audio_data_file[1024] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9
};

#define AUDIO_ONE_PACKET_LEN  128
static void test_send_audio_data(int init_flag)
{
    static u32 send_pt = 0;
    static u32 start_flag = 0;

    if (!con_handle) {
        return;
    }

    if (init_flag) {
        log_info("audio send init\n");
        send_pt = 0;
        start_flag = 1;
    }

    if (!start_flag) {
        return;
    }

    u32 file_size = sizeof(test_audio_data_file);
    u8 *file_ptr = test_audio_data_file;

    if (send_pt >= file_size) {
        log_info("audio send Complete\n");
        start_flag = 0;
        return;
    }

    u32 send_len = file_size - send_pt;
    if (send_len > AUDIO_ONE_PACKET_LEN) {
        send_len = AUDIO_ONE_PACKET_LEN;
    }

    while (1) {
        if (app_send_user_data_check(send_len)) {
            log_info("audio send %08x\n", send_pt);
            if (app_send_user_data(ATT_CHARACTERISTIC_ae3c_01_VALUE_HANDLE, &file_ptr[send_pt], send_len, ATT_OP_AUTO_READ_CCC)) {
                log_info("audio send fail!\n");
                break;
            } else {
                send_pt += send_len;
            }
        } else {
            break;
        }
    }
}

#endif


static void send_request_connect_parameter(u8 table_index)
{
    struct conn_update_param_t *param = (void *)&connection_param_table[table_index];//static ram

    log_info("update_request:-%d-%d-%d-%d-\n", param->interval_min, param->interval_max, param->latency, param->timeout);
    if (con_handle) {
        ble_op_conn_param_request(con_handle, param);
    }
}

static void check_connetion_updata_deal(void)
{
    if (connection_update_enable) {
        if (connection_update_cnt < CONN_PARAM_TABLE_CNT) {
            send_request_connect_parameter(connection_update_cnt);
        }
    }
}

static void connection_update_complete_success(u8 *packet)
{
    int con_handle, conn_interval, conn_latency, conn_timeout;

    con_handle = hci_subevent_le_connection_update_complete_get_connection_handle(packet);
    conn_interval = hci_subevent_le_connection_update_complete_get_conn_interval(packet);
    conn_latency = hci_subevent_le_connection_update_complete_get_conn_latency(packet);
    conn_timeout = hci_subevent_le_connection_update_complete_get_supervision_timeout(packet);

    uplus_evt = UPLUS_BLE_GAP_EVT_CONN_PARAM_UPDATED;
    uplus_evt_paramm.update_conn.conn_param.min_conn_interval = conn_interval;
    uplus_evt_paramm.update_conn.conn_param.max_conn_interval = conn_latency;
    uplus_evt_paramm.update_conn.conn_param.slave_latency = conn_latency;
    uplus_evt_paramm.update_conn.conn_param.conn_sup_timeout = conn_timeout;
    uplus_ble_cb_gap_event_send(uplus_evt,&uplus_evt_paramm);

    log_info("conn_interval = %d\n", conn_interval);
    log_info("conn_latency = %d\n", conn_latency);
    log_info("conn_timeout = %d\n", conn_timeout);
}


static void set_ble_work_state(ble_state_e state)
{
    if (state != ble_work_state) {
        log_info("ble_work_st:%x->%x\n", ble_work_state, state);
        ble_work_state = state;
        if (app_ble_state_callback) {
            app_ble_state_callback((void *)channel_priv, state);
        }
    }
}

static ble_state_e get_ble_work_state(void)
{
    return ble_work_state;
}

unsigned int uplus_get_ble_work_state(void)
{
    return ble_work_state;
}

static void cbk_sm_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    sm_just_event_t *event = (void *)packet;
    u32 tmp32;
    switch (packet_type) {
    case HCI_EVENT_PACKET:
        switch (hci_event_packet_get_type(packet)) {
        case SM_EVENT_JUST_WORKS_REQUEST:
            sm_just_works_confirm(sm_event_just_works_request_get_handle(packet));
            log_info("Just Works Confirmed.\n");
            break;
        case SM_EVENT_PASSKEY_DISPLAY_NUMBER:
            log_info_hexdump(packet, size);
            memcpy(&tmp32, event->data, 4);
            log_info("Passkey display: %06u.\n", tmp32);
            break;
        }
        break;
    }
}


#if TEST_SEND_DATA_RATE
static void server_timer_handler(void)
{
    if (!con_handle) {
        test_data_count = 0;
        test_data_start = 0;
        return;
    }

    log_info("peer_rssi = %d\n", ble_vendor_get_peer_rssi(con_handle));

    if (test_data_count) {
        log_info("\n%d bytes send: %d.%02d KB/s \n", test_data_count, test_data_count / 1000, test_data_count % 1000);
        test_data_count = 0;
    }
}

static void server_timer_start(void)
{
    if (server_timer_handle) {
        return;
    }

    server_timer_handle  = sys_timer_add(NULL, server_timer_handler, 1000);
}

static void server_timer_stop(void)
{
    if (server_timer_handle) {
        sys_timeout_del(server_timer_handle);
        server_timer_handle = 0;
    }
}

void test_data_send_packet(void)
{
    u32 vaild_len = get_buffer_vaild_len(0);//获取发送buffer可写入的数据
    if (!test_data_start) {
        return;
    }

    if (vaild_len) {
        if (!app_send_user_data(TEST_SEND_HANDLE_VAL, (void *)&test_data_count, vaild_len, ATT_OP_AUTO_READ_CCC)) {
            test_data_count += vaild_len;
        }
    }
    clr_wdt();
}
#endif


static void can_send_now_wakeup(void)
{
    /* putchar('E'); */
    if (ble_resume_send_wakeup) {
        ble_resume_send_wakeup();
    }

#if TEST_SEND_DATA_RATE
    test_data_send_packet();
#endif

#if TEST_AUDIO_DATA_UPLOAD
    test_send_audio_data(0);
#endif

}

static void ble_auto_shut_down_enable(u8 enable)
{
#if TCFG_AUTO_SHUT_DOWN_TIME
    if (enable) {
        if (get_total_connect_dev() == 0) {    //已经没有设备连接
            sys_auto_shut_down_enable();
        }
    } else {
        sys_auto_shut_down_disable();
    }
#endif
}

const char *const phy_result[] = {
    "None",
    "1M",
    "2M",
    "Coded",
};

static void set_connection_data_length(u16 tx_octets, u16 tx_time)
{
    if (con_handle) {
        ble_op_set_data_length(con_handle, tx_octets, tx_time);
    }
}

static void set_connection_data_phy(u8 tx_phy, u8 rx_phy)
{
    if (0 == con_handle) {
        return;
    }

    u8 all_phys = 0;
    u16 phy_options = CONN_SET_PHY_OPTIONS_S8;

    ble_op_set_ext_phy(con_handle, all_phys, tx_phy, rx_phy, phy_options);
}

static void server_profile_start(u16 con_handle)
{
#if BT_FOR_APP_EN
    set_app_connect_type(TYPE_BLE);
#endif
    ble_op_att_send_init(con_handle, att_ram_buffer, ATT_RAM_BUFSIZE, att_local_mtu_size);
    set_ble_work_state(BLE_ST_CONNECT);
    att_server_set_exchange_mtu(con_handle);
    ble_auto_shut_down_enable(0);

    /* set_connection_data_phy(CONN_SET_CODED_PHY, CONN_SET_CODED_PHY); */
}

_WEAK_
u8 ble_update_get_ready_jump_flag(void)
{
    return 0;
}


#define ERROR_CODE_SUCCESS                                    0x00
#define ERROR_CODE_PAGE_TIMEOUT                               0x04
#define ERROR_CODE_AUTHENTICATION_FAILURE                     0x05
#define ERROR_CODE_PIN_OR_KEY_MISSING                         0x06
#define ERROR_CODE_CONNECTION_TIMEOUT                         0x08
#define ERROR_CODE_SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED  0x0A
#define ERROR_CODE_ACL_CONNECTION_ALREADY_EXISTS                      0x0B
#define ERROR_CODE_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES       0x0D
#define ERROR_CODE_CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR    0x0F
#define ERROR_CODE_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED         0x10
#define ERROR_CODE_REMOTE_USER_TERMINATED_CONNECTION          0x13
#define ERROR_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST        0x16
/*
 * @section Packet Handler
 *
 * @text The packet handler is used to:
 *        - stop the counter after a disconnect
 *        - send a notification when the requested ATT_EVENT_CAN_SEND_NOW is received
 */

/* LISTING_START(packetHandler): Packet Handler */
static void cbk_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    int mtu;
    u32 tmp;
    u8 status;
    const char *attribute_name;

    switch (packet_type) {
    case HCI_EVENT_PACKET:
        switch (hci_event_packet_get_type(packet)) {

        /* case DAEMON_EVENT_HCI_PACKET_SENT: */
        /* break; */
        case ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE:
            log_info("ATT_EVENT_HANDLE_VALUE_INDICATION_COMPLETE, %s\n", os_current_task());
            unsigned int time_ms = 0;
            time_ms = uplus_systick_uptime_get();
            UPLUS_BLE_LOG_INTERNAL_DEBUG("%s, time_ms3 = %d", __func__, time_ms);
            extern void uplus_indicate_ack_handle_timer_start(u8 flag);
            uplus_indicate_ack_handle_timer_start(OK_INDICATE_ACK);//success

        case ATT_EVENT_CAN_SEND_NOW:

            can_send_now_wakeup();
            log_info("**************ATT_EVENT_CAN_SEND_NOW*******");
            break;
        case HCI_EVENT_LE_META:
            switch (hci_event_le_meta_get_subevent_code(packet)) {
            case HCI_SUBEVENT_LE_ENHANCED_CONNECTION_COMPLETE:
                status = hci_subevent_le_enhanced_connection_complete_get_status(packet);
                if (status) {
                    log_info("LE_SLAVE CONNECTION FAIL!!! %0x\n", status);
                    set_ble_work_state(BLE_ST_DISCONN);
                    break;
                }
                con_handle = hci_subevent_le_enhanced_connection_complete_get_connection_handle(packet);
                log_info("HCI_SUBEVENT_LE_ENHANCED_CONNECTION_COMPLETE : %0x\n", con_handle);
                log_info("conn_interval = %d\n", hci_subevent_le_enhanced_connection_complete_get_conn_interval(packet));
                log_info("conn_latency = %d\n", hci_subevent_le_enhanced_connection_complete_get_conn_latency(packet));
                log_info("conn_timeout = %d\n", hci_subevent_le_enhanced_connection_complete_get_supervision_timeout(packet));
                server_profile_start(con_handle);
                break;

            case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
                con_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);

                log_info("HCI_SUBEVENT_LE_CONNECTION_COMPLETE: %0x\n", con_handle);
                at_uart_send_packet(buffer_ok,6);
                put_buf(packet,size);
                connection_update_complete_success(packet + 8);
                server_profile_start(con_handle);

#if RCSP_BTMATE_EN
#if (defined(BT_CONNECTION_VERIFY) && (0 == BT_CONNECTION_VERIFY))
                JL_rcsp_auth_reset();
#endif
                //rcsp_dev_select(RCSP_BLE);
                rcsp_init();
#endif

                uplus_evt = UPLUS_BLE_GAP_EVT_CONNECTED;

                /* 对事件进行赋值 */

                hci_subevent_le_connection_complete_get_peer_address(packet,uplus_evt_paramm.connect.peer_addr);
                uplus_evt_paramm.conn_handle = con_handle;
                uplus_evt_paramm.connect.type = hci_subevent_le_connection_complete_get_peer_address_type(packet);
                uplus_evt_paramm.connect.role = hci_subevent_le_connection_complete_get_role(packet);

                uplus_evt_paramm.connect.conn_param.min_conn_interval = hci_subevent_le_connection_complete_get_conn_interval(packet);
                uplus_evt_paramm.connect.conn_param.max_conn_interval = uplus_evt_paramm.connect.conn_param.min_conn_interval;
                uplus_evt_paramm.connect.conn_param.slave_latency = hci_subevent_le_connection_complete_get_conn_latency(packet);
                uplus_evt_paramm.connect.conn_param.conn_sup_timeout = hci_subevent_le_connection_complete_get_supervision_timeout(packet);


                uplus_ble_cb_gap_event_send(uplus_evt,&uplus_evt_paramm);
                log_info("ble remote rssi= %d\n", ble_vendor_get_peer_rssi(con_handle));


                break;

            case HCI_SUBEVENT_LE_CONNECTION_UPDATE_COMPLETE:
                connection_update_complete_success(packet);
                break;

            case HCI_SUBEVENT_LE_DATA_LENGTH_CHANGE:
                log_info("APP HCI_SUBEVENT_LE_DATA_LENGTH_CHANGE\n");
                /* set_connection_data_phy(CONN_SET_CODED_PHY, CONN_SET_CODED_PHY); */
                break;

            case HCI_SUBEVENT_LE_PHY_UPDATE_COMPLETE:
                log_info("APP HCI_SUBEVENT_LE_PHY_UPDATE %s\n", hci_event_le_meta_get_phy_update_complete_status(packet) ? "Fail" : "Succ");
                log_info("Tx PHY: %s\n", phy_result[hci_event_le_meta_get_phy_update_complete_tx_phy(packet)]);
                log_info("Rx PHY: %s\n", phy_result[hci_event_le_meta_get_phy_update_complete_rx_phy(packet)]);
                break;
            }
            break;

        case HCI_EVENT_DISCONNECTION_COMPLETE:
            log_info("HCI_EVENT_DISCONNECTION_COMPLETE: %0x\n", packet[5]);
            
			at_uart_send_packet(buffer_no,6);
#if RCSP_BTMATE_EN
            rcsp_exit();
#endif
            con_handle = 0;
            ble_op_att_send_init(con_handle, 0, 0, 0);
            set_ble_work_state(BLE_ST_DISCONN);

            if (!ble_update_get_ready_jump_flag()) {
                //bt_ble_adv_enable(1);
            }
            connection_update_cnt = 0;
#if BT_FOR_APP_EN
            set_app_connect_type(TYPE_NULL);
#endif
            ble_auto_shut_down_enable(1);

            uplus_evt = UPLUS_BLE_GAP_EVT_DISCONNET;
            switch (packet[5])
            {
            case ERROR_CODE_CONNECTION_TIMEOUT:
                uplus_evt_paramm.disconnect.reason = CONNECTION_TIMEOUT;
                break;

            case ERROR_CODE_REMOTE_USER_TERMINATED_CONNECTION:
                uplus_evt_paramm.disconnect.reason = REMOTE_USER_TERMINATED;
                break;

            case ERROR_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST:
                uplus_evt_paramm.disconnect.reason = LOCAL_HOST_TERMINATED;
                break;

            default:
                break;
            }

            uplus_ble_cb_gap_event_send(uplus_evt,&uplus_evt_paramm);

            break;

        case ATT_EVENT_MTU_EXCHANGE_COMPLETE:
            mtu = att_event_mtu_exchange_complete_get_MTU(packet) - 3;
            log_info("ATT MTU = %u\n", mtu);
            ble_op_att_set_send_mtu(mtu);
            save_mtu = mtu;
            /* set_connection_data_length(251, 2120); */
            break;

        case HCI_EVENT_VENDOR_REMOTE_TEST:
            log_info("--- HCI_EVENT_VENDOR_REMOTE_TEST\n");
            break;

        case L2CAP_EVENT_CONNECTION_PARAMETER_UPDATE_RESPONSE:
            tmp = little_endian_read_16(packet, 4);
            log_info("-update_rsp: %02x\n", tmp);
            if (tmp) {
                connection_update_cnt++;
                log_info("remoter reject!!!\n");
                check_connetion_updata_deal();
            } else {
                connection_update_cnt = CONN_PARAM_TABLE_CNT;
            }
            break;

        case HCI_EVENT_ENCRYPTION_CHANGE:
            log_info("HCI_EVENT_ENCRYPTION_CHANGE= %d\n", packet[2]);
            break;

#if TRANS_ANCS_EN
        case HCI_EVENT_ANCS_META:
            switch (hci_event_ancs_meta_get_subevent_code(packet)) {
            case ANCS_SUBEVENT_CLIENT_NOTIFICATION:
                printf("ANCS_SUBEVENT_CLIENT_NOTIFICATION \n");
                attribute_name = ancs_client_attribute_name_for_id(ancs_subevent_client_notification_get_attribute_id(packet));
                if (!attribute_name) {
                    printf("ancs unknow attribute_id :%d \n", ancs_subevent_client_notification_get_attribute_id(packet));
                    break;
                } else {
                    u16 attribute_strlen = little_endian_read_16(packet, 7);
                    u8 *attribute_str = (void *)little_endian_read_32(packet, 9);
                    printf("Notification: %s - %s \n", attribute_name, attribute_str);
                }
                break;
            default:
                break;
            }

            break;
#endif

        }
        break;
    }
}


/* LISTING_END */

/*
 * @section ATT Read
 *
 * @text The ATT Server handles all reads to constant data. For dynamic data like the custom characteristic, the registered
 * att_read_callback is called. To handle long characteristics and long reads, the att_read_callback is first called
 * with buffer == NULL, to request the total value length. Then it will be called again requesting a chunk of the value.
 * See Listing attRead.
 */

/* LISTING_START(attRead): ATT Read */

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// @param offset defines start of attribute value
static uint16_t att_read_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{

    uint16_t  att_value_len = 0;
    uint16_t handle = att_handle;
    int ret = 0;

    log_info("read_callback, handle= 0x%04x,buffer= %08x\n", handle, (u32)buffer);

    g_printf("%s[buffer_size:%d][offset:%d]\n",__func__,buffer_size, offset);

    if (buffer) {

        log_info("uplue_gatts_evt_param.read.len = %d.\n", uplue_gatts_evt_param.read.len);
        if(uplue_gatts_evt_param.read.len > 0 && buffer_size > 0)
        {
            memcpy(buffer, &uplue_gatts_evt_param.read.data[offset], uplue_gatts_evt_param.read.len);
            put_buf(buffer,uplue_gatts_evt_param.read.len);
            free(uplue_gatts_evt_param.read.data);
        }
        else
        {
            //do nothing
        }
        att_value_len = uplue_gatts_evt_param.read.len;
        if ((offset >= att_value_len) || (offset + buffer_size) > att_value_len) {
            log_info("att_read_callback, error len!!.\n");
            return 0;
        }
    }else{
        uplus_gatts_evt = UPLUS_BLE_GATTS_EVT_READ;
        uplue_gatts_evt_param.conn_handle = connection_handle;
        uplue_gatts_evt_param.read.offset = offset;
        for(int i=0;i<TEST_SERVICE_NUM_MAX;i++){
            for (int j = 0; j < TEST_CHAR_NUM_MAX; j++){
                if(uuid_sive_buf[i][j] == handle){
                    get_char_uuid(i,j,&uplue_gatts_evt_param.read.uuid);
                    uplue_gatts_evt_param.read.value_handle = att_handle;
                    goto read_find_uuid;
                }
            }
        }

        for(int i=0;i<TEST_SERVICE_NUM_MAX;i++){
            for (int j = 0; j < TEST_CHAR_NUM_MAX; j++){
                if((uuid_sive_buf[i][j]+1 )== handle){  //带notify的特征handle会比不带的多加1
                    get_notify_indicate_char_uuid(i,j,&uplue_gatts_evt_param.read.uuid);
                    uplue_gatts_evt_param.read.value_handle = att_handle-1;
                    break;
                }
            }
        }
read_find_uuid:
        ret = uplus_ble_cb_gatts_event_send(uplus_gatts_evt,&uplue_gatts_evt_param);
        if(ret == UPLUS_BLE_SUCCESS){
            att_value_len = uplue_gatts_evt_param.read.len;
        }else{
            log_info("%s[%s], read.len = %d;\n",__func__,"read callback data error!!!", uplue_gatts_evt_param.read.len);
            att_value_len = 0;
        }
    }

    log_info("att_value_len= %d\n", att_value_len);
    wdt_clear();

    return att_value_len;

}

/* LISTING_END */
/*
 * @section ATT Write
 *
 * @text The only valid ATT write in this example is to the Client Characteristic Configuration, which configures notification
 * and indication. If the ATT handle matches the client configuration handle, the new configuration value is stored and used
 * in the heartbeat handler to decide if a new value should be sent. See Listing attWrite.
 */

/* LISTING_START(attWrite): ATT Write */
static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    int ret = 0;
    int result = 0;
    u16 tmp16;

    u16 handle = att_handle;

    log_info("write_callback, handle= 0x%04x,size = %d\n", handle, buffer_size);
    put_buf(buffer,buffer_size);

    uplus_gatts_evt = UPLUS_BLE_GATTS_EVT_WRITE;
    uplue_gatts_evt_param.conn_handle = connection_handle;
    //uplue_gatts_evt_param.write.value_handle = handle;

    for(int i=0;i<TEST_SERVICE_NUM_MAX;i++){
        for (int j = 0; j < TEST_CHAR_NUM_MAX; j++)
        {
            if(uuid_sive_buf[i][j] == handle){
                get_char_uuid(i,j,&(uplue_gatts_evt_param.write.uuid));
                uplue_gatts_evt_param.write.value_handle = handle;
                goto write_find_uuid;
            }
        }
    }

    for(int i=0;i<TEST_SERVICE_NUM_MAX;i++){
        for (int j = 0; j < TEST_CHAR_NUM_MAX; j++){
            if((uuid_sive_buf[i][j]+1 )== handle){
                get_notify_indicate_char_uuid(i,j,&uplue_gatts_evt_param.write.uuid);
                uplue_gatts_evt_param.write.value_handle = handle - 1;
                break;
            }
        }
    }
write_find_uuid:
    uplue_gatts_evt_param.write.data = buffer;
    uplue_gatts_evt_param.write.offset = offset;
    uplue_gatts_evt_param.write.len = buffer_size;

    ret = uplus_ble_cb_gatts_event_send(uplus_gatts_evt,&uplue_gatts_evt_param);
    att_set_ccc_config(uplue_gatts_evt_param.write.value_handle, uplue_gatts_evt_param.write.data[0]);

    log_info("write_callback ret = %d, buffer_size = %d, offset = %d.\n", ret, buffer_size, offset);
    //uplue_gatts_evt_param.write.op = 0; /* 留空 */
    wdt_clear();

    return ret;
}


void set_notify_ready(void)
{
    set_ble_work_state(BLE_ST_NOTIFY_IDICATE);
    check_connetion_updata_deal();
}

int app_send_user_data(u16 handle, u8 *data, u16 len, u8 handle_type)
{
    u32 ret = APP_BLE_NO_ERROR;

    if (!con_handle) {
        return APP_BLE_OPERATION_ERROR;
    }

#if 0
    if (!att_get_ccc_config(handle + 1)) {
        log_info("fail,no write ccc!!!,%04x\n", handle + 1);
        return APP_BLE_NO_WRITE_CCC;
    }
#else
    if (!att_get_ccc_config(handle)) {
        log_info("fail,no write ccc!!!,%04x\n", handle);
        return APP_BLE_NO_WRITE_CCC;
    }
#endif // 0
    ret = ble_op_att_send_data(handle, data, len, handle_type);
    if (ret == BLE_BUFFER_FULL) {
        ret = APP_BLE_BUFF_FULL;
    }

    if (ret) {
        log_info("app_send_fail:%d !!!!!!\n", ret);
    }
    else
    {
        log_info("app_send_success:%d !!!!!!\n", ret);
        //log_info_hexdump(data, len);
    }
    wdt_clear();

    return ret;
}

//------------------------------------------------------
#if RCSP_BTMATE_EN
static u8 tag_in_adv;
#endif

static u8 external_adv_data_len = 0;
static u8 external_adv_data[ADV_RSP_PACKET_MAX];

//外部配置广播数据，保存一下
bool external_set_adv_data(u8 *data, u8 len)
{
    if(len > ADV_RSP_PACKET_MAX){
        return false;
    }
    memcpy(external_adv_data, data, len);
    external_adv_data_len = len;
    return true;
}

static int make_set_adv_data(void)
{
    u8 offset = 0;
    u8 *buf = adv_data;

    if(external_adv_data_len){
        memcpy(buf, external_adv_data, external_adv_data_len);
        offset = external_adv_data_len;
        goto __SET_ADV;
    }


#if DOUBLE_BT_SAME_MAC
    offset += make_eir_packet_val(&buf[offset], offset, HCI_EIR_DATATYPE_FLAGS, 0x0A, 1);
#else
    offset += make_eir_packet_val(&buf[offset], offset, HCI_EIR_DATATYPE_FLAGS, 0x06, 1);
#endif

    offset += make_eir_packet_val(&buf[offset], offset, HCI_EIR_DATATYPE_COMPLETE_16BIT_SERVICE_UUIDS, 0xAF30, 2);

#if RCSP_BTMATE_EN
    u8  tag_len = sizeof(user_tag_string);
    if (tag_len > ADV_RSP_PACKET_MAX - (offset + 2)) {
        tag_in_adv = 0;
    } else {
        offset += make_eir_packet_data(&buf[offset], offset, HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA, (void *)user_tag_string, tag_len);
        tag_in_adv = 1;
    }
#endif

    if (offset > ADV_RSP_PACKET_MAX) {
        puts("***adv_data overflow!!!!!!\n");
        return -1;
    }
    log_info("adv_data(%d):", offset);
    log_info_hexdump(buf, offset);
    adv_data_len = offset;
    ble_op_set_adv_data(offset, buf);
    return 0;

__SET_ADV:
    if (offset > ADV_RSP_PACKET_MAX) {
        puts("***adv_data overflow!!!!!!\n");
        return -1;
    }
    log_info("adv_data(%d):", offset);
    log_info_hexdump(buf, offset);
    adv_data_len = offset;
    ble_op_set_adv_data(offset, buf);
    return 0;
}

static u8 external_rsp_data_len = 0;
static u8 external_rsp_data[ADV_RSP_PACKET_MAX];

//外部配置广播数据，保存一下
bool external_set_rsp_data(u8 *data, u8 len)
{
    if(len > ADV_RSP_PACKET_MAX){
        return false;
    }
    memcpy(external_rsp_data, data, len);
    external_rsp_data_len = len;
    return true;
}

static int make_set_rsp_data(void)
{
    u8 offset = 0;
    u8 *buf = scan_rsp_data;

    if(external_rsp_data_len){
        memcpy(buf, external_rsp_data, external_rsp_data_len);
        offset = external_rsp_data_len;
        goto __SET_RSP;
    }

#if RCSP_BTMATE_EN
    if (!tag_in_adv) {
        u8  tag_len = sizeof(user_tag_string);
        offset += make_eir_packet_data(&buf[offset], offset, HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA, (void *)user_tag_string, tag_len);
    }
#endif

    u8 name_len = gap_device_name_len;
    u8 vaild_len = ADV_RSP_PACKET_MAX - (offset + 2);
    if (name_len > vaild_len) {
        name_len = vaild_len;
    }
    offset += make_eir_packet_data(&buf[offset], offset, HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME, (void *)gap_device_name, name_len);

    if (offset > ADV_RSP_PACKET_MAX) {
        puts("***rsp_data overflow!!!!!!\n");
        return -1;
    }

    log_info("rsp_data(%d):", offset);
    log_info_hexdump(buf, offset);
    scan_rsp_data_len = offset;
    ble_op_set_rsp_data(offset, buf);
    return 0;

__SET_RSP:
    if (offset > ADV_RSP_PACKET_MAX) {
        puts("***rsp_data overflow!!!!!!\n");
        return -1;
    }

    log_info("rsp_data(%d):", offset);
    log_info_hexdump(buf, offset);
    scan_rsp_data_len = offset;
    ble_op_set_rsp_data(offset, buf);

    return 0;
}


typedef struct {
    u32 adv_interval_min;
    u8 adv_type;
    u8 adv_channel;
    u8 en;
}e_ble_adv_param_t;

static e_ble_adv_param_t external_adv_param = {
    .adv_interval_min = 0,
    .adv_channel = 0,
    .adv_type = 0,
    .en = 0,

};

void external_set_adv_param(u32 interval,u8 type,u8 channel)
{
    external_adv_param.adv_interval_min = interval;
    external_adv_param.adv_type = type;
    external_adv_param.adv_channel = channel;
}

//广播参数设置
static void advertisements_setup_init()
{
    uint8_t adv_type = ADV_IND;
    uint8_t adv_channel = ADV_CHANNEL_ALL;
    int   ret = 0;

    if(!external_adv_param.en){
        ble_op_set_adv_param(ADV_INTERVAL_MIN, adv_type, adv_channel);
    }else{
        ble_op_set_adv_param(external_adv_param.adv_interval_min, external_adv_param.adv_type, external_adv_param.adv_channel);
    }


    ret |= make_set_adv_data();
    ret |= make_set_rsp_data();

    if (ret) {
        puts("advertisements_setup_init fail !!!!!!\n");
        return;
    }

}

#define PASSKEY_ENTER_ENABLE      0 //输入passkey使能，可修改passkey
//重设passkey回调函数，在这里可以重新设置passkey
//passkey为6个数字组成，十万位、万位。。。。个位 各表示一个数字 高位不够为0
static void reset_passkey_cb(u32 *key)
{
#if 1
    u32 newkey = rand32();//获取随机数

    newkey &= 0xfffff;
    if (newkey > 999999) {
        newkey = newkey - 999999; //不能大于999999
    }
    *key = newkey; //小于或等于六位数
    printf("set new_key= %06u\n", *key);
#else
    *key = 123456; //for debug
#endif
}

void ble_sm_setup_init(io_capability_t io_type, u8 auth_req, uint8_t min_key_size, u8 security_en)
{
    //setup SM: Display only
    sm_init();
    sm_set_io_capabilities(io_type);
    sm_set_authentication_requirements(auth_req);
    sm_set_encryption_key_size_range(min_key_size, 16);
    sm_set_request_security(security_en);
    sm_event_callback_set(&cbk_sm_packet_handler);

    if (io_type == IO_CAPABILITY_DISPLAY_ONLY) {
        reset_PK_cb_register(reset_passkey_cb);
    }
}


void ble_profile_init(void)
{

#if 1
    printf("ble_profile init.\n");
    le_device_db_init();

#if PASSKEY_ENTER_ENABLE
    ble_sm_setup_init(IO_CAPABILITY_DISPLAY_ONLY, SM_AUTHREQ_MITM_PROTECTION, 7, TCFG_BLE_SECURITY_EN);
#else
    ble_sm_setup_init(IO_CAPABILITY_NO_INPUT_NO_OUTPUT, SM_AUTHREQ_BONDING, 7, TCFG_BLE_SECURITY_EN);
#endif

    /* setup ATT server */
    att_server_init(p_save_profile, att_read_callback, att_write_callback);
    att_server_register_packet_handler(cbk_packet_handler);
    /* gatt_client_register_packet_handler(packet_cbk); */

    // register for HCI events
    hci_event_callback_set(&cbk_packet_handler);
    /* ble_l2cap_register_packet_handler(packet_cbk); */
    /* sm_event_packet_handler_register(packet_cbk); */
    le_l2cap_register_packet_handler(&cbk_packet_handler);

#if TRANS_ANCS_EN
    //setup GATT client
    gatt_client_init();

    //setup ANCS clent
    ancs_client_init();
    ancs_set_notification_buffer(ancs_info_buffer, sizeof(ancs_info_buffer));
    ancs_client_register_callback(&cbk_packet_handler);
#endif

    ble_vendor_set_default_att_mtu(ATT_LOCAL_MTU_SIZE);
#endif
}


#if EXT_ADV_MODE_EN


#define EXT_ADV_NAME                    'J', 'L', '_', 'E', 'X', 'T', '_', 'A', 'D', 'V'
/* #define EXT_ADV_NAME                    "JL_EXT_ADV" */
#define BYTE_LEN(x...)                  sizeof((u8 []) {x})
#define EXT_ADV_DATA                    \
    0x02, 0x01, 0x06, \
    0x03, 0x02, 0xF0, 0xFF, \
    BYTE_LEN(EXT_ADV_NAME) + 1, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, EXT_ADV_NAME

struct ext_advertising_param {
    u8 Advertising_Handle;
    u16 Advertising_Event_Properties;
    u8 Primary_Advertising_Interval_Min[3];
    u8 Primary_Advertising_Interval_Max[3];
    u8 Primary_Advertising_Channel_Map;
    u8 Own_Address_Type;
    u8 Peer_Address_Type;
    u8 Peer_Address[6];
    u8 Advertising_Filter_Policy;
    u8 Advertising_Tx_Power;
    u8 Primary_Advertising_PHY;
    u8 Secondary_Advertising_Max_Skip;
    u8 Secondary_Advertising_PHY;
    u8 Advertising_SID;
    u8 Scan_Request_Notification_Enable;
} _GNU_PACKED_;

struct ext_advertising_data  {
    u8 Advertising_Handle;
    u8 Operation;
    u8 Fragment_Preference;
    u8 Advertising_Data_Length;
    u8 Advertising_Data[BYTE_LEN(EXT_ADV_DATA)];
} _GNU_PACKED_;

struct ext_advertising_enable {
    u8  Enable;
    u8  Number_of_Sets;
    u8  Advertising_Handle;
    u16 Duration;
    u8  Max_Extended_Advertising_Events;
} _GNU_PACKED_;

const struct ext_advertising_param ext_adv_param = {
    .Advertising_Handle = 0,
    .Advertising_Event_Properties = 1,
    .Primary_Advertising_Interval_Min = {30, 0, 0},
    .Primary_Advertising_Interval_Max = {30, 0, 0},
    .Primary_Advertising_Channel_Map = 7,
    .Primary_Advertising_PHY = ADV_SET_1M_PHY,
    .Secondary_Advertising_PHY = ADV_SET_1M_PHY,
};

const struct ext_advertising_data ext_adv_data = {
    .Advertising_Handle = 0,
    .Operation = 3,
    .Fragment_Preference = 0,
    .Advertising_Data_Length = BYTE_LEN(EXT_ADV_DATA),
    .Advertising_Data = EXT_ADV_DATA,
};

const struct ext_advertising_enable ext_adv_enable = {
    .Enable = 1,
    .Number_of_Sets = 1,
    .Advertising_Handle = 0,
    .Duration = 0,
    .Max_Extended_Advertising_Events = 0,
};

const struct ext_advertising_enable ext_adv_disable = {
    .Enable = 0,
    .Number_of_Sets = 1,
    .Advertising_Handle = 0,
    .Duration = 0,
    .Max_Extended_Advertising_Events = 0,
};

#endif /* EXT_ADV_MODE_EN */

static int set_adv_enable(void *priv, u32 en)
{
	if(app_var.adv_close == 1){
		log_info("stop adv ");
		return 0xff;
	}
    ble_state_e next_state, cur_state;

    if (!adv_ctrl_en) {

        log_info("adv_ctrl_en is %d, exit\n", adv_ctrl_en);
        return APP_BLE_OPERATION_ERROR;
    }

    if (con_handle) {

        log_info("con_handle is not null, exit\n");
        return APP_BLE_OPERATION_ERROR;
    }

    if (en) {
        next_state = BLE_ST_ADV;
    } else {
        next_state = BLE_ST_IDLE;
    }

    cur_state =  get_ble_work_state();
    switch (cur_state) {
    case BLE_ST_ADV:
    case BLE_ST_IDLE:
    case BLE_ST_INIT_OK:
    case BLE_ST_NULL:
    case BLE_ST_DISCONN:
        break;
    default:
        return APP_BLE_OPERATION_ERROR;
        break;
    }

    if (cur_state == next_state) {
        return APP_BLE_NO_ERROR;
    }
    log_info("adv_en:%d\n", en);
    set_ble_work_state(next_state);

#if EXT_ADV_MODE_EN
    if (en) {
        ble_op_set_ext_adv_param(&ext_adv_param, sizeof(ext_adv_param));

        log_info_hexdump(&ext_adv_data, sizeof(ext_adv_data));
        ble_op_set_ext_adv_data(&ext_adv_data, sizeof(ext_adv_data));

        ble_op_set_ext_adv_enable(&ext_adv_enable, sizeof(ext_adv_enable));
    } else {
        ble_op_set_ext_adv_enable(&ext_adv_disable, sizeof(ext_adv_disable));
    }
#else
    if (en) {
        advertisements_setup_init();
    }
    ble_op_adv_enable(en);
#endif /* EXT_ADV_MODE_EN */

    return APP_BLE_NO_ERROR;
}

static int ble_disconnect(void *priv)
{
    if (con_handle) {
        if (BLE_ST_SEND_DISCONN != get_ble_work_state()) {
            log_info(">>>ble send disconnect\n");
            set_ble_work_state(BLE_ST_SEND_DISCONN);
            ble_op_disconnect(con_handle);
        } else {
            log_info(">>>ble wait disconnect...\n");
        }
        return APP_BLE_NO_ERROR;
    } else {
        return APP_BLE_OPERATION_ERROR;
    }
}


static int get_buffer_vaild_len(void *priv)
{
    u32 vaild_len = 0;
    ble_op_att_get_remain(&vaild_len);
    return vaild_len;
}

static int app_send_user_data_do(void *priv, u8 *data, u16 len)
{
#if PRINT_DMA_DATA_EN
    if (len < 128) {
        log_info("-le_tx(%d):");
        log_info_hexdump(data, len);
    } else {
        putchar('L');
    }
#endif
    return 0;//app_send_user_data(ATT_CHARACTERISTIC_ae02_01_VALUE_HANDLE, data, len, ATT_OP_AUTO_READ_CCC);
}

void ble_send_data_to_app_notify(void)
{
    u8 data[] = {1,2,3,4};
    r_printf("ATT_OP_NOTIFY\n");

    app_send_user_data(0x0d, data, sizeof(data), ATT_OP_NOTIFY);
}

void ble_send_data_to_app_read(void)
{
    u8 data[] = {5,6,7,8};
    r_printf("ATT_OP_AUTO_READ_CCC\n");

    app_send_user_data(0x0d, data, sizeof(data), ATT_OP_AUTO_READ_CCC);
}

void ble_send_data_to_app_indicate(void)
{
    u8 data[] = {9,10,11,12};
    r_printf("ATT_OP_INDICATE\n");
    //changed by yuanlf 2021/11/22
    //app_send_user_data(0x0d, data, sizeof(data), ATT_OP_INDICATE);
    app_send_user_data(47, data, sizeof(data), ATT_OP_INDICATE);
}

static int app_send_user_data_check(u16 len)
{
    u32 buf_space = get_buffer_vaild_len(0);
    if (len <= buf_space) {
        return 1;
    }
    return 0;
}



static int regiest_wakeup_send(void *priv, void *cbk)
{
    ble_resume_send_wakeup = cbk;
    return APP_BLE_NO_ERROR;
}

static int regiest_recieve_cbk(void *priv, void *cbk)
{
    channel_priv = (u32)priv;
    app_recieve_callback = cbk;
    return APP_BLE_NO_ERROR;
}

static int regiest_state_cbk(void *priv, void *cbk)
{
    channel_priv = (u32)priv;
    app_ble_state_callback = cbk;
    return APP_BLE_NO_ERROR;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


u8 *ble_get_scan_rsp_ptr(u16 *len)
{
    if (len) {
        *len = scan_rsp_data_len;
    }
    return scan_rsp_data;
}

u8 *ble_get_adv_data_ptr(u16 *len)
{
    if (len) {
        *len = adv_data_len;
    }
    return adv_data;
}

u8 *ble_get_gatt_profile_data(u16 *len)
{
    *len = get_uplus_profile();
    return (u8 *)p_save_profile;
}


void bt_ble_adv_enable(u8 enable)
{
    log_info("set_adv_enable, %d.\n", enable);
    set_adv_enable(0, enable);
}

u16 bt_ble_is_connected(void)
{
    return con_handle;
}

void ble_module_enable(u8 en)
{
    log_info("mode_en:%d\n", en);
    if (en) {
        adv_ctrl_en = 1;
        bt_ble_adv_enable(1);
    } else {
        if (con_handle) {
            adv_ctrl_en = 0;
            ble_disconnect(NULL);
        } else {
            bt_ble_adv_enable(0);
            adv_ctrl_en = 0;
        }
    }
}


//流控使能 EN: 1-停止收数 or 0-继续收数
int ble_trans_flow_enable(u8 en)
{
    int ret = -1;
#if ATT_DATA_RECIEVT_FLOW
    if (con_handle) {
        att_server_flow_hold(con_handle, en);
        ret = 0;
    }
#endif
    log_info("ble_trans_flow_enable:%d,%d\n", en, ret);
    return ret;
}

//for test
static void timer_trans_flow_test(void)
{
    static u8 sw = 0;
    if (con_handle) {
        sw = !sw;
        ble_trans_flow_enable(sw);
    }
}

static const char ble_ext_name[] = "(BLE)";

void bt_ble_init(void)
{
    log_info("***** ble_init******\n");
    char *name_p;

#if DOUBLE_BT_SAME_NAME
    u8 ext_name_len = 0;
#else
    u8 ext_name_len = sizeof(ble_ext_name) - 1;
#endif

    name_p = bt_get_local_name();
    gap_device_name_len = strlen(name_p);
    if (gap_device_name_len > BT_NAME_LEN_MAX - ext_name_len) {
        gap_device_name_len = BT_NAME_LEN_MAX - ext_name_len;
    }

    memcpy(gap_device_name, name_p, gap_device_name_len);

#if DOUBLE_BT_SAME_NAME == 0
    //增加后缀，区分名字
    memcpy(&gap_device_name[gap_device_name_len], "(BLE)", ext_name_len);
    gap_device_name_len += ext_name_len;
#endif

    log_info("ble name(%d): %s \n", gap_device_name_len, gap_device_name);

#if ATT_DATA_RECIEVT_FLOW
    log_info("att_server_flow_enable\n");
    att_server_flow_enable(1);
    /* sys_timer_add(0, timer_trans_flow_test, 3000); */
#endif

    set_ble_work_state(BLE_ST_INIT_OK);
    //ble_module_enable(1);

#if TEST_SEND_DATA_RATE
    server_timer_start();
#endif
}

void bt_ble_exit(void)
{
    log_info("***** ble_exit******\n");

    ble_module_enable(0);

#if TEST_SEND_DATA_RATE
    server_timer_stop();
#endif

}


void ble_app_disconnect(void)
{
    ble_disconnect(NULL);
}

#if RCSP_BTMATE_EN
static int rcsp_send_user_data_do(void *priv, u8 *data, u16 len)
{
    log_info("rcsp_tx:%x\n", len);
#if PRINT_DMA_DATA_EN
    if (len < 128) {
        log_info("-dma_tx(%d):");
        log_info_hexdump(data, len);
    } else {
        putchar('L');
    }
#endif
    return app_send_user_data(ATT_CHARACTERISTIC_ae02_02_VALUE_HANDLE, data, len, ATT_OP_AUTO_READ_CCC);
}
#endif

#if RCSP_BTMATE_EN
static const struct ble_server_operation_t mi_ble_operation = {
    .adv_enable = set_adv_enable,
    .disconnect = ble_disconnect,
    .get_buffer_vaild = get_buffer_vaild_len,
    .send_data = (void *)rcsp_send_user_data_do,
    .regist_wakeup_send = regiest_wakeup_send,
    .regist_recieve_cbk = regiest_recieve_cbk,
    .regist_state_cbk = regiest_state_cbk,
};
#else
static const struct ble_server_operation_t mi_ble_operation = {
    .adv_enable = set_adv_enable,
    .disconnect = ble_disconnect,
    .get_buffer_vaild = get_buffer_vaild_len,
    .send_data = (void *)app_send_user_data_do,
    .regist_wakeup_send = regiest_wakeup_send,
    .regist_recieve_cbk = regiest_recieve_cbk,
    .regist_state_cbk = regiest_state_cbk,
};
#endif

void ble_get_server_operation_table(struct ble_server_operation_t **interface_pt)
{
    *interface_pt = (void *)&mi_ble_operation;
}

void ble_server_send_test_key_num(u8 key_num)
{
    ;
}

uint16_t gatts_get_con_handle(void)
{
    return con_handle;
}

uint16_t gatts_get_mtu(void)
{
    return save_mtu;
}

#endif


