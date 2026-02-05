#include "functions.h"

uint16_t check3 = 0;

str_0 str0 = {0};
str_1 str1 = {0};
str_2 str2 = {0};
str_3 str3 = {0};
str_4 str4 = {0};
str_5 str5 = {0};
str_6 str6 = {0};
str_7 str7 = {0};
str_8 str8 = {0};
str_9 str9 = {0};

str_8 temp_str2 = {0};
str_9 temp_str3 = {0};

uint8_t flag_work = 0; // флаг, обозначающий процесс выполнения теста
uint8_t flag_full_test = 0; //флаг, готовности полного теста
uint8_t flag_ind_test_req = 0; //флаг готовности индивидуальных тестов
uint8_t flag_reset = 0;
uint8_t flag_otrub_can = 0;

uint8_t ch_can = 0;

uint8_t counter = 0; //счетчик двежения по циклу 

uint8_t my_flag_test = 0;

bool connect_to_pc = 0;

void end_test(void);

void status_KZ()
{
	static uint32_t kz_Timer;
	str0.kz = HAL_GPIO_ReadPin (KZ_GPIO_Port, KZ_Pin); 
	if(str0.kz)
	{
		set_led_state(KZ_Led, ON);
		reset();
		obnull();
		ModbusRegister[8] = str0.val;
		if(HAL_GetTick() - kz_Timer >= 5000)
		{
			kz_Timer = HAL_GetTick();
			toggle_relay_state(KZ); //переключаем реле 
		}
	}
	else 
	{
		set_led_state(KZ_Led, OFF);
		set_relay_state(KZ, OFF);
	}
}

void update_status_test()
{
	uint16_t val_ = counter + counter_temp + counter_ai + counter_io + counter_pwm; 
	str7.counter_now = val_;
	ModbusRegister[7] = str7.val;
	check3++;
}


void test()
{
	if(str0.kz != 1)
	{
			if (str0.flag_test == 1 && flag_work == 1) 
			{
				str8.flag_diagnostic_ready = 0;
				str8.flag_isTesting = 1;
				switch(counter) 
				{	
					case 0:
						TOGGLE_STATUS_LED();
						test_485();
						//counter++;
						break;
					
					case 1:
						TOGGLE_STATUS_LED();
						flag_otrub_can = 1;
						test_can();
						//counter++;
						break;
					
					case 2:
						TOGGLE_STATUS_LED();
						flag_otrub_can = 0;
						test_temp();
						//counter++;
						break;
					
					case 3:
						TOGGLE_STATUS_LED();
						test_ai();
						//counter++;
						break;
					
					case 4:
						TOGGLE_STATUS_LED();
						test_io();
						//counter++;
						break;
					
					case 5:
						TOGGLE_STATUS_LED();
						test_pwm();
						//counter++;
						break;
					
					case 6:
						end_test();
						break;
				}
			}	
	
		if (str8.flag_diagnostic_start_stop == 1 && flag_work == 1) //условие для выполнения индивидуальных тестов
		{
					individual_diagnostic();// функция старта индивидуального теста
					if(str8.flag_isError != 1)
					{
						GOOD_LED_OFF();
						set_led_state(ERROR_Led, OFF);
						
						flag_work = 0;
						flag_ind_test_req = 0;
						str8.counter_test = 0;
						str8.flag_isTesting = 0;
						str8.flag_diagnostic_ready = 1;
						ModbusRegister[83] = str8.val;
					}
		}
	/*
	if(str0.flag_test == 3)
	{
		GOOD_LED_ON();
	}
	*/
	}
}


void reset()
{
	//flag_ignor_kz = 0;
	//set_relay_state(KZ, ON);
	//vTaskDelay(1500);
	//set_relay_state(KZ, OFF);
	//str0.flag_test_end = 0;
	str0.val = 0; 
	str1.val = 0; 
	str2.val = 0; 
	str3.val = 0; 
	str4.val = 0; 
	str5.val = 0; 
	str6.val = 0;
	str8.val = 0;
	str9.val = 0;
	
	counter = 0;
	counter_temp = 0;
	counter_ai = 0;
	counter_io =0;
	counter_pwm = 0; 
	
	set_led_state(ERROR_Led, OFF);
	//set_led_state(GOOD_Led, OFF);
	GOOD_LED_OFF();
	GOOD_LED_OFF();
}

void obnull() //обнуление значений всех регистров модбаса
{
	str1.val = 0;
	ModbusRegister[9] = str1.val;
	str2.val = 0;
	ModbusRegister[10] = str2.val;
	str3.val = 0;
	ModbusRegister[13] = str3.val;
	str4.val = 0;
	ModbusRegister[11] = str4.val;
	str5.val = 0;
	ModbusRegister[12] = str5.val;
	str6.val = 0;
	ModbusRegister[14] = str6.val;
	str7.counter_now = 0;
	ModbusRegister[7] = str7.val;
	
	counter = 0;
	counter_temp = 0;
	counter_ai = 0;
	counter_io = 0;
	counter_pwm = 0;
	
	flag_work = 0;
	flag_ind_test_req = 0;
	flag_full_test = 0;
	
	GOOD_LED_OFF();
	set_led_state(ERROR_Led, OFF);
}

