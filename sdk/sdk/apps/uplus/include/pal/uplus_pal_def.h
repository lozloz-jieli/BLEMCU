
#ifndef __UPLUS_PAL_DEF_H__
#define __UPLUS_PAL_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* os */
#define TIME_NO_WAIT                0
#define TIME_WAIT_FOREVER           0
#define SEM_TAKE_TIMEOUT            0
#define uplus_os_task_create(name, stack_size, priority, func, para, id)
#define uplus_os_task_delete(id)
#define uplus_os_task_sleep(delay)
#define uplus_os_mutex_create(id)
#define uplus_os_mutex_take(id, time_wait)
#define uplus_os_mutex_give(id)
#define uplus_os_mutex_delete(id)
#define uplus_os_sem_create(id)
#define uplus_os_sem_take(id, time_wait)
#define uplus_os_sem_give(id)
#define uplus_os_sem_delete(id)
#define uplus_os_current_time_get()   uplus_sys_uptime_get()
#define uplus_os_diff_time_cal(new, old)

extern void* safe_malloc(size_t size);
extern void safe_free(void *FirstByte);
#define uplus_tool_safe_malloc(size)                safe_malloc(size)
#define uplus_tool_safe_free(p)                     safe_free(p)
#define uplus_tool_malloc(size)                     malloc(size)
#define uplus_tool_free(p)                          free(p)

#define uplus_tool_memcpy(dest, src, n)             memcpy(dest, src, n)
#define uplus_tool_memmove(dest, src, n)
#define uplus_tool_memset(dest, c, n)               memset(dest, c, n)
#define uplus_tool_memcmp(dest, src, n)             memcmp(dest, src, n)
#define uplus_tool_strncmp(s1, s2, n)               strncmp(s1, s2, n)
#define uplus_tool_strcmp(s1, s2)                   strcmp(s1, s2)
#define uplus_tool_strncpy(s1, s2, n)               strncpy(s1, s2, n)
#define uplus_tool_strcpy(s1, s2)                   strcpy(s1, s2)
#define uplus_tool_strlen(s)                        strlen(s)
#define uplus_tool_strcasecmp(s1, s2)               strcasecmp(s1, s2)
#define uplus_tool_strncasecmp(s1, s2, n)
#define uplus_tool_atoi(s)
#define uplus_tool_isdigit(c)
#define uplus_tool_isalnum(c)
#define uplus_tool_isprint(c)

#ifdef P_TELINK_8258
#define uplus_tool_sprintf(s, fmt, ...)             u_sprintf(s, fmt, ##__VA_ARGS__)
#define uplus_tool_snprintf(s, len, fmt, ...)       u_snprintf(s, len, fmt, ##__VA_ARGS__)
#define uplus_tool_vsnprintf(s, len, fmt, va)       vsnprintf(s, len, fmt, va)
#else
#define uplus_tool_sprintf(s, fmt, ...)             sprintf(s, fmt, ##__VA_ARGS__)
#define uplus_tool_snprintf(s, len, fmt, ...)       snprintf(s, len, fmt, ##__VA_ARGS__)
#define uplus_tool_vsnprintf(s, len, fmt, va)       vsnprintf(s, len, fmt, va)
#endif

#define uplus_tool_rand()                           uplus_rand()
#define uplus_tool_srand(seed)                      uplus_srand(seed)
#define uplus_net_htons(hostshort)                  htons(hostshort)
#define uplus_net_ntohs(netshort)                   ntohs(netshort)
#define uplus_net_htonl(hostlong)                   htonl(hostlong)
#define uplus_net_ntohl(netlong)                    ntohl(netlong)

#define MODE_ENCRYPT                                UH_MODE_ENCRYPT
#define MODE_DECRYPT                                UH_MODE_DECRYPT
#define uplus_tool_aes_crypt_cbc(mode, key, key_len, in, in_len, out)   uh_crypt_aes_crypt_cbc(mode, key, key_len, in, in_len, out)

#ifdef __cplusplus
}
#endif

#endif /*__UPLUS_PAL_DEF_H__*/

