
#include "uplus_trans.h"
#include "string.h"

/* static uint8_t s_u8RxBuff[256] = {0};
static uint8_t s_u8TxBuff[256] = {0};
static uint8_t s_u8BkBuff[256] = {0}; */
static T_BpParam s_tBpParam = {0};
static T_Time s_tTime = {0};

#define MYLOG(...) printf(__VA_ARGS__)

extern int at_uart_send_packet(const uint8_t *packet, int size);
extern int uplus_ble_sdk_config_start(void);
extern void uplus_report(uint8_t *buff, uint8_t len, uint8_t isHist);

static uint8_t BP_CheckXor(uint8_t *pBuff, uint8_t u8Len)
{
	uint8_t u8Result = 0;
	uint8_t u8Idx;
	for (u8Idx = 0; u8Idx < u8Len; u8Idx++)
	{
		u8Result ^= pBuff[u8Idx];
	}
	return u8Result;
}

static void BP_DeviceCtrl(uint8_t cmd, uint8_t *pData, uint8_t len)
{
	uint8_t u8Idx;
	uint8_t u8Buff[20] = {0x02, 0x40, 0xDC, 0x01, 0x00, 0x00};

	
	u8Buff[4] = cmd;
	if (len > 0)
	{
		u8Buff[3] += len;
		memcpy(&u8Buff[5], pData, len);
	}
	u8Buff[5+len] = BP_CheckXor(u8Buff + 1, 4+len);

	at_uart_send_packet(u8Buff, 6+len);
}

void UPLUS_SyncMeasData(T_BpParam bpParam, T_Time tTime, uint8_t isHist)
{
	T_AppFrame appFrame;
	appFrame.no = 0x01;
	appFrame.measFlg = bpParam.u8MeasFlg;
	appFrame.year = tTime.year + 30;
	appFrame.month = tTime.month;
	appFrame.day = tTime.day;
	appFrame.hour = tTime.hour;
	appFrame.minute = tTime.minute;
	
	appFrame.sysHi = bpParam.u16Sys >> 8;
	appFrame.sysLo = bpParam.u16Sys;
	appFrame.diaHi = bpParam.u16Dia >> 8;
	appFrame.diaLo = bpParam.u16Dia;
	appFrame.hrHi = 0;
	appFrame.hrLo = bpParam.u8Pr;
	appFrame.user = bpParam.u8User;
	appFrame.batt = bpParam.u8Batt;
	
	uplus_report(&appFrame, sizeof(T_AppFrame), isHist);
}

uint8_t UPLUS_GetStateData(uint8_t *pData)
{
	pT_AppFrame pFrame = (pT_AppFrame)pData;
	pFrame->no = 0x01;
	pFrame->measFlg = s_tBpParam.u8MeasFlg;
	pFrame->year = s_tTime.year + 30;
	pFrame->month = s_tTime.month;
	pFrame->day = s_tTime.day;
	pFrame->hour = s_tTime.hour;
	pFrame->minute = s_tTime.minute;
	
	pFrame->sysHi = s_tBpParam.u16Sys >> 8;
	pFrame->sysLo = s_tBpParam.u16Sys;
	pFrame->diaHi = s_tBpParam.u16Dia >> 8;
	pFrame->diaLo = s_tBpParam.u16Dia;
	pFrame->hrHi = 0;
	pFrame->hrLo = s_tBpParam.u8Pr;
	pFrame->user = s_tBpParam.u8User;
	pFrame->batt = s_tBpParam.u8Batt;
	return sizeof(T_AppFrame);
}

static void BP_SyncTime(uint8_t *pData)
{
	uint8_t u8Buff[20] = {0x02, 0x40, 0xDC, 0x07, 0xB0, 0x00};

	u8Buff[5] = pData[0] - 30;
	u8Buff[6] = pData[1];
	u8Buff[7] = pData[2];
	u8Buff[8] = pData[3];
	u8Buff[9] = pData[4];
	u8Buff[10] = 0;

	u8Buff[11] = BP_CheckXor(u8Buff+1, 10);

	at_uart_send_packet(u8Buff, 12);
}


