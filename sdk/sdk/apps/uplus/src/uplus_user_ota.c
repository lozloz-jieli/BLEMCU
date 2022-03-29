
#include "include/includes.h"

#define LOG_TAG_CONST       UPlus  //保持与文件名同步
#define LOG_TAG             "[UPlus]"
#define LOG_ERROR_ENABLE
#define LOG_DEBUG_ENABLE
#define LOG_INFO_ENABLE
/* #define LOG_DUMP_ENABLE */
#define LOG_CLI_ENABLE
#include "debug.h"

#define USER_APP_TESET_MAX_SUPPORT_VERSION_NUM      (2)
#define UPLUS_USER_OTA_TIMER_INTERVAL               (60 * 1000)
#define UPLUS_USER_OTA_TIMER_LOOP_CNT               (10)

enum {
    USER_APP_OTA_STATE_IDLE,
    USER_APP_OTA_STATE_PREPARE,
    USER_APP_OTA_STATE_PROCESSING,
    USER_APP_OTA_STATE_CHECK_UPGRADE,
    USER_APP_OTA_STATE_FINISHED
};

typedef struct {
    uplus_s32     fm_upgrade; // 0: not upgrade; 1: upgraded
    uplus_s32     fm_offset;
} uplus_user_ver_manage_t;

typedef struct {
    uplus_u8                                ota_state;
    uplus_u32                               session;
    uplus_ble_sdk_ota_pkt_desc_t            *pkg_desc;
    uplus_ble_sdk_ota_sub_firmware_desc_t   *curr_sub_fm;
    uplus_s16                               curr_fm_index;
    uplus_user_ver_manage_t                 vers[USER_APP_TESET_MAX_SUPPORT_VERSION_NUM];
    uplus_timer_id_e						ota_timer_handle;
    uplus_u8                                ota_cur_loop_cnt;
} uplus_user_ota_ctrl_t;

static uplus_user_ota_ctrl_t g_user_ota_ctrl;

static void user_app_select_fw(void);
static void user_app_ota_do_prepare(const uplus_ble_sdk_ota_evt_t *evt);
static void user_app_ota_do_download_complete(const uplus_ble_sdk_ota_evt_t *evt);
static void user_app_ota_do_upgrading(const uplus_ble_sdk_ota_evt_t *evt);
static void user_app_ota_do_check(const uplus_ble_sdk_ota_evt_t *evt);

static void user_app_ota_do_prepare(const uplus_ble_sdk_ota_evt_t *evt)
{

    UPLUS_BLE_LOG_INFO("===user app ota prepare===\n");

    g_user_ota_ctrl.pkg_desc  = evt->evt.OTA_REQ_desc;
    g_user_ota_ctrl.session   = evt->session;
    g_user_ota_ctrl.ota_state = USER_APP_OTA_STATE_PREPARE;
    user_app_select_fw();
}

static void user_app_ota_do_upgrading(const uplus_ble_sdk_ota_evt_t *evt)
{
    //UPLUS_BLE_LOG_INFO("===user app ota upgrading===\n");
    // 模块重启后， 通知用户更新升级包数据信息。
    // TODO 用户可以考虑复制该信息, 这里测试，仅仅做引用。
    g_user_ota_ctrl.pkg_desc  = evt->evt.OTA_SDK_UPGRADING_info;
    g_user_ota_ctrl.session   = evt->session;
    g_user_ota_ctrl.ota_state = USER_APP_OTA_STATE_PROCESSING;
}

static void user_app_ota_do_download_complete(const uplus_ble_sdk_ota_evt_t *evt)
{
    //UPLUS_BLE_LOG_INFO("===user app ota download successfully===\n");;
    g_user_ota_ctrl.curr_sub_fm = evt->evt.OTA_DOWNLOAD_info;
    g_user_ota_ctrl.ota_state = USER_APP_OTA_STATE_PROCESSING;
   user_app_ota_do_check(evt);
}

