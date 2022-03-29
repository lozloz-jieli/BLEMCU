/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

/**
 * @file uh_type.h
 * @brief UH基本类型定义
 *
 * @date 2017-04-14
 * @author fanming
 *
 */

#ifndef __UH_TYPE_H__
#define __UH_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __INT64_TYPE__
    /* armclang predefines '__INT64_TYPE__' and '__INT64_C_SUFFIX__' */
    #define __INT64 __INT64_TYPE__
#else
    /* armcc has builtin '__int64' which can be used in --strict mode */
    #define __INT64 __int64
    #define __INT64_C_SUFFIX__ ll
#endif
  
#define UH_TRUE 1
#define UH_FALSE 0

#define UH_OK 0
#define UH_FAIL -1

#ifndef NULL
#define NULL ((void*) 0)
#endif

#define UNUSED(x)  ((void) x)

#ifdef __cplusplus
}
#endif

#endif /*__UH_TYPE_H__*/

