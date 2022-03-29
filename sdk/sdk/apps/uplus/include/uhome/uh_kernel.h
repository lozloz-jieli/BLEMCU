/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

/**
 * @file uh_kernel.h
 * @brief 与操作系统相关的接口定义，包括任务、中断、信号量、队列，时钟、原子操作和电源管理等。
 *
 * @date 2017-04-14
 * @author fanming
 *
 */

#ifndef __UH_KERNEL_H__
#define __UH_KERNEL_H__

#include "uh_common.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @typedef uh_task_id_t
 * @brief 任务标识。
 */
typedef void * uh_task_id_t;
/**
 * @typedef uh_mutex_id_t
 * @brief 互斥信号量标识。
 */
typedef void * uh_mutex_id_t;
/**
 * @typedef uh_sem_id_t
 * @brief 同步信号量标识。
 */
typedef void * uh_sem_id_t;
/**
 * @typedef uh_queue_id_t
 * @brief 消息队列标识。
 */
typedef void * uh_queue_id_t;
/**
 * @typedef uh_timer_id_t
 * @brief 定时器标识。
 */
typedef void * uh_timer_id_t;

#if 0
/**
 * @typedef uh_task_id_t
 * @brief 任务标识。
 */
typedef void * uh_task_id_t;
/**
 * @typedef uh_mutex_id_t
 * @brief 互斥信号量标识。
 */
typedef void * uh_mutex_id_t;
/**
 * @typedef uh_sem_id_t
 * @brief 同步信号量标识。
 */
typedef void * uh_sem_id_t;
/**
 * @typedef uh_queue_id_t
 * @brief 消息队列标识。
 */
typedef void * uh_queue_id_t;
/**
 * @typedef uh_timer_id_t
 * @brief 定时器标识。
 */
typedef void * uh_timer_id_t;

/**  @def TASK_LOWEST_PRIORITY/TASK_HIGHEST_PRIORITY
 *
 *  @brief 任务优先级，数字越小，优先级越低。
 *
 */
 
/*task priortiy*/
#define IDLE_TASK_PRIORITY 0
#define TASK_LOWEST_PRIORITY 1
#define TASK_HIGHEST_PRIORITY 15

/** @def TASK_OPTION_XXX
 *
 * @brief 创建任务时的选项。
 *
 * @details XXX包括STACK_CAN_USE_NOT_DMA_MEMORY。
 */
#define TASK_OPTION_STACK_CAN_USE_NOT_DMA_MEMORY 0x1

/**
 * @typedef task_entry_t
 * @brief 任务入口。
 * @param task_param 任务入口参数。
 */
typedef void (*task_entry_t)(void *task_param);

/**
 * @brief 创建任务。
 *
 * @param task_name 任务名称。
 * @param stack_size 任务栈大小，单位字节。
 * @param priority 任务优先级，最低TASK_LOWEST_PRIORITY，最高TASK_HIGHEST_PRIORITY。
 * @param options TASK_OPTION_STACK_CAN_USE_NOT_DMA_MEMORY表示任务栈空间可以使用非DMA内存。
 * @param entry 任务入口。
 * @param task_id 返回的任务标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_task_create(
	const char *task_name,
	uint32_t stack_size,
	uint32_t priority,
	uint32_t options,
	task_entry_t entry,
	void *task_param,
	uh_task_id_t * task_id);

/**
 * @brief 删除任务。
 * @note 任务退出时必须调用uh_k_task_delete，否则会导致内存泄漏。
 *
 * @param task_id 任务标识，由uh_k_task_create创建或者NULL。NULL表示当前任务。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_task_delete(uh_task_id_t task_id);

/**
 * @brief 当前任务睡眠。
 *
 * @param duration 任务睡眠时间，单位毫秒。
 *
 * @return N/A。
 */
extern void uh_k_task_msleep(uint32_t duration);

/**
 * @brief 当前任务放弃CPU控制权。
 *
 * @return N/A。
 */
extern void uh_k_task_yield(void);

/**
 * @brief 任务挂起。
 *
 * @param task_id 任务标识，由uh_k_task_create创建或者NULL。NULL表示当前任务。
 *
 * @return N/A。
 */
extern int uh_k_task_suspend(uh_task_id_t task_id);

/**
 * @brief 恢复挂起的任务。
 *
 * @param task_id 任务标识，由uh_k_task_create创建。
 *
 * @return N/A。
 */
extern int uh_k_task_resume(uh_task_id_t task_id);