void end_test()
{
	/*
			нет ошибок в конце тоста 
			Зеленый - 1 
			Красный - 0  
			
			Если есть ошибка 
			Зеленый - 0
			Красный - 1
			*/
			
			if (str0.Transkon_485 == 1 && str0.can == 1  && str0.kz == 0 && str1.val == 65535 && str2.val == 15 && str3.val == 511 && str4.val == 65535 && str5.val == 65535 && str6.val == 255)
			{
				GOOD_LED_ON();
			//	set_led_state(GOOD_Led, ON);
				set_led_state(ERROR_Led, OFF);
				str0.good = 1;
				str0.error = 0;
				
			}
			else 
			{
				str0.error = 1;
				str0.good = 0;
				//set_led_state(GOOD_Led, OFF);
				GOOD_LED_OFF();
				set_led_state (ERROR_Led, ON);
			}
			
			str0.flag_test = 0; //конец теста
			ModbusRegister[8] = str0.val;
			flag_full_test = 0;
			flag_work = 0;
			//str8.counter_test = 56;
}

void check_start_req() // требования для старта тестов
{
		str_0 temp_str = {0};
		temp_str.val = ModbusRegister[8]; //заполняем структуру
		temp_str2.val = ModbusRegister[83];
		
		if (ModbusRegister[0] != SLAVEID) SLAVEID = ModbusRegister[0]; //смена SLAVEID в 0 регистре
	
		if(flag_work == 0)
		{
			if((str0.flag_test == 0 || str0.flag_test == 2) && (str8.flag_diagnostic_start_stop == 0))
			{
				str8.flag_diagnostic_ready = 1;
				str8.flag_isTesting = 0;
				flag_full_test = 1;
			}
			
			if((temp_str2.counter_test != 0) && (temp_str2.counter_test < 71))
			{
				str8.counter_test = temp_str2.counter_test;
			}
				
			if((str8.counter_test != 0) && (str8.counter_test < 71) && (str8.flag_diagnostic_ready == 1))
			{
				flag_ind_test_req = 1;
			}
		}
		
		if(temp_str.reset_status == 1) //команда на сброс
		{
			str0.reset_status = 0;
			str0.good = 0;
			str0.error = 0;
			//reset();
			ModbusRegister[8] = str0.val;
		}
			
		if(temp_str.reset_test == 1) //получена команда на сброс
		{
			str0.reset_test = 2; //подтверждаем
			ModbusRegister[8] = str0.val; //записываем
			set_relay_state(KZ, ON);
			vTaskDelay(4000);
			set_relay_state(KZ, OFF);
			//str0.reset_test = 0;
			reset();
			obnull();
			ModbusRegister[8] = str0.val; 
		}
		
}

void check_start()
{
	if(flag_work == 0)
	{
		if(flag_full_test == 1)
		{
			str_0 temp_str = {0};
			temp_str.val = ModbusRegister[8];
			
			if(temp_str.flag_test == 1 || temp_str.flag_test == 3)
			{
				str0.flag_test = 1;
				str0.error = 0;
				str0.good = 0;
				str0.error = 0;
				ModbusRegister[8] = str0.val;
			}
			
			if(str0.flag_test == 1)
			{
				flag_ind_test_req = 0;
				obnull();
				flag_work = 1;
				flag_full_test = 1;
			}
		}
	}
	
		if(flag_ind_test_req == 1)
		{		
			str_8 temp_str = {0};
			temp_str.val = ModbusRegister[83];
			
			if(temp_str.flag_diagnostic_start_stop == 1)
			{
				str8.flag_diagnostic_start_stop = 1;
				flag_work = 1;
				flag_full_test = 0;
				ModbusRegister[8] = str8.val;
			}
			else
			{
				str8.flag_diagnostic_start_stop = 0;
				ModbusRegister[8] = str8.val;
			}
			
			if(str8.flag_isError == 1)
			{
				str8.flag_diagnostic_start_stop = 0;
				str8.counter_test = 0;
				flag_work = 0;
				flag_ind_test_req = 0;
				str8.flag_isError = 0;
				ModbusRegister[83] = str8.val;
			}
			
		}
			
		if((str0.can == 0) && ((str0.flag_test == 1) || (str8.flag_diagnostic_start_stop == 1)))
		{
			if(flag_otrub_can != 1)
			{
				static uint8_t kk = 0;
				static uint32_t TimOtrub = 0;
				setup_can();
				
				if(str0.can == 0)
				{
					if((HAL_GetTick() - TimOtrub > 15000) && (kk == 1))
					{
						kk = 2;
						TimOtrub = HAL_GetTick();
					}
					if(kk == 0)
					{
						TimOtrub = HAL_GetTick();
						kk = 1;
					}
					
					if((kk == 2) && (HAL_GetTick() - TimOtrub > 1500))
					{
						//reset();
						//obnull();
						ch_can++;
					}
					
				}
			}
		}
		
		ModbusRegister[8] = str0.val; //постоянно обновляем инфу о кз (+ 485 can flag-проверки)
		ModbusRegister[83] = str8.val;
		update_status_test(); //прогрес выполнения проверки для индикации на пк
}

void my_connect_to_pc_485()
{
	
	check_start_req();
	check_start();
		//flag_reset = 0;
	
	if(connect_to_pc == 0)
	{
		button_start();
		button_reset();
	}
}

void PC_Connect()
{
	if(HAL_GetTick() - flag_pc_connect >= 1500)
	{
		HAL_GPIO_WritePin(CONNECT_Led_GPIO_Port, CONNECT_Led_Pin, GPIO_PIN_RESET);
		connect_to_pc = 0;
	}
	else
	{
		HAL_GPIO_WritePin(CONNECT_Led_GPIO_Port, CONNECT_Led_Pin, GPIO_PIN_SET);
		connect_to_pc = 1;
	}
}
