/*
 * @Description:
 * @Author: jianye
 * @Date: 2021-08-11 11:09:12
 * @LastEditTime: 2021-08-16 14:36:25
 * @LastEditors: jianye
 */
/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

#ifndef __UPLUS_HAL_GPIO_H__
#define __UPLUS_HAL_GPIO_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	UPLUS_INPUT,             //input
	UPLUS_OUTPUT           //output
}uplus_pin_io_e;
typedef enum{
    UPLUS_GPIO_PULL_UP,
    UPLUS_GPIO_PULL_DOWN,
    UPLUS_GPIO_PULL_NO
} uplus_gpio_pull_mode_e;
/* End GPIO relevant */
typedef enum{
    UPLUS_GPIO_POLARITY_NONE,     //
    UPLUS_GPIO_POLARITY_LOTOHI,   //  Low to high.
    UPLUS_GPIO_POLARITY_HITOLO ,   //  High to low.
    UPLUS_GPIO_POLARITY_TOGGLE     //  Toggle.
} uplus_gpio_action_e;

typedef enum{
    UPLUS_BUTTON_MODE_INT,
    UPLUS_BUTTON_MODE_NOINT
} uplus_gpio_mode_e;
typedef struct{
	uint8_t      pin_port_id;
	uint32_t     pin_id;
}uplus_pin_t;
typedef struct{
	uplus_pin_t            pin;
	uplus_pin_io_e         pin_direction;
	uplus_gpio_pull_mode_e pin_pull_mode;
	uplus_gpio_action_e    pin_action;
	uplus_gpio_mode_e      pin_mode;
}uplus_gpio_t;

typedef void(*uplus_gpio_handler_t)(uplus_pin_t* p_pin, uplus_gpio_action_e pin_action);

extern void uplus_gpio_init(uplus_gpio_t* p_gpio_init_pram);
extern void uplus_gpio_deinit(uplus_pin_t* p_pin);
extern void uplus_gpio_register(uplus_gpio_handler_t gpio_handler);

extern void uplus_gpio_set_input(uplus_pin_t* p_pin, uplus_gpio_pull_mode_e pull_mode);
extern void uplus_gpio_set_output(uplus_pin_t* p_pin, uplus_gpio_pull_mode_e pull_mode);
extern void uplus_gpio_out_set(uplus_pin_t* p_pin, uint8_t value);
extern uint8_t uplus_gpio_in_get(uplus_pin_t* p_pin);

#ifdef __cplusplus
}
#endif

#endif

