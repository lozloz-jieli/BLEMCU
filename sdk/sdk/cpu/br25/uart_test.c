/******************************Product test******************************/
#include "system/includes.h"
#include "app_config.h"
#include "asm/uart_dev.h"
#include "user_cfg.h"

#include "include/hal/uplus_hal_uart.h"
#include "include/user_config.h"

#if 1

#define UART_DEV_USAGE_TEST_SEL         1       //uart_dev.c api接口使用方法选择
//  选择1  串口中断回调函数推送事件，由事件响应函数接收串口数据
//  选择2  由task接收串口数据

#define UART_DEV_TEST_MULTI_BYTE        1       //uart_dev.c 读写多个字节api / 读写1个字节api 选择

static u8 uart_cbuf[512] __attribute__((aligned(4)));
static u8 uart_rxbuf[512] __attribute__((aligned(4)));
static u8 uart_txbuf[512] __attribute__((aligned(4)));
static u8 data_temp[32];
const uart_bus_t *uart_bus = NULL;
extern void swapX(const uint8_t *src, uint8_t *dst, int len);
extern void set_uartid_table(uint32_t uart_bus,u8 uartid);

static u8 *get_data_complement_code(u8 *data, u8 len)
{
    int i = 0;
    if(len > sizeof(data_temp))
    {
        return NULL;
    }

    for(i = 0; i < len; i++)
    {
        data_temp[i] = 0xff - data[i];
    }

    return data_temp;
}

static u16 uart_tx_buff_input(u8 *data, u16 offset, u16 len)
{
    if((offset + len) > sizeof(uart_txbuf))
    {
        return 0;
    }

    if(offset == 0)
    {
        memset(uart_txbuf, 0x00, sizeof(uart_txbuf));
    }

    memcpy(uart_txbuf + offset, data, len);

    return len;
}

static u16 uart_tx_data_input(u8 data, u16 offset, u16 len)
{
    if((offset + len) > sizeof(uart_txbuf))
    {
        return 0;
    }

    if(offset == 0)
    {
        memset(uart_txbuf, 0x00, sizeof(uart_txbuf));
    }

    uart_txbuf[offset] = data;

    return len;
}
extern uplus_uart_handler_t uart_receive_data;
 void uart_receive_data_callback_reg(u8 (*cb)(u8 *data, u16 len))
 {
     uart_receive_data = cb;
 }


#define SYS_UART_EVENT_TYPE_OVERFLOW (('O' << 24) | ('V' << 16) | ('E' << 8) | '\0')
#define SYS_UART_EVENT_TYPE_OUTTIME  (('O' << 24) | ('U' << 16) | ('T' << 8) | '\0')

//设备事件响应demo
static void uart_event_handler(struct sys_event *e)
{
    const uart_bus_t *uart_bus;
    u32 uart_rxcnt = 0;
    uplus_uart_data_t uplus_uart_data;

    if (e == NULL || e->arg == NULL){
        return;
    }

    if (e->arg == SYS_UART_EVENT_TYPE_OVERFLOW) {
        if (e->u.dev.event == DEVICE_EVENT_CHANGE) {
            printf("uart event: SYS_UART_EVENT_TYPE_OVERFLOW\n");
            uart_bus = (const uart_bus_t *)e->u.dev.value;
            uart_rxcnt = uart_bus->read(uart_rxbuf, sizeof(uart_rxbuf), 0);
            if (uart_rxcnt) {
                printf("get_buffer:\n");
                if (uart_rxcnt % 16) {
                    putchar('\n');
                }
                put_buf(uart_rxbuf, uart_rxcnt);
                

                // uart_bus->write(uart_rxbuf, uart_rxcnt);
                if(uart_receive_data){
                    uplus_uart_data.uart_id = 0;
                    uplus_uart_data.data_buff = uart_rxbuf;
                    uplus_uart_data.data_len = uart_rxcnt;
                    uart_receive_data(&uplus_uart_data);
                }
                uplus_uart_tx(&uplus_uart_data);
            }
            printf("uart out\n");
        }
    }
    if (e->arg == SYS_UART_EVENT_TYPE_OUTTIME) {
        if (e->u.dev.event == DEVICE_EVENT_CHANGE) {
            printf("uart event: SYS_UART_EVENT_TYPE_OUTTIME\n");
            uart_bus = (const uart_bus_t *)e->u.dev.value;
            uart_rxcnt = uart_bus->read(uart_rxbuf, sizeof(uart_rxbuf), 0);
            if (uart_rxcnt) {
                printf("get_buffer:\n");
                if (uart_rxcnt % 16) {
                    putchar('\n');
                }
                // uart_bus->write(uart_rxbuf, uart_rxcnt);
                put_buf(uart_rxbuf, uart_rxcnt);
                if(uart_receive_data){
                    uplus_uart_data.uart_id = 0;
                    uplus_uart_data.data_buff = uart_rxbuf;
                    uplus_uart_data.data_len = uart_rxcnt;
                    uart_receive_data(&uplus_uart_data);
                }
                uplus_uart_tx(&uplus_uart_data);
            }
            printf("uart out\n");
        }
    }
}

SYS_EVENT_HANDLER(SYS_DEVICE_EVENT, uart_event_handler, 0);