/**
 * @brief 获取任务的优先级。
 *
 * @param task_id 任务标识，由uh_k_task_create创建或者NULL。NULL表示当前任务。
 * @param priority 获取的任务优先级。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_task_priority_get(uh_task_id_t task_id, uint32_t *priority);

/**
 * @brief 修改任务的优先级。
 *
 * @param task_id 任务标识，由uh_k_task_create创建或者NULL。NULL表示当前任务。
 * @param priority 任务优先级，最低TASK_LOWEST_PRIORITY，最高TASK_HIGHEST_PRIORITY。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_task_priority_set(uh_task_id_t task_id, uint32_t priority);

/**
 * @brief 获取当前任务标识。
 *
 * @return 当前任务标识。
 */
extern uh_task_id_t uh_k_task_id_self(void);

/**
 * @brief 获取指定任务标识的任务名称。
 *
 * @param task_id 任务标识，由uh_k_task_create创建或者NULL。NULL表示当前任务。
 *
 * @return 任务名称。
 */
extern const char * uh_k_task_name(uh_task_id_t task_id);

/**
 * @brief 显示当前所有任务信息。
 * @details 可以显示的任务信息包括
 * 任务名称、任务标识、当前任务优先级、未使用的栈空间大小、
 * 任务编号和任务占用CPU时间及百分比 
 *
 * @return N/A
 */
extern void uh_k_task_show(void);

/**
 * @brief 设置当前任务的本地存储。
 *
 * @param var 本地存储指针。建议使用动态申请的内存。
 *
 * @return N/A。
 */
extern void uh_k_task_local_var_set(void * var);

/**
 * @brief 获取当前任务的本地存储。
 *
 * @return 如果没设置过任务的本地存储，返回NULL，否则返回本地存储指针。
 */
extern void * uh_k_task_local_var_get(void);

/**
 * @brief 获取当前任务保留的本地存储。
 * @note 仅限内部使用，不要直接调用此接口。
 *
 * @return 保留的本地存储指针。
 */
extern void * uh_k_task_local_sys_var_get(void);

/**
 * @brief 启动任务调度器。
 * @note 在启动任务调度器之前，创建的任务都不会自动运行。
 *
 * @return N/A。
 */
extern void uh_k_sched_start(void);

#define SCHEDULER_STATE_IDLE 0
#define SCHEDULER_STATE_RUNNING 1
#define SCHEDULER_STATE_SUSPENDED 2
/**
 * @brief 获取任务调度器的状态。
 *
 * @retval SCHEDULER_STATE_IDLE（0） 任务调度器未启动。
 * @retval SCHEDULER_STATE_RUNNING（1） 任务调度器运行中。
 * @retval SCHEDULER_STATE_SUSPENDED（2） 任务调度器挂起中。
 */
extern int uh_k_sched_state_get(void);

/**
 * @brief 任务锁。
 * @details 禁止任务调度。用于临界区保护。
 * @note 临界区的代码必须简短，防止占用时间过长，导致高优先级任务不能及时运行。
 * 不要在中断处理函数中调用。
 * 可以嵌套使用。
 * 不能锁中断。
 * 临界区不能调用其他系统函数，除了uh_k_task_lock。
 *
 * @return N/A。
 */
extern void uh_k_task_lock(void);

/**
 * @brief 任务解锁。
 * @details 恢复任务调度。用于临界区保护。
 * @note 必须与uh_k_task_lock成对使用。
 *
 * @return N/A。
 */
extern void uh_k_task_unlock(void);

/**
 * @brief 中断锁。
 * @details 禁止中断。用于临界区保护。
 * @note 临界区的代码必须简短，防止占用时间过长，中断不能及时响应造成系统崩溃。
 * 可以嵌套使用。
 * 临界区不能调用其他系统函数，除了uh_k_irq_lock。
 * 
 * @param from_isr 是否在中断上下文中调用。
 *
 * @return 系统相关的中断状态，用于uh_k_irq_unlock。
 */
extern uint32_t uh_k_irq_lock(int from_isr);

/**
 * @brief 中断解锁。
 * @details 恢复中断。用于临界区保护。
 * @note 必须与uh_k_task_lock成对使用。
 * 
 * @param lock_key uh_k_irq_lock返回的系统相关的中断状态。
 * @param from_isr 是否在中断上下文中调用。
 *
 * @return 系统相关的中断状态，用于uh_k_irq_unlock。
 */
extern void uh_k_irq_unlock(uint32_t lock_key, int from_isr);

/**
 * @brief 使能中断。
 *
 * @return N/A。
 */
extern void uh_k_irq_enable(void);

