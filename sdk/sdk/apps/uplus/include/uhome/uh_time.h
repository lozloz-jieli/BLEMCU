#ifndef __UH_TIME_H__
#define __UH_TIME_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BASE_UNIX_TIME                    28800//0x5C2A3D00
#define IsLeapYear(yr) (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))

#define DAY                         86400
#define HOUR                        3600
#define MINU                        60

#define BASE_YEAR                    1970// 2019
#define BASE_MONTH                    1
#define BASE_DAY                    1
#define BASE_HOUR                     0
#define BASE_MIN                    0
#define BASE_SEC                    0

typedef struct {
    volatile uint16_t year;    // 2019+
    volatile uint8_t month;    // 0-11
    volatile uint8_t day;      // 0-30
    volatile uint8_t seconds;  // 0-59
    volatile uint8_t minutes;  // 0-59
    volatile uint8_t hour;     // 0-23
    volatile uint8_t weekday;
} UTCTimeStructs;
//UTCTimeStructs local_time;
typedef enum {
    JAN = 31,
    FEB = 28,
    MAR = 31,
    APR = 30,
    MAY = 31,
    JUN = 30,
    JUL = 31,
    AUG = 31,
    SEP = 30,
    OCT = 31,
    NOV = 30,
    DEC = 31,
} enum_month_t;


typedef struct {
	uint32_t total_sec;
	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint32_t hour;
	uint32_t min;
	uint32_t sec;
	int32_t timeZone;
} uh_time_t;

int8_t uh_local_time_get(uint32_t sys_time, uh_time_t *ts);

#ifdef __cplusplus
}
#endif

#endif /*__UH_TIME_H__*/