static void BP_DatasProc(pT_Frame pFrame)
{
	T_BpParam tBpParam = {0};

	if (CMD_DEV_RSP == pFrame->u8Cmd)
	{
		if (2 == pFrame->u8Len)
		{
			MYLOG("BP Press Data!\n");
			s_tBpParam.u8MeasFlg = 0x10;
			s_tBpParam.u16Sys = pFrame->u8Data[0]*0x0100 + pFrame->u8Data[1];
			
			UPLUS_SyncMeasData(s_tBpParam, s_tTime, 0);
		}
		else if (3 == pFrame->u8Len)
		{
			MYLOG("BP Batt Data(%d)!\n", pFrame->u8Data[2]);
			s_tBpParam.u8Batt = pFrame->u8Data[2];

			UPLUS_SyncMeasData(s_tBpParam, s_tTime, 0);
		}
		else if (0x0d == pFrame->u8Len && 0xB3 == pFrame->u8Data[0])
		{
			MYLOG("BP History Data!\n");

//			s_tBpParam.u16Sys = pFrame->u8Data[1];
//			s_tBpParam.u16Dia = pFrame->u8Data[2];
//			s_tBpParam.u8Pr = pFrame->u8Data[3];
//			s_tBpParam.u8State = pFrame->u8Data[4];
//			s_tBpParam.u8User = pFrame->u8Data[5];
//
//			tTime.year = pFrame->u8Data[7];
//			tTime.month = pFrame->u8Data[8];
//			tTime.day = pFrame->u8Data[9];
//			tTime.hour = pFrame->u8Data[10];
//			tTime.minute = pFrame->u8Data[11];
//			tTime.second = 0;

//			uplus_report(&(pFrame->u8Data[1]), 12, 1);
		}
		else if (0x0F == pFrame->u8Len && 0x1C == pFrame->u8Data[0])
		{
			MYLOG("BP Meas Succ Data!\n");
			s_tBpParam.u8MeasFlg = 0;
			s_tBpParam.u16Sys = pFrame->u8Data[1]*0x0100 + pFrame->u8Data[2];
			s_tBpParam.u16Dia = pFrame->u8Data[3]*0x0100 + pFrame->u8Data[4];
			s_tBpParam.u8State = pFrame->u8Data[6];
			s_tBpParam.u8Pr = pFrame->u8Data[8];
			s_tBpParam.u8User = pFrame->u8Data[9];

			s_tTime.year = pFrame->u8Data[10];
			s_tTime.month = pFrame->u8Data[11];
			s_tTime.day = pFrame->u8Data[12];
			s_tTime.hour = pFrame->u8Data[13];
			s_tTime.minute = pFrame->u8Data[14];
			s_tTime.second = 0;

			s_tBpParam.u8IsUpLoad = 1;
			UPLUS_SyncMeasData(s_tBpParam, s_tTime, 1);
		}
		else if (0x0C <= pFrame->u8Len && 0x3C == pFrame->u8Data[0])
		{
			MYLOG("BP Meas Fail Data!\n");
			
			s_tBpParam.u8MeasFlg = 0;

			//UPLUS_SyncMeasData(s_tBpParam, tTime);
		}
	}
	else if (CMD_DEV_NOTIFY == pFrame->u8Cmd)
	{
		if (0x04 == pFrame->u8Data[0])
		{
			MYLOG("BP Dev Info Reset!\n");
			/* 
			myCallback(MSG_ENROLL_UNBOND);
			BP_DeviceCtrl(OP_ACK); */
			uplus_ble_sdk_config_start();
		}
		else if (0x01 == pFrame->u8Data[0])
		{
			MYLOG("BP Start Meas!\n");
			s_tBpParam.u8MeasFlg = 0x10;
		}
		else if (0x02 == pFrame->u8Data[0])
		{
			MYLOG("BP Stop Meas!\n");
			s_tBpParam.u8MeasFlg = 0;
		}
	}
}

