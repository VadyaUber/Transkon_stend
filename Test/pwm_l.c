#include "pwm_l.h"

#define ch_test_pwm 10 // количество попыток

uint8_t counter_pwm = 0; //счетрчик движения по алгоритму проверки

void reset_mod_pwm()
{
	reset_all_relays();
	set_relay_state(SEL2, ON); //подключаем in pwm
	//Отключаем
	set_relay_state(IO0, OFF);
	set_relay_state(IO1, OFF);
	set_relay_state(IO2, ON);
	set_relay_state(IO3, ON);
	set_relay_state(IO4, ON);
	set_relay_state(IO5, ON);
	set_relay_state(IO6, ON);
	set_relay_state(IO7, ON);
	while(1)
	{
		send_transkon("MODE=2;", 7); 
		uint8_t Mode = 0;
		if (sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 2) break;
	}
	counter_pwm ++;
}

void individual_test_pwm_generic(RelayIndex pin, uint8_t pwmIndexTarget, uint16_t pwmFreqTarget, uint8_t pwmDutyTarget, uint8_t flag_next)
{
	reset_all_relays();
	set_relay_state(SEL2, ON); //подключаем in pwm
	//Отключаем
	set_relay_state(IO0, OFF);
	set_relay_state(IO1, OFF);
	set_relay_state(IO2, ON);
	set_relay_state(IO3, ON);
	set_relay_state(IO4, ON);
	set_relay_state(IO5, ON);
	set_relay_state(IO6, ON);
	set_relay_state(IO7, ON);
	
	if(flag_next == 0)
	{
		send_transkon("MODE=2;", 7); 
		uint8_t Mode = 0;
		if (sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 2)
		{
			flag_next++;
		}
	}
	
	if(flag_next == 1)
	{
    set_relay_state(pin, OFF);
	
		// Формируем команду для отправки
		char command[20];
		snprintf(command, sizeof(command), "PWM=%d,%d,%d;", pwmIndexTarget, pwmFreqTarget, pwmDutyTarget);
		send_transkon(command, strlen(command));
	
		int pwmIndex, pwmFreq, pwmDuty;
		if (sscanf((char *)rx_buffer, "PWM = %d,%d,%d", &pwmIndex, &pwmFreq, &pwmDuty) == 3)
    {
			if (pwmIndex == pwmIndexTarget && pwmFreq == pwmFreqTarget && pwmDuty == pwmDutyTarget)
			{			
				while(str8.flag_diagnostic_start_stop)
				{
					if(count_main >= 499 && count_main <= 501) 
					{
						GOOD_LED_ON();// мигание светодиодом 
						set_led_state(ERROR_Led, OFF);
					}
					else
					{
						set_led_state(ERROR_Led, ON);
					}	
				}
				vTaskDelay(250);
			}
		}
	}
	
	if(flag_next != 1)
	{
		str8.flag_isError = 1;
	}
	
	reset_all_relays();
	str0.flag_test = 0;
}


void test_pwm_generic(RelayIndex pin, uint8_t pwmIndexTarget, uint16_t pwmFreqTarget, uint8_t pwmDutyTarget, uint8_t ofset)
{
    set_relay_state(pin, OFF);
	
		// Формируем команду для отправки
		char command[20];
		snprintf(command, sizeof(command), "PWM=%d,%d,%d;", pwmIndexTarget, pwmFreqTarget, pwmDutyTarget);
		send_transkon(command, strlen(command));
	
		int pwmIndex, pwmFreq, pwmDuty;
		if (sscanf((char *)rx_buffer, "PWM = %d,%d,%d", &pwmIndex, &pwmFreq, &pwmDuty) == 3)
    {
			if (pwmIndex == pwmIndexTarget && pwmFreq == pwmFreqTarget && pwmDuty == pwmDutyTarget)
			{
				for (uint8_t attempt = 0; attempt < ch_test_pwm; attempt++) 
				{
					if (count_main >= 499 && count_main <= 501)
					{
							str6.val |= 1 << ofset;  // Устанавливаем флаг для соответствующего PWM
							ModbusRegister[75+ofset] = count_main;
							set_relay_state(pin, ON);
							vTaskDelay(50);
							break;
					}
					vTaskDelay(250);
				}
				counter_pwm++;
			}
		}
}

void individual_test_1_pwm(uint8_t flag_next)
{
	reset_all_relays();
	set_relay_state(SEL2, ON); //подключаем in pwm
	//Отключаем
	set_relay_state(IO0, OFF);
	set_relay_state(IO1, ON);
	set_relay_state(IO2, ON);
	set_relay_state(IO3, ON);
	set_relay_state(IO4, ON);
	set_relay_state(IO5, ON);
	set_relay_state(IO6, ON);
	set_relay_state(IO7, ON);
	
	if(flag_next == 0)
	{
		send_transkon("MODE=2;", 7); 
		uint8_t Mode = 0;
		if (sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 2)
		{
			flag_next++;
		}
	}
	
	if(flag_next == 1)
	{
		send_transkon("PWM=1,500,50;", 13);
		int pwmIndex, pwmFreq, pwmDuty;
		if (sscanf((char *)rx_buffer,"PWM = %d,%d,%d", &pwmIndex, &pwmFreq, &pwmDuty) == 3)
		{
			if (pwmIndex == 1 && pwmFreq == 500 && pwmDuty == 50)
			{
				while(str8.flag_diagnostic_start_stop)
				{
					if(count_main >= 499 && count_main <= 501) 
					{
						GOOD_LED_ON();// мигание светодиодом 
						set_led_state(ERROR_Led, OFF);
					}
					else
					{
						set_led_state(ERROR_Led, ON);
					}	
				}
				vTaskDelay(250);
			}
		}
	}
	
	if(flag_next != 1)
	{
		str8.flag_isError = 1;
	}
	
	reset_all_relays();
	str0.flag_test = 0;
}