/**
 * @brief 禁止中断。
 *
 * @return N/A。
 */
extern void uh_k_irq_disable(void);

#if 0
typedef void (*irq_entry_t)(void *irq_param);
extern int uh_k_irq_connect(uint32_t irq, uint32_t priority, irq_entry_t entry, void *irq_param, uint32_t options);

extern void uh_k_irq_disconnect(uint32_t irq);
#endif

/** @def TIMEOUT_NO_WAIT/TIMEOUT_WAIT_FOREVER
 *
 * @brief 用于信号量、队列的超时指示。
 * TIMEOUT_NO_WAIT表示条件不满足立即返回
 * TIMEOUT_WAIT_FOREVER表示直到条件满足才返回
 */
#define TIMEOUT_NO_WAIT (0)
#define TIMEOUT_WAIT_FOREVER (-1)

/** @def SEM_MUTEX_QUEUE_TIMEOUT
 *
 * @brief 用于信号量、队列操作的返回值，表示在指定时间内未条件未能满足。
 */
#define SEM_MUTEX_QUEUE_TIMEOUT (-2)

/**
 * @brief 创建互斥信号量。
 * @details 用于临界区的保护。
 *
 * @param options 选项，必须设置为0。
 * @param muetx_id 返回的互斥信号量标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_mutex_create(uint32_t options, uh_mutex_id_t * muetx_id);

/**
 * @brief 删除互斥信号量。
 *
 * @param muetx_id 由uh_k_mutex_create创建的互斥信号量标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_mutex_delete(uh_mutex_id_t muetx_id);

/**
 * @brief 获取互斥信号量。
 * @note 不能嵌套使用。
 * 互斥信号量相关接口均不能在中断中调用。
 *
 * @param muetx_id 由uh_k_mutex_create创建的互斥信号量标识。
 * @param timeout TIMEOUT_NO_WAIT/TIMEOUT_WAIT_FOREVER/超时时间（单位毫秒）。
 *
 * @return 成功返回UH_OK，超时返回SEM_MUTEX_QUEUE_TIMEOUT，失败返回UH_FAIL。
 */
extern int uh_k_mutex_take(uh_mutex_id_t muetx_id, int32_t timeout);

/**
 * @brief 释放互斥信号量。
 * @note 与uh_k_mutex_take成对使用。
 *
 * @param muetx_id 由uh_k_mutex_create创建的互斥信号量标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_mutex_give(uh_mutex_id_t muetx_id);

/**
 * @brief 创建二进制同步信号量。
 * @details 用于任务间的同步通知。
 *
 * @param sem_id 返回的二进制同步信号量标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_sem_b_create(uh_sem_id_t * sem_id);

/**
 * @brief 删除二进制同步信号量。
 *
 * @param sem_id 由uh_k_sem_b_create创建的二进制同步信号量标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_sem_b_delete(uh_sem_id_t sem_id);

/**
 * @brief 获取二进制同步信号量。
 *
 * @param sem_id 由uh_k_sem_b_create创建的二进制同步信号量标识。
 * @param timeout TIMEOUT_NO_WAIT/TIMEOUT_WAIT_FOREVER/超时时间（单位毫秒）。
 *
 * @return 成功返回UH_OK，超时返回SEM_MUTEX_QUEUE_TIMEOUT，失败返回UH_FAIL。
 */
extern int uh_k_sem_b_take(uh_sem_id_t sem_id, int32_t timeout);

/**
 * @brief 释放二进制同步信号量。
 * @note 与uh_k_sem_b_take成对使用。
 *
 * @param sem_id 由uh_k_sem_b_create创建的二进制同步信号量标识。
 * @param from_isr 是否在中断上下文中调用。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_sem_b_give(uh_sem_id_t sem_id, int from_isr);

/**
 * @brief 创建计数型同步信号量。
 * @details 用于任务间的同步通知。
 *
 * @param initial_count 信号量计数初值
 * @param max_count 信号量计数最大值
 * @param sem_id 返回的计数型同步信号量标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_sem_c_create(uint32_t initial_count, uint32_t max_count, uh_sem_id_t * sem_id);

/**
 * @brief 删除计数型同步信号量。
 *
 * @param sem_id 由uh_k_sem_c_create创建的计数型同步信号量标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_sem_c_delete(uh_sem_id_t sem_id);

/**
 * @brief 获取计数型同步信号量。
 *
 * @param sem_id 由uh_k_sem_c_create创建的计数型同步信号量标识。
 * @param timeout TIMEOUT_NO_WAIT/TIMEOUT_WAIT_FOREVER/超时时间（单位毫秒）。
 *
 * @return 成功返回UH_OK，超时返回SEM_MUTEX_QUEUE_TIMEOUT，失败返回UH_FAIL。
 */