void BP_FrameParse(uint8_t *pData, uint8_t u8Len)
{
	static uint8_t u8Buffer[20];
	static uint8_t u8BufIdx = 0;
	static uint8_t u8ResetByte = 0;
	static uint8_t u8HeadFlg = 0;
	static uint8_t u8DevFlg = 0;
	static uint8_t u8CmdFlg = 0;
	static uint8_t u8LenFlg = 0;
	uint8_t u8Idx = 0;
	uint8_t u8CurByte;

	for (;u8Idx < u8Len;u8Idx++)
	{
		u8CurByte = pData[u8Idx];

		if (u8HeadFlg)
		{
			if (u8DevFlg)
			{
				if (u8CmdFlg)
				{
					if (u8LenFlg)
					{
						u8Buffer[u8BufIdx++] = u8CurByte;
						u8ResetByte--;
						if (!u8ResetByte)
						{
							u8CurByte = BP_CheckXor(u8Buffer+1, u8BufIdx-2);
							if (u8CurByte == u8Buffer[u8BufIdx-1])
							{
								BP_DatasProc((pT_Frame)u8Buffer);
							}

							u8BufIdx = 0;
							u8HeadFlg = 0;
							u8DevFlg = 0;
							u8CmdFlg = 0;
							u8LenFlg = 0;
						}
					}
					else
					{
						if (MAX_FRAME_LENGTH > u8CurByte)
						{
							u8Buffer[u8BufIdx++] = u8CurByte;
							u8ResetByte = u8CurByte + 1;
							u8LenFlg = 1;
						}
						else
						{
							u8HeadFlg = 0;
						}
					}
				}
				else
				{
					if (FRAME_CMD_2_HOST == u8CurByte || FRAME_CMD_2_HOST2 == u8CurByte)
					{
						u8Buffer[u8BufIdx++] = u8CurByte;
						u8CmdFlg = 1;
					}
					else
					{
						u8HeadFlg = 0;
					}
				}
			}
			else
			{
				if (FRAME_DEVIVE == u8CurByte)
				{
					u8Buffer[u8BufIdx++] = u8CurByte;
					u8DevFlg = 1;
				}
				else
				{
					u8HeadFlg = 0;
				}
			}
		}
		else
		{
			if (FRAME_HEAD == u8CurByte)
			{

				u8Buffer[0] = u8CurByte;
				u8BufIdx = 1;
				u8HeadFlg = 1;
				u8DevFlg = 0;
				u8CmdFlg = 0;
				u8LenFlg = 0;
			}
		}

	}
}

uint8_t UPLUS_Parse(uint8_t *pData, uint8_t u8Len)
{
	uint8_t u8Buff[1];
	if (pData[0] == 0x4D && (pData[1] == 0x01 || pData[1] == 0xfe))
	{
		MYLOG("App query Data!(cmd:%d)\n", pData[1]);
		UPLUS_SyncMeasData(s_tBpParam, s_tTime, 0);
	}
	if (pData[0] == 0x5D && pData[1] == 0x03)
	{
		MYLOG("App ctrl Data!(cmd:%d)\n", pData[3]);
		if (pData[3] == 1)
		{
			BP_DeviceCtrl(CTRL_START, NULL, 0);
		}
		else
		{
			BP_DeviceCtrl(CTRL_STOP, NULL, 0);
		}
	}
	else if (pData[0] == 0x5D && pData[1] == 0x05)
	{	
		if (pData[3] < 2)
		{
			u8Buff[0] = pData[3];
			BP_DeviceCtrl(CTRL_CHANGE_USER, u8Buff, 1);
		}
		else
		{
			return 1;
			MYLOG("user error!\n");
		}
	}
	else if (pData[0] == 0x00 && pData[1] == 0x01)
	{
		MYLOG("App set time!\n");
		BP_SyncTime(&pData[3]);
	}
	return 0;
}
