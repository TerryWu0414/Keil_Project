/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ec20.h"
#include "server.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_LEN 100  //单个数据包的最大长度
#define REC_LENGTH 1 //接收数据单位长度
#define LED GPIO_PinState

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t SYNC[2]={0x5A,0xA5};
volatile uint8_t Packet_Length[2];
volatile uint8_t CMD_ID[17];
volatile uint8_t Frame_Type;
volatile uint8_t Packet_Type;
volatile uint8_t Frame_No;
volatile uint8_t Channel_No;
volatile uint8_t Presetting_No;
volatile uint8_t Time_Stamp[4];
volatile uint8_t CRC16[2];
volatile uint8_t END;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t Button_flg=0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if( GPIO_Pin == B1_Pin )//判断外部中断源
	{ /* Prevent unused argument(s) compilation warning */
		Button_flg=1;
   HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //反转PA5 LED灯的状态
    /* NOTE: This function Should not be modified, when the callback is needed,
             the HAL_GPIO_EXTI_Callback could be implemented in the user file
     */}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

{
  if(huart->Instance==LPUART1)
  {
    __HAL_TIM_SET_COUNTER(&htim2,0);
    if(0 == LPUART1_Rx_cnt)//如果是第一个字符，则开启定时器
    {
    __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_UPDATE);
      HAL_TIM_Base_Start_IT(&htim2);
    }
    LPUART1_Rx_Buf[LPUART1_Rx_cnt] = LPUART1_temp[0];
    LPUART1_Rx_cnt++;
    HAL_UART_Receive_IT(&hlpuart1,(uint8_t *)LPUART1_temp,REC_LENGTH);
  }
	  if(huart->Instance==USART2)
  {
    __HAL_TIM_SET_COUNTER(&htim3,0);
    if(0 == UART2_Rx_cnt)//如果是第一个字符，则开启定时器
    {
    __HAL_TIM_CLEAR_FLAG(&htim3,TIM_FLAG_UPDATE);
      HAL_TIM_Base_Start_IT(&htim3);
    }
    UART2_Rx_Buf[UART2_Rx_cnt] = UART2_temp[0];
    UART2_Rx_cnt++;
    HAL_UART_Receive_IT(&huart2,(uint8_t *)UART2_temp,REC_LENGTH);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim==(&htim2))
  {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0U);
    LPUART1_Rx_flg = 1;
    HAL_TIM_Base_Stop_IT(&htim2);//关闭定时器
  }
	  if(htim==(&htim3))
  {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0U);
    UART2_Rx_flg = 1;
    HAL_TIM_Base_Stop_IT(&htim3);//关闭定时器
  }
	if(htim==(&htim6))
	{
		   HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //反转PA5 LED灯的状态
	}
}

/*周期唤醒回调函数*/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    if(HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK)
    {
        HAL_RTC_GetDate(hrtc, &sDate,  RTC_FORMAT_BIN);
//        char str[22];
//        sprintf(str,"RTC Time= %2d:%2d:%2d\r\n",sTime.Hours,sTime.Minutes,sTime.Seconds);
//        printf("%s", str);
			Sendover_Callback();
    }
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); //反转PA5 LED灯的状态
}
   

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_LPUART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&hlpuart1,(uint8_t *)LPUART1_temp,REC_LENGTH);
	HAL_UART_Receive_IT(&huart2,(uint8_t *)UART2_temp,REC_LENGTH);
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0, 0);
	Write_RTCTime();
//	EC200A_ReStart();
//	HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//main() while(1)
    if(LPUART1_Rx_flg)
    {
      HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
      for(int i = 0;i<LPUART1_Rx_cnt;i++)
        LPUART1_Rx_Buf[i] = 0;
      LPUART1_Rx_cnt = 0;
      LPUART1_Rx_flg = 0;
    }   
		    if(UART2_Rx_flg)
    {
      HAL_UART_Transmit(&hlpuart1,UART2_Rx_Buf,UART2_Rx_cnt,0xffff);    //发送接收到的数据
      for(int i = 0;i<UART2_Rx_cnt;i++)
        UART2_Rx_Buf[i] = 0;
      UART2_Rx_cnt = 0;
      UART2_Rx_flg = 0U;
    }   
		
		if(Button_flg)
		{
			EC200A_Init();
			Button_flg=0;
		}
		HAL_Delay(500);

//		HAL_GPIO_TogglePin(LD2_GPIO_Port, GPIO_PIN_5); 
//		HAL_Delay(500);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_HSI;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
