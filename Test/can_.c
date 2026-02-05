#include "can_.h"



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint32_t wdcan = 0;
	
  CAN_RxHeaderTypeDef msgHeader;

  uint8_t msgData[8];
  
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &msgHeader, msgData)== HAL_OK)
	{
		str0.can = 1;
		//ModbusRegister[8] = str0.val;
		/*
		if(msgHeader.ExtId == 0x18FFC2A9 && msgHeader.DLC == 8 && str0.can == 0)		//if ID = 0x18FFC2A9 and lenght = 9
		{
			if(wdcan < 0xFFFFFFFF)
			{
				wdcan++; //пришел пакет 
				str0.can = 1; //can исправен
				//ModbusRegister[8] = str0.val;
			}
		}
		else
		{
			wdcan=0;
			str0.can = 0;
			//reset();
			//obnull();
			//ModbusRegister[8] = str0.val;
		}
		*/
	}
	else
	{
		str0.can = 0;
		//reset();
		//obnull();
		//ModbusRegister[8] = str0.val;
	}
    //__NOP();

}




void test_can()
{
	send_transkon("SEND=11;", 8); //запрос can

	uint8_t _can = 0;
	if (sscanf((char *)rx_buffer,"can = %hhd", &_can) == 1)
	{
		if (_can == 1) 
		{
			counter++;
			//str0.can = 1; //can исправен 
		}
	}
}

void individual_test_can()
{
	while(str8.flag_diagnostic_start_stop) 
	{
		send_transkon("SEND=11;", 8); //запрос can
		
		uint8_t _can = 0;
		if (sscanf((char *)rx_buffer,"can = %hhd", &_can) == 1)
		{
			if(_can == 1)
			{
				GOOD_LED_ON();// мигание светодиодом 
				set_led_state(ERROR_Led, OFF);
			}
			else
			{
				set_led_state(ERROR_Led, ON);
			}				
		}
	}
	str0.flag_test = 0;
} 

void read_can()
{
	uint8_t soxr = 0;
	uint8_t mass_read_can[100] = {0};
	
	for(uint8_t i = 0; i < 100; i++)
	{
		mass_read_can[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
		HAL_Delay(1);
		if (mass_read_can[i] != 1)
		{
			soxr = mass_read_can[i];
			break;
		}
	}
	
	if(soxr == 0)
	{
		str0.can = 1;
		ModbusRegister[8] = str0.val;
	}
	else
	{
		str0.can = 0;
		ModbusRegister[8] = str0.val;
	}
	
}
/*
void test_can_permach()
{
	send_transkon("SEND=11;", 8); //запрос can

	uint8_t _can = 0;
	if (sscanf((char *)rx_buffer,"can = %hhd", &_can) == 1)
	{
		if (_can == 1) 
		{
			str0.can = 1; //can исправен 
		}
		else
		{
			str0.can = 0; //can исправен 
		}
	}
}*/

void setup_can()
{
	CAN_FilterTypeDef canFilterConfig;
	canFilterConfig.FilterBank = 0;
	canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfig.FilterIdHigh = 0;//CAN_InputControlMessageID<<5;		//Input Message IDs
	canFilterConfig.FilterIdLow = 0;
	canFilterConfig.FilterMaskIdHigh = 0;//0x7f0<<5;										//Inpun message mask
	canFilterConfig.FilterMaskIdLow = 0;
	canFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfig.FilterActivation = ENABLE;
	canFilterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan, &canFilterConfig);
    
  HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