extern int uh_k_sem_c_take(uh_sem_id_t sem_id, int32_t timeout);

/**
 * @brief 释放计数型同步信号量。
 * @note 与uh_k_sem_c_take成对使用。
 *
 * @param sem_id 由uh_k_sem_c_create创建的计数型同步信号量标识。
 * @param from_isr 是否在中断上下文中调用。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_sem_c_give(uh_sem_id_t sem_id, int from_isr);

/**
 * @brief 创建消息队列。
 * @details 用于任务间的通信。
 *
 * @param options 选项，必须设置为0。
 * @param msg_size 消息大小。
 * @param max_msgs 消息队列的大小。
 * @param queue_id 返回的消息队列标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_queue_create(uint32_t options, uint32_t msg_size, uint32_t max_msgs, uh_queue_id_t * queue_id);

/**
 * @brief 删除消息队列。
 *
 * @param queue_id 由uh_k_queue_create创建的消息队列标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_queue_delete(uh_queue_id_t queue_id);

/**
 * @brief 发送消息到消息队列。
 *
 * @param queue_id 由uh_k_queue_create创建的消息队列标识。
 * @param buffer 待发送的消息指针。
 * @param timeout TIMEOUT_NO_WAIT/TIMEOUT_WAIT_FOREVER/超时时间（单位毫秒）。
 *
 * @return 成功返回UH_OK，超时返回SEM_MUTEX_QUEUE_TIMEOUT，失败返回UH_FAIL。
 */
extern int uh_k_queue_send(uh_queue_id_t queue_id, void * buffer, int32_t timeout);

/**
 * @brief 中断上下文中发送消息到消息队列。
 *
 * @param queue_id 由uh_k_queue_create创建的消息队列标识。
 * @param buffer 待发送的消息指针。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_queue_send_from_isr(uh_queue_id_t queue_id, void * buffer);

/**
 * @brief 从消息队列接收消息。
 *
 * @param queue_id 由uh_k_queue_create创建的消息队列标识。
 * @param buffer 存储消息的指针。
 * @param timeout TIMEOUT_NO_WAIT/TIMEOUT_WAIT_FOREVER/超时时间（单位毫秒）。
 *
 * @return 成功返回UH_OK，超时返回SEM_MUTEX_QUEUE_TIMEOUT，失败返回UH_FAIL。
 */
extern int uh_k_queue_receive(uh_queue_id_t queue_id, void * buffer, int32_t timeout);

/**
 * @typedef timer_expire_t
 * @brief 定时器处理入口。
 * @param timer_param 定时器处理入口参数。
 */
typedef void (*timer_expire_t)(void *timer_param);

/**
 * @brief 创建定时器。
 *
 * @param expire_fn 定时器处理入口。
 * @param timer_param 定时器处理入口参数。
 * @param timer_id 返回的定时器标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_timer_create(timer_expire_t expire_fn, void *timer_param, uh_timer_id_t * timer_id);

/**
 * @brief 删除定时器。
 *
 * @param timer_id 由uh_k_timer_create创建的定时器标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_timer_delete(uh_timer_id_t timer_id);

/**
 * @brief 判断定时器是否运行中。
 *
 * @param timer_id 由uh_k_timer_create创建的定时器标识。
 *
 * @return 运行中返回UH_TRUE，未运行返回UH_FALSE。
 */
extern int uh_k_timer_is_active(uh_timer_id_t timer_id);

/**
 * @brief 启动定时器。
 *
 * @param timer_id 由uh_k_timer_create创建的定时器标识。
 * @param period 定时器的超时时间（单位毫秒）。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_timer_start(uh_timer_id_t timer_id, uint32_t period);

/**
 * @brief 停止定时器。
 *
 * @param timer_id 由uh_k_timer_create创建的定时器标识。
 *
 * @return 成功返回UH_OK，失败返回UH_FAIL。
 */
extern int uh_k_timer_stop(uh_timer_id_t timer_id);

/**
 * @brief 获取系统运行时间。
 *
 * @return 系统运行时间（单位毫秒）。
 */
extern uint32_t uh_k_uptime_get(void);

/**
 * @brief 中断上下文中获取系统运行时间。
 *
 * @return 系统运行时间（单位毫秒）。
 */
extern uint32_t uh_k_uptime_get_from_isr(void);

