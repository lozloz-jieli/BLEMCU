/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

/**
 * @file uh_compiler.h
 * @brief 与编译器相关的预编译宏和编译选项定义
 *
 * @date 2017-03-28
 * @author fanming
 *
 */

#ifndef __UH_COMPILER_H__
#define __UH_COMPILER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define _UH_STRINGIFY(x)     #x
#define UH_STRINGIFY(x)     _UH_STRINGIFY(x)

/* Compiler Related Definitions */
#if defined (__CC_ARM)                 /* ARM Compiler */
	#include <stdarg.h>
	#define UH_SECTION(x)       __attribute__((section(x)))
	#define UH_USED             __attribute__((used))
	#define UH_ALIGN(n)         __attribute__((aligned(n)))
	#define UH_WEAK             __weak
	#define UH_INLINE           static __inline
	#define UH_PACKED           __packed

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
	#include <stdarg.h>
	#define UH_SECTION(x)       _Pragma(UH_STRINGIFY(location=x))
	#define UH_USED
	#define UH_ALIGN(n)         _Pragma(UH_STRINGIFY(data_alignment=n))
	#define UH_WEAK             __weak
	#define UH_INLINE           static inline

#elif defined (__GNUC__)        /* GNU GCC Compiler */
	typedef __builtin_va_list   __gnuc_va_list;
	typedef __gnuc_va_list      va_list;
	#define va_start(v,l)       __builtin_va_start(v,l)
	#define va_end(v)           __builtin_va_end(v)
	#define va_arg(v,l)         __builtin_va_arg(v,l)

	#define UH_SECTION(x)       __attribute__((section(x)))
	#define UH_USED             __attribute__((used))
	#define UH_ALIGN(n)         __attribute__((aligned(n)))
	#define UH_WEAK             __attribute__((weak))
	#define UH_INLINE           static __inline
	#define UH_PACKED           __attribute__ ((__packed__))

#else /*use IAR*/
	#define UH_SECTION(x)       _Pragma(UH_STRINGIFY(location=x))
	#define UH_USED
	#define UH_ALIGN(n)         _Pragma(UH_STRINGIFY(data_alignment=n))
	#define UH_WEAK             __weak
	#define UH_INLINE           static inline
#endif

#define ARG_UNUSE(x) (void)(x)

#ifdef __cplusplus
}
#endif

#endif /*__UH_COMPILER_H__*/
