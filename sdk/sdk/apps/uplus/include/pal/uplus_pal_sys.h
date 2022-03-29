#ifndef __UPLUS_PAL_SYS_H__
#define __UPLUS_PAL_SYS_H__


/**
 * \brief 获取系统上电时间
 */
extern uint32_t uplus_sys_uptime_get(void);

/**
 * \brief 获取上电时间差值
 */
uint32_t uplus_sys_uptime_delta(uint32_t now, uint32_t ref_time);

/**
 * \brief 系统重启
 */
extern void uplus_sys_reboot(void);

#endif

