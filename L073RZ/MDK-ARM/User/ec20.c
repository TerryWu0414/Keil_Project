#include "ec20.h"

char *strx,*extstrx;
uint8_t i=0;
void EC200A_Init(void)
{
    UART2_SendString("TCP/IP协议初始化...\r\n");
		HAL_Delay(300);                                                            
    LPUART1_SendString("AT+CPIN?\r\n");                                          //获取卡号，类似是否存在卡的意思，比较重要。
    HAL_Delay(500);                                                             
    strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CPIN: READY");                //返回OK，表明识别到卡了
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据	
    while(strx==NULL&&i<4)                                                          
    {    
				i++;
        Clear_Buffer();                                                        
        LPUART1_SendString("AT+CPIN?\r\n");                                      //获取卡号，类似是否存在卡的意思，比较重要。
        HAL_Delay(500);                                                         
        strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CPIN: READY");            //返回OK,说明卡是存在的
				HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    }  
			if(i==4)
			{
				UART2_SendString("TCP/IP协议初始化失败！\r\n");
				Clear_Buffer();
				return;
			}
		i=0;
		Clear_Buffer(); 																																				     
    LPUART1_SendString("AT+CREG?\r\n");                                         //查看是否注册GSM网络                     
    HAL_Delay(500);                                                             
    strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CREG: 0,1");         //返回正常
    extstrx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CREG: 0,5");      //返回正常，漫游
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    while(strx==NULL&&extstrx==NULL&&i<4)                                           
    {   
				i++;
        Clear_Buffer();                                                        
        LPUART1_SendString("AT+CREG?\r\n");                                     //查看是否注册GSM网络                 
        HAL_Delay(500);                                                         
        strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CREG: 0,1");     //返回正常
        extstrx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CREG: 0,5");  //返回正常，漫游
				HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    }
			if(i==4)
			{
				UART2_SendString("TCP/IP协议初始化失败！\r\n");
				Clear_Buffer();
				return;
			}
		i=0;
    Clear_Buffer();

    LPUART1_SendString("AT+CGREG?\r\n");                                        //查看是否注册GPRS网络
    HAL_Delay(500);
    strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CGREG: 0,1");        //这里重要，只有注册成功，才可以进行GPRS数据传输。
    extstrx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CGREG: 0,5");     //返回正常，漫游
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    while(strx==NULL&&extstrx==NULL&&i<4)
    {
				i++;
        Clear_Buffer();
        LPUART1_SendString("AT+CGREG?\r\n");                                    //查看是否注册GPRS网络
        HAL_Delay(500);
        strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CGREG: 0,1");    //这里重要，只有注册成功，才可以进行GPRS数据传输。
        extstrx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+CGREG: 0,5"); //返回正常，漫游
				HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    }
			if(i==4)
			{
				UART2_SendString("TCP/IP协议初始化失败！\r\n");
				Clear_Buffer();
				return;
			}
		i=0;
    Clear_Buffer();
    LPUART1_SendString("AT+COPS?\r\n");                                         //查看注册到哪个运营商，支持移动 联通 电信
    HAL_Delay(500);
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    Clear_Buffer();
    LPUART1_SendString("AT+QICLOSE=0\r\n");                                     //关闭socket连接
    HAL_Delay(500);
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    Clear_Buffer();
    LPUART1_SendString("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n");              //接入APN，无用户名和密码
    HAL_Delay(500);
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    Clear_Buffer();
    LPUART1_SendString("AT+QIACT=1\r\n");                                       //激活
    HAL_Delay(500);
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    Clear_Buffer();
    LPUART1_SendString("AT+QIACT?\r\n");                                        //获取当前卡的IP地址
    HAL_Delay(500);
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    Clear_Buffer();
		LPUART1_SendString("AT+QIOPEN=1,0,\"TCP\",\"121.43.55.240\",52487,0,1\r\n");                                        //获取当前卡的IP地址
    HAL_Delay(500);
		strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+QIOPEN:");                //返回OK，表明识别到卡了
		HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
                                                               
    while(strx==NULL&&i<4)                                                          
    {               
				i++;
        Clear_Buffer();                                                        
        LPUART1_SendString("AT+CPIN?\r\n");                                      //获取卡号，类似是否存在卡的意思，比较重要。
        HAL_Delay(500);                                                         
        strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"+QIOPEN:");            //返回OK,说明卡是存在的
				HAL_UART_Transmit(&huart2,LPUART1_Rx_Buf,LPUART1_Rx_cnt,0xffff);    //发送接收到的数据
    } 
				if(i==4)
			{
				UART2_SendString("TCP/IP协议初始化失败！\r\n");
				Clear_Buffer();
				return;
			}
		i=0;
    Clear_Buffer();
		
}

void EC200A_ReStart(void)
{
		UART2_SendString("EC20正在重启...\r\n");
		LPUART1_SendString("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n");              //接入APN，无用户名和密码
    HAL_Delay(500);
		LPUART1_SendString("AT+QPOWD\r\n");                                        //获取当前卡的IP地址
    HAL_Delay(2000);
		UART2_SendString("...");
		strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"RDY");                //返回OK，表明识别到卡了                                                        
    while(strx==NULL)                                                          
    {                                                                          
        Clear_Buffer();                                                        
        LPUART1_SendString("AT+QPOWD\r\n");                                      //获取卡号，类似是否存在卡的意思，比较重要。
        HAL_Delay(2000);  
				UART2_SendString("...");			
        strx=strstr((const char*)LPUART1_Rx_Buf,(const char*)"RDY");            //返回OK,说明卡是存在的

    } 
}