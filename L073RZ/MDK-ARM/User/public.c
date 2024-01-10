#include "public.h"

void LPUART1_SendString(char* String)
{
	HAL_UART_Transmit(&hlpuart1,(uint8_t*)String,strlen(String),0xffff);    //发送接收到的数据
}

void UART2_SendString(char* String)
{
	HAL_UART_Transmit(&huart2,(uint8_t*)String,strlen(String),0xffff);    //发送接收到的数据
}

void Clear_Buffer(void)
{
	      for(int i = 0;i<LPUART1_Rx_cnt;i++)
        LPUART1_Rx_Buf[i] = 0;
      LPUART1_Rx_cnt = 0;
      LPUART1_Rx_flg = 0;
	
	      for(int i = 0;i<UART2_Rx_cnt;i++)
        UART2_Rx_Buf[i] = 0;
      UART2_Rx_cnt = 0;
      UART2_Rx_flg = 0U;
	
}