void test_1_pwm()
{
	set_relay_state(IO1, ON);
	send_transkon("PWM=1,500,50;", 13);
	int pwmIndex, pwmFreq, pwmDuty;
	if (sscanf((char *)rx_buffer,"PWM = %d,%d,%d", &pwmIndex, &pwmFreq, &pwmDuty) == 3)
	{
		if (pwmIndex == 1 && pwmFreq == 500 && pwmDuty == 50)
		{
			for (uint8_t attempt = 0; attempt < ch_test_pwm; attempt++) 
			{
				if(count_main >= 499 && count_main <=501)
				{
					str6.pwm_1 = 1;
					ModbusRegister[76] = count_main;
					set_relay_state(IO1, OFF);
					vTaskDelay(50);
					break;
				}
				vTaskDelay(250);
			}
			counter_pwm++;
		}
	}
}

void individual_test_0_pwm(uint8_t flag_next)
{
	reset_all_relays();
	set_relay_state(SEL2, ON); //подключаем in pwm
	//Отключаем
	set_relay_state(IO0, ON);
	set_relay_state(IO1, OFF);
	set_relay_state(IO2, ON);
	set_relay_state(IO3, ON);
	set_relay_state(IO4, ON);
	set_relay_state(IO5, ON);
	set_relay_state(IO6, ON);
	set_relay_state(IO7, ON);
	
	if(flag_next == 0)
	{
		send_transkon("MODE=2;", 7); 
		uint8_t Mode = 0;
		if (sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 2)
		{
			flag_next++;
		}
	}
	
	if(flag_next == 1)
	{
		send_transkon("PWM=0,500,50;", 13);
		int pwmIndex, pwmFreq, pwmDuty;
		if (sscanf((char *)rx_buffer,"PWM = %d,%d,%d", &pwmIndex, &pwmFreq, &pwmDuty) == 3)
		{
			if (pwmIndex == 0 && pwmFreq == 500 && pwmDuty == 50)
			{				
				while(str8.flag_diagnostic_start_stop)
				{
					if(count_main >= 499 && count_main <= 501) 
					{
						GOOD_LED_ON();// мигание светодиодом 
						set_led_state(ERROR_Led, OFF);
					}
					else
					{
						set_led_state(ERROR_Led, ON);
					}	
				}
				vTaskDelay(250);
			}
		}
	}
	
	if(flag_next != 1)
	{
		str8.flag_isError = 1;
	}
	
	reset_all_relays();
	str0.flag_test = 2;
}

void test_0_pwm()
{
	set_relay_state(IO0, ON);
	send_transkon("PWM=0,500,50;", 13);
	int pwmIndex, pwmFreq, pwmDuty;
	if (sscanf((char *)rx_buffer,"PWM = %d,%d,%d", &pwmIndex, &pwmFreq, &pwmDuty) == 3)
	{
		if (pwmIndex == 0 && pwmFreq == 500 && pwmDuty == 50)
		{
			for (uint8_t attempt = 0; attempt < ch_test_pwm; attempt++) 
			{
				if(count_main >= 499 && count_main <=501)
				{
					str6.pwm_0 = 1;
					ModbusRegister[75] = count_main;
					set_relay_state(IO0, OFF);
					vTaskDelay(50);
					break;
				}
				vTaskDelay(250);
			}
			counter_pwm++;
		}
	}
}



void test_pwm()
{
	switch (counter_pwm) 
	{
		case 0:
			reset_mod_pwm();
			break;
		
		case 1:
			test_pwm_generic(IO7, 7, 500, 50, 7); // Тестируем 7-й пин
			break;
		
		case 2:
			  test_pwm_generic(IO6, 6, 500, 50, 6); // Тестируем 6-й пин
			break;
		
		case 3:
				test_pwm_generic(IO5, 5, 500, 50, 5); // Тестируем 5-й пин
			break;
		
		case 4:
				test_pwm_generic(IO4, 4, 500, 50, 4); // Тестируем 4-й пин
			break;
		
		case 5:
				test_pwm_generic(IO3, 3, 500, 50, 3); // Тестируем 3-й пин
			break;
		
		case 6:
				test_pwm_generic(IO2, 2, 500, 50, 2); // Тестируем 2-й пин
			break;
		
		case 7:
				test_1_pwm();
			break;
		
		case 8:
				test_0_pwm();
			break;
		
		case 9:
			//	counter_pwm = 0;
				ModbusRegister[14] = str6.val;
				counter++;
			break;
	}	
}
