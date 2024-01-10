/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern volatile uint8_t LPUART1_Rx_cnt;				//低功耗串口接收到的数据长度
extern uint8_t LPUART1_Rx_Buf[MAX_LEN];					//低功耗串口数据接受区
extern volatile uint8_t LPUART1_temp[1];					//用于存放低功耗串口接收到的单字节
extern volatile uint8_t LPUART1_Rx_flg;			//低功耗串口超时信号
extern volatile uint8_t UART2_Rx_cnt;				//二号串口接收到的数据长度
extern uint8_t UART2_Rx_Buf[MAX_LEN];					//二号串口数据接受区
extern volatile uint8_t UART2_temp[1];					//用于存放二号串口接收到的单字节
extern volatile uint8_t UART2_Rx_flg;				//二号串口超时信号

int fputc(int ch, FILE *f);//printf重定义
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
