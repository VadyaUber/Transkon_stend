#include "uart_.h"

#define BUFFSIZE 255

uint8_t SLAVEID = 1;

uint8_t rx_buffer[rx_buffer_size] = {0,};		//данные с транскона по uart 
uint16_t check1 = 0;
uint16_t check4 = 0;

char real_sbor_buffer[BUFFSIZE] = {0};
char my_sbor_buffer[BUFFSIZE] = {0};

void send_transkon(char *data, uint8_t size)  //отправка транскон 
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_AbortReceive(&huart1);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, rx_buffer_size); 
	
	HAL_GPIO_WritePin(Transcon_UART1_EN_GPIO_Port, Transcon_UART1_EN_Pin, GPIO_PIN_RESET); 	//поднимаем 1 
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)data, size);
	vTaskDelay(250);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)  //по окончанию передачи
{
	if(huart == &huart1) HAL_GPIO_WritePin(Transcon_UART1_EN_GPIO_Port, Transcon_UART1_EN_Pin, GPIO_PIN_SET); //MUDBUS TRANSCON
	if(huart == &huart2) HAL_GPIO_WritePin(PC_UART2_EN_GPIO_Port, PC_UART2_EN_Pin, GPIO_PIN_RESET);  //MUDBUS PC  
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) //данные приняты 
{
	if(huart == &huart1)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, rx_buffer_size);
	}
	
	if(huart == &huart2) //MUDBUS PC  
	{
		check1++;
		uint16_t CRC_summ = 0;
		uint16_t rxCRC = 0;
//		static uint32_t tim_kley = 0;
		//uint8_t index_end_buff = 0;
			 	
		uartTimeCounter = 0;
		
		//DataCounter = sizeof(ModbusRx); // - DMA1_Channel6->CNDTR;
		//HAL_UARTEx_ReceiveToIdle_DMA(&huart2,(uint8_t *)ModbusRx,sizeof(ModbusRx)); //прием данных DMA 
		//index_end_buff = sizeof(sbor_buffer) - 1;
		//index_end_buff = MY_BUFFSIZE - 1;
		
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2,(uint8_t *)my_sbor_buffer,sizeof(my_sbor_buffer)); //прием данных DMA
		
		//status = HAL_UART_Receive(&huart2,(uint8_t *)real_sbor_buffer,sizeof(sbor_buffer),100);
		//uint8_t sch_null = 0;
		//static uint8_t j = 0;
		//uint8_t per = 0;
		//uint8_t flag_pack = 0;
		//uint8_t ff = 0;
			
		//HAL_Delay(1);
		memcpy(real_sbor_buffer, my_sbor_buffer, BUFFSIZE);
		
		//выделение сообщения
		//поиск ID - длина команды всегда 8

		for(uint8_t k = 0; k < BUFFSIZE; k++)
		{
			if(real_sbor_buffer[k] == SLAVEID)
			{
				char vr[8] = {0};
				for(uint8_t y = 0; y < 8; y++)
				{
					vr[y] = real_sbor_buffer[k + y];	
				}
				if(vr[3] == 0x08 && vr[1] == 0x06)
				{
					vr[0] = 0x01;
				}
				CRC_summ = MODBUS_CRC16(vr, 6);
				rxCRC = (vr[7] << 8) | (vr[6]);
				if(CRC_summ == rxCRC)
					{
						transmitDataMake(vr,8); // отправка в либу модабаса
						memset(my_sbor_buffer, 0, sizeof(sbor_buffer));
						flag_pc_connect = HAL_GetTick(); //обновление времени подключения к пк
						//uartPacketComplatedFlag = SET;
					}	
			}
			
		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) 
{
	if(huart == &huart2) //MUDBUS PC  
	{
		if (huart->ErrorCode & HAL_UART_ERROR_NONE) 	/* 0x00000000U   !< Нет ошибки           */ 
		{

		}
		
		if (huart->ErrorCode & HAL_UART_ERROR_PE) 	  /* 0x00000001U   !< Ошибка четности       */
		{

		}
		
		if (huart->ErrorCode & HAL_UART_ERROR_NE) 	  /* 0x00000002U   !< Шумовая ошибка       */
		{
			__HAL_UART_FLUSH_DRREGISTER(&huart2);
			HAL_UART_AbortReceive(&huart2);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart2,(uint8_t *)ModbusRx,sizeof(ModbusRx));
		}

		if (huart->ErrorCode & HAL_UART_ERROR_FE) 		/* 0x00000004U   !< Ошибка кадра        */
		{
			__HAL_UART_FLUSH_DRREGISTER(&huart2);
			HAL_UART_AbortReceive(&huart2);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart2,(uint8_t *)ModbusRx,sizeof(ModbusRx));
		}
	}
	
	if(huart == &huart1) //MUDBUS TRANSCON
	{
		if (huart->ErrorCode & HAL_UART_ERROR_NONE) 	// 0x00000000U   !< Нет ошибки        
		{

		}
		
		if (huart->ErrorCode & HAL_UART_ERROR_PE) 	  // 0x00000001U   !< Ошибка четности       
		{

		}
		
		if (huart->ErrorCode & HAL_UART_ERROR_NE) 	  // 0x00000002U   !< Шумовая ошибка       
		{

		}

		if (huart->ErrorCode & HAL_UART_ERROR_FE) 		// 0x00000004U   !< Ошибка кадра        
		{
			//__HAL_RCC_USART1_CLK_ENABLE();
			//__HAL_RCC_USART1_FORCE_RESET();
			//__HAL_RCC_USART1_RELEASE_RESET();

			//HAL_UART_AbortReceive_IT(&huart1);
			//HAL_GPIO_WritePin(EN_UART_GPIO_Port, EN_UART_Pin, GPIO_PIN_SET); 

			__HAL_UART_FLUSH_DRREGISTER(&huart1);
			HAL_UART_AbortReceive(&huart1);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, rx_buffer_size); 
			//HAL_UART_Receive_DMA(&huart1, rx_buffer, rx_buffer_size); 
			
			
		}
	}
}

