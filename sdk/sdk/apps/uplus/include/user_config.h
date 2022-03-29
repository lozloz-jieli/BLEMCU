/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-28 16:24:26
 * @LastEditTime: 2021-08-30 17:36:39
 * @LastEditors: jianye
 */
#ifndef __USER_CONFIG__
#define __USER_CONFIG__

#define TEST_SERVICE_NUM_MAX    4
#define TEST_CHAR_NUM_MAX       6

//uplus 私有串口
#define TX_PORT  IO_PORTB_04//IO_PORT_DP
#define RX_PORT  IO_PORTB_05//IO_PORT_DM


#define USER_OVERALL_MODE_NUMBER		("JV001W003")
#define USER_OVERALL_VERSION			("1.4.00")
#define USER_SOFTWARE_TYPE         		("BLE_SDK")
#define USER_DEV_NAME_U_				("BLDPR")
#define USER_DEV_NAME					("U-BLDPR")
#define USER_SOFTWARE_VER				("1.4.00")
#define USER_HARDWARE_VER				("1.0.00")
#define USER_PRODUCT_CODE				("JV001W003")
#define USER_TYPE_ID					("200c10841050850815016359c4ef7c000000a014a33befbc0bd12123e189bd40")
#define USER_HARD_TYPE					("RTL_8762")

#define OK_INDICATE_ACK 0
#define ERR_INDICATE_ACK 1

typedef int (*uplus_indicate_ack_handler)(const int32_t status);
typedef struct {
	int					cfg_state;
	unsigned char 		connect_state;
	unsigned int 		ts;
	unsigned int        uplus_indicate_send_data_num;
    unsigned int        uplus_indicate_send_data_base_time;
	unsigned int 	    rpt_timer_id;
	uplus_indicate_ack_handler indicate_ack_cb;
    unsigned int        indicate_ack_state;//0: success, 1: error
} uplus_user_ctrl_t;

#endif /* __USER_CONFIG__ */
