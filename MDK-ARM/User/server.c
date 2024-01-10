#include "server.h"

void Write_RTCTime(void)
{
	RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
	
	sTime.Hours = 0x00;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;
	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}

void Sendover_Callback(void)	//发送远程图像数据上送结束标记
{
//	uint8_t Device_ID=;
	uint8_t len[2]={0x00,0x00};
	uint8_t str[33];
	len[1]=sizeof(str);
//	for(int i=0;i<33;i++)
//	{
//		if(i<2)
//			str[i]=SYNC[i];
//		else if(i<4)
//			str[i]=Packet_Length[i-2];
//		else if(i<21)
//			str[i]=CMD_ID[i-4];
//		else if(i<22)
//			str[i]=Frame_Type;
//		else if(i<23)
//			str[i]=Packet_Type;
//		else if(i<24)
//			str[i]=Frame_No;
//		else if(i<25)
//			str[i]=Channel_No;
//		else if(i<26)
//			str[i]=Presetting_No;
//		else if(i<30)
//			str[i]=Time_Stamp[i-26];
//		else if(i<32)
//			str[i]=CRC16[i-30];
//		else
//		{
//			str[32]=END;
//			break;
//		}
//	}
	uint8_t offset=0U;
	memcpy(str+offset,(uint8_t*)SYNC,sizeof(SYNC));	//报文头5AA5H
	offset+=sizeof(SYNC);
	memcpy(str+offset,(uint8_t*)Packet_Length,sizeof(Packet_Length));	//报文长度
	offset+=sizeof(Packet_Length);
	memcpy(str+offset,(uint8_t*)CMD_ID,sizeof(CMD_ID));	//检测装置编码（17位编码）
	offset+=sizeof(CMD_ID);
	str[offset]=Frame_Type;	//帧类型一一参考表C.77相关含义
	offset+=sizeof(Frame_Type);
	str[offset]=Packet_Type;	//报文类型一一参考表C.78相关含义
	offset+=sizeof(Packet_Type);
	str[offset]=Frame_No;	//帧序列号，无符号整数
	offset+=sizeof(Frame_No);
	str[offset]=Channel_No;	//通道号一一表示监测装置的摄像机编号
	offset+=sizeof(Channel_No);
	str[offset]=Presetting_No;	//预设位号，不带云台摄像机，预设位号位0xFF
	offset+=sizeof(Presetting_No);
	memcpy(str+offset,(uint8_t*)Time_Stamp,sizeof(Time_Stamp));	//时间
	offset+=sizeof(Time_Stamp);
	memcpy(str+offset,(uint8_t*)CRC16,sizeof(CRC16));	//校验位
	offset+=sizeof(CRC16);
	str[offset]=END;
	offset+=sizeof(END);	//报文尾：96H
	printf("str的赋值结果为%s,offset=%d",str,offset);
//	UART2_SendString(SYNC);
}