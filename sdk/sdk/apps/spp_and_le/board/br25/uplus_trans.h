#ifndef __UPLUS_H__
#define __UPLUS_H__

#include "stdint.h"

#define MAX_FRAME_LENGTH 24
#define FRAME_CMD_2_HOST  0xDD
#define FRAME_CMD_2_HOST2 0xa1
#define FRAME_DEVIVE 0x40
#define FRAME_HEAD 0x02
#define CMD_DEV_NOTIFY 0xa1
#define CMD_DEV_RSP 0xdd

typedef enum
{
	CTRL_START = 0xA1,
	CTRL_STOP,
	
	CTRL_ACK = 0xAD,
	CTRL_CHANGE_USER,
	CTRL_MAX,
}E_BP_CTRL_TYPE;

typedef struct _time
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	
}T_Time, *pT_Time;

typedef struct _bpParam
{
	uint16_t u16Sys;
	uint16_t u16Dia;
	uint8_t u8Pr;
	uint8_t u8State;
	uint8_t u8User;
	uint8_t u8Batt;
	uint8_t u8IsUpLoad;
	uint8_t u8MeasFlg;
}T_BpParam, *pT_BpParam;

typedef struct _frame
{
	uint8_t u8Head;
	uint8_t u8Dev;
	uint8_t u8Cmd;
	uint8_t u8Len;
	uint8_t u8Data[1];
}T_Frame, *pT_Frame;

typedef struct _appFrame
{
	uint8_t no;
	uint8_t measFlg;
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t sysHi;
	uint8_t sysLo;
	uint8_t diaHi;
	uint8_t diaLo;
	uint8_t hrHi;
	uint8_t hrLo;
	uint8_t null1;
	uint8_t null2;
	uint8_t null3;
	uint8_t user;
	uint8_t null4;
	uint8_t batt;
}T_AppFrame, *pT_AppFrame;


void BP_FrameParse(uint8_t *pData, uint8_t u8Len);
uint8_t UPLUS_Parse(uint8_t *pData, uint8_t u8Len);
uint8_t UPLUS_GetStateData(uint8_t *pData);

#endif