static void uart_u_task(void *arg)
{
    const uart_bus_t *uart_bus = arg;
    int ret;
    u32 uart_rxcnt = 0;

    printf("uart_u_task start\n");
    while (1) {
#if !UART_DEV_TEST_MULTI_BYTE
        //uart_bus->getbyte()在尚未收到串口数据时会pend信号量，挂起task，直到UART_RX_PND或UART_RX_OT_PND中断发生，post信号量，唤醒task
        ret = uart_bus->getbyte(&uart_rxbuf[0], 0);
        if (ret) {
            uart_rxcnt = 1;
            printf("get_byte: %02x\n", uart_rxbuf[0]);
            uart_bus->putbyte(uart_rxbuf[0]);
        }
#else
        //uart_bus->read()在尚未收到串口数据时会pend信号量，挂起task，直到UART_RX_PND或UART_RX_OT_PND中断发生，post信号量，唤醒task
        uart_rxcnt = uart_bus->read(uart_rxbuf, sizeof(uart_rxbuf), 0);
        if (uart_rxcnt) {
            printf("get_buffer:\n");
            if (uart_rxcnt % 16) {
                putchar('\n');
            }
            uart_bus->write(uart_rxbuf, uart_rxcnt);
        }
#endif
    }
}

static void uart_isr_hook(void *arg, u32 status)
{
    const uart_bus_t *ubus = arg;
    struct sys_event e;

    //当CONFIG_UARTx_ENABLE_TX_DMA（x = 0, 1）为1时，不要在中断里面调用ubus->write()，因为中断不能pend信号量
    if (status == UT_RX) {
        printf("uart_rx_isr\n");
#if (UART_DEV_USAGE_TEST_SEL == 1)
        e.type = SYS_DEVICE_EVENT;
        e.arg = (void *)SYS_UART_EVENT_TYPE_OVERFLOW;
        e.u.dev.event = DEVICE_EVENT_CHANGE;
        e.u.dev.value = (int)ubus;
        sys_event_notify(&e);
#endif
    }
    if (status == UT_RX_OT) {
        printf("uart_rx_ot_isr\n");
#if (UART_DEV_USAGE_TEST_SEL == 1)
        e.type = SYS_DEVICE_EVENT;
        e.arg = (void *)SYS_UART_EVENT_TYPE_OUTTIME;
        e.u.dev.event = DEVICE_EVENT_CHANGE;
        e.u.dev.value = (int)ubus;
        sys_event_notify(&e);
#endif
    }
}


void uart_no_rx_income_timeout(void *p)
{
    if(uart_bus == NULL ){
        return;
    }
    printf("uart_no_rx_income_timeout, close\n");
#if (UART_DEV_USAGE_TEST_SEL == 2)
    os_task_del("uart_u_task");
#endif
    uart_dev_close(uart_bus);
    gpio_direction_input(RX_PORT);
    gpio_set_pull_down(RX_PORT, 0);
    gpio_set_pull_up(RX_PORT, 1);
    gpio_set_die(RX_PORT, 1);

    gpio_direction_input(TX_PORT);
    gpio_set_pull_down(TX_PORT, 0);
    gpio_set_pull_up(TX_PORT, 1);
    gpio_set_die(TX_PORT, 1);
    uart_bus = NULL;
}

static int uart_timeout_handler = 0;
void uart_timeout_init(u16 ms)
{
    uart_timeout_handler = sys_timeout_add(NULL, uart_no_rx_income_timeout, ms);
}

void uart_timeout_del(void)
{
    if(uart_timeout_handler)
    {
        sys_timeout_del(uart_timeout_handler);
        uart_timeout_handler = 0;
    }
}

struct uart_platform_data_t u_arg = {0};

bool uart_dev_test_init(u8 tx_pin, u8 rx_pin, u32 baud)
{
    u_arg.tx_pin = tx_pin;
    u_arg.rx_pin = rx_pin;
    u_arg.rx_cbuf = uart_cbuf;
    u_arg.rx_cbuf_size = 512;
    u_arg.frame_length = 32;
    u_arg.rx_timeout = 100;
    u_arg.isr_cbfun = uart_isr_hook;
    u_arg.baud = baud;
    u_arg.is_9bit = 0;
    uart_bus = uart_dev_open(&u_arg);
    if (uart_bus != NULL) {
// #ifndef PRODUCT_TEST_CONTROL_IO
//        uart_timeout_init(10000);//10秒内未收到合法的串口数据就关闭串口
// #endif
        printf("uart_dev_open() success\n");
#if (UART_DEV_USAGE_TEST_SEL == 2)
        os_task_create(uart_u_task, (void *)uart_bus, 31, 512, 0, "uart_u_task");
#endif
        return true;
    }
    else{
        printf("uart_dev_open() failed\n");
    }

    return false;
}

static u8 uart_idle_query(void)
{
    return uart_bus == NULL ? 1 : 0;
}

REGISTER_LP_TARGET(uart_lp_target) = {
    .name = "uar",
    .is_idle = uart_idle_query,
};

static void uart_clock_critical_enter(void)
{

}

static void uart_clock_critical_exit(void)
{
    if(uart_bus == NULL){
        return;
    }

    uart_bus->set_baud(u_arg.baud);
}

CLOCK_CRITICAL_HANDLE_REG(user_uart, uart_clock_critical_enter, uart_clock_critical_exit)


bool uart_dev_test_init_api(u32 baud,u8 uartid)
{
    bool ret = uart_dev_test_init(TX_PORT, RX_PORT, baud);
    if(ret){
        set_uartid_table(*(uint32_t *)uart_bus,uartid);
    }
    return ret;
}

u16 uart_dev_test_send_api(void *data, u16 len)
{
    if(uart_bus == NULL){
        return 0;
    }
    uart_bus->write(data, len);
    return len;
}
u16 uart_dev_busy_api(u8 uartid)
{
    return uart_dev_busy(uart_bus);
}

void uart_dev_tx_enable_api(u8 uartid)
{
    uart_dev_tx_enable(uart_bus);
}

void uart_dev_tx_disable_api(u8 uartid)
{
    uart_dev_tx_disable(uart_bus);
}



#endif
/******************************Product test******************************/
