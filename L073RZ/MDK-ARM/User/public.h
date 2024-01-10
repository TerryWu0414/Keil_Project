#ifndef __PUBLIC_H_
#define __PUBLIC_H_

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "rtc.h"
#include "tim.h"
#include "gpio.h"
#include "string.h"
#include "stdio.h"

void LPUART1_SendString(char* String);
void UART2_SendString(char* String);
void Clear_Buffer(void);

#endif