static void user_app_ota_do_check(const uplus_ble_sdk_ota_evt_t *evt)
{
    //UPLUS_BLE_LOG_INFO("===user app ota fw check===\n");
    g_user_ota_ctrl.curr_sub_fm = evt->evt.OTA_FINISHED_info;
    g_user_ota_ctrl.ota_state = USER_APP_OTA_STATE_PROCESSING;

    /* TODO code to do check */
    // 测试代码，直接认为校验通过并成功。
    g_user_ota_ctrl.vers[g_user_ota_ctrl.curr_fm_index].fm_upgrade = 1;
    UPLUS_BLE_LOG_INFO("check num %d curIdx %d\n", g_user_ota_ctrl.pkg_desc->fm_desc.sub_firmware_num, g_user_ota_ctrl.curr_fm_index);

    if (g_user_ota_ctrl.pkg_desc->fm_desc.sub_firmware_num == (g_user_ota_ctrl.curr_fm_index + 1)) {
        g_user_ota_ctrl.ota_state = USER_APP_OTA_STATE_FINISHED;
        uplus_ble_sdk_ota_status_notify(UPLUS_BLE_SDK_OTA_STATUS_FINISH, UPLUS_BLE_SDK_OTA_ERRNUM_OK);
    } else {
        user_app_ota_do_prepare(evt);
    }
}

static int user_app_ota_evt_notify_cb(const uplus_ble_sdk_ota_evt_t *evt, void * userdata)
{
    switch (evt->type) {
        case UPLUS_BLE_SDK_OTA_EVT_REQ:
        user_app_ota_do_prepare(evt);
        break;

        case UPLUS_BLE_SDK_OTA_EVT_SDK_UPGRADING:
        user_app_ota_do_upgrading(evt);
        break;

        case UPLUS_BLE_SDK_OTA_EVT_DOWNLOAD_COMPLETE:
        user_app_ota_do_download_complete(evt);
        break;

        case UPLUS_BLE_SDK_OTA_EVT_FINISHED:
        user_app_ota_do_check(evt);
        break;

    default:
        break;
    }

    return true;
}

static void user_app_select_fw(void)
{
    uplus_ble_sdk_ota_sub_firmware_desc_t * sub_fm_desc = NULL;
    uplus_s32 fm_loop = 0, fm_find = 0;

    for (fm_loop = 0; fm_loop < g_user_ota_ctrl.pkg_desc->fm_desc.sub_firmware_num; fm_loop++) {
        sub_fm_desc = &g_user_ota_ctrl.pkg_desc->sub_fm_desc[fm_loop];
        UPLUS_BLE_LOG_INFO("ota select fw sub[%d] type=%d \n", fm_loop, sub_fm_desc->type);
        if (sub_fm_desc->type == UPLUS_OTA_TYPE_BLE_SDK) {
            fm_find = 1;
            break;
        }
    }

    if (fm_find) {
        if (strcmp(USER_SOFTWARE_VER, (const char *)sub_fm_desc->version)) {
            uplus_ble_sdk_ota_upgrade_firmware(g_user_ota_ctrl.session, UPLUS_BLE_SDK_OTA_MODE_SDK, sub_fm_desc);
            g_user_ota_ctrl.ota_cur_loop_cnt = 0;
            uplus_timer_start(g_user_ota_ctrl.ota_timer_handle);
        } else {
            uplus_ble_sdk_ota_status_notify(UPLUS_BLE_SDK_OTA_STATUS_FINISH, UPLUS_BLE_SDK_OTA_ERRNUM_OK);
        }
    } else {
        uplus_ble_sdk_ota_status_notify(UPLUS_BLE_SDK_OTA_STATUS_FINISH, UPLUS_BLE_SDK_OTA_ERRNUM_IMAGE_DATA_ERR);
    }
}

static void uplus_user_ota_timer_func(void *ctx)
{
    g_user_ota_ctrl.ota_cur_loop_cnt++;
    if (g_user_ota_ctrl.ota_cur_loop_cnt >= UPLUS_USER_OTA_TIMER_LOOP_CNT) {
        UPLUS_BLE_LOG_INFO("==ota timeout==\n");
        uplus_ble_sdk_ota_status_notify(UPLUS_BLE_SDK_OTA_STATUS_FINISH, UPLUS_BLE_SDK_OTA_ERRNUM_TIME_OUT);

        // ota add by yuanlf
        dual_bank_passive_update_exit(NULL);

        uplus_timer_stop(g_user_ota_ctrl.ota_timer_handle);
    }
}

void uplus_user_ota_init(void)
{
    memset(&g_user_ota_ctrl, 0, sizeof(g_user_ota_ctrl));
    g_user_ota_ctrl.ota_state = USER_APP_OTA_STATE_IDLE;

    uplus_ble_sdk_ota_evt_register(user_app_ota_evt_notify_cb, &g_user_ota_ctrl);
    g_user_ota_ctrl.ota_timer_handle = UPLUS_OTA_TIMER;
	uplus_timer_create(g_user_ota_ctrl.ota_timer_handle, UPLUS_TIMER_REPEATED, UPLUS_USER_OTA_TIMER_INTERVAL, uplus_user_ota_timer_func);
}