/**
 * @brief 计算时间差。
 * @details 系统运行时间未溢出时，直接计算时间差；系统运行时间溢出后，基于卷回计算时间差。
 * @note 最大可计算时间差只能包含一次溢出卷回。
 *
 * @param now 当前时间。
 * @param ref_time 参考时间。
 *
 * @return 时间差（单位毫秒）。
 */
extern uint32_t uh_k_uptime_delta(uint32_t now, uint32_t ref_time);

/**
 * @brief 设置任务调度日志开关。
 * @details 支持任务创建、删除、挂起、恢复、睡眠、运行日志的开关控制。
 *
 * @param level_bits 按比特表示，1为开启，0为关闭。
 *
 * @return N/A。
 */
extern void uh_k_log_set(uint32_t level_bits);

/**
 * @brief 任务调度日志输出。
 *
 * @param log_type 任务调度类型。
 * @param task_name 任务名称。
 *
 * @return N/A。
 */
extern void uh_k_log(int log_type, const char * task_name);

#if 0
typedef int (* pmu_ready_to_sleep_func)(void);
typedef int (* pmu_func_pre_sleep_func)(void);
typedef void (* pmu_func_post_sleep_func)(void);
extern void uh_k_pmu_set_sleep(
	pmu_ready_to_sleep_func ready_to_sleep,
	pmu_func_pre_sleep_func pre_sleep,
	pmu_func_post_sleep_func post_sleep);
#endif

/**
 * @brief 原子加操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 加操作的值。
 *
 * @return N/A。
 */
extern void uh_atomic_add(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子加操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 加操作的值。
 *
 * @return 原子操作后的值。
 */
extern uh_atomic_val_t uh_atomic_add_return(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子减操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 减操作的值。
 *
 * @return N/A。
 */
extern void uh_atomic_sub(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子减操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 减操作的值。
 *
 * @return 原子操作后的值。
 */
extern uh_atomic_val_t uh_atomic_sub_return(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子设置操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 设置操作的值。
 *
 * @return N/A。
 */
extern void uh_atomic_set(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子或操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 或操作的值。
 *
 * @return N/A。
 */
extern void uh_atomic_or(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子或操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 或操作的值。
 *
 * @return 原子操作后的值。
 */
extern uh_atomic_val_t uh_atomic_or_return(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子与操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 与操作的值。
 *
 * @return N/A。
 */
extern void uh_atomic_and(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子与操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 与操作的值。
 *
 * @return 原子操作后的值。
 */
extern uh_atomic_val_t uh_atomic_and_return(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子异或操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 异或操作的值。
 *
 * @return N/A。
 */
extern void uh_atomic_xor(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子异或操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 异或操作的值。
 *
 * @return 原子操作后的值。
 */
extern uh_atomic_val_t uh_atomic_xor_return(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子与非操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 与非操作的值。
 *
 * @return N/A。
 */
extern void uh_atomic_nand(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子与非操作。
 *
 * @param target 操作目标，输入输出。
 * @param value 与非操作的值。
 *
 * @return 原子操作后的值。
 */
extern uh_atomic_val_t uh_atomic_nand_return(uh_atomic_t *target, uh_atomic_val_t value);

/**
 * @brief 原子读操作。
 *
 * @param target 操作目标。
 *
 * @return 目标值。
 */
#define uh_atomic_get(target) (*target)

/**
 * @brief 原子加1操作。
 *
 * @param target 操作目标，输入输出。
 *
 * @return N/A。
 */
#define uh_atomic_inc(target) uh_atomic_add(target, 1)

/**
 * @brief 原子加1操作。
 *
 * @param target 操作目标，输入输出。
 *
 * @return 原子操作后的值。
 */
#define uh_atomic_inc_return(target) uh_atomic_add_return(target, 1)

/**
 * @brief 原子减1操作。
 *
 * @param target 操作目标，输入输出。
 *
 * @return N/A。
 */
#define uh_atomic_dec(target) uh_atomic_sub(target, 1)

/**
 * @brief 原子减1操作。
 *
 * @param target 操作目标，输入输出。
 *
 * @return 原子操作后的值。
 */
#define uh_atomic_dec_return(target) uh_atomic_sub_return(target, 1)

/**
 * @brief 原子清零操作。
 *
 * @param target 操作目标，输入输出。
 *
 * @return N/A。
 */
#define uh_atomic_clear(target) uh_atomic_set(target, 0)
#endif
#ifdef __cplusplus
}
#endif

#endif /*__UH_KERNEL_H__*/

