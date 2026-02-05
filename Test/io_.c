#include "io_.h"

/*io4-7 mode0*/

#define ch_test_io 10 // количество попыток

#define pwm_0 2399 //0v PWM

uint8_t counter_io = 0; //счетрчик движения по алгоритму проверки

#ifdef NOMBER_Sten_1
uint16_t pwm_4_5v = 1946; 
uint16_t pwm_7_5v = 1660;  
uint16_t pwm_24v = 0;
#endif 


#ifdef NOMBER_Sten_2
uint16_t pwm_4_5v = 1961; 
uint16_t pwm_7_5v = 1679;  
uint16_t pwm_24v = 0;
#endif 

#ifdef NOMBER_Sten_3
uint16_t pwm_4_5v = 1952; 
uint16_t pwm_7_5v = 1665;  
uint16_t pwm_24v = 0;
#endif 

char message_mode0_io[] = "MODE=0;";

void individual_io_test(uint16_t pwm_value, RelayIndex exclude_relay, uint8_t expected_IO[4], uint8_t flag_next)
{
	TIM8->CCR2 = pwm_0; // сброс в 0
	reset_all_relays();
	
	if(flag_next == 0) // установка мода 0 для транскона
	{
		send_transkon(message_mode0_io, sizeof(message_mode0_io));

		uint8_t Mode = 1;
		if(sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 0) 
		{
			flag_next++;
		}
	}
	
	if(flag_next == 1)
	{
		TIM8->CCR2 = pwm_value; // Устанавливаем PWM

		reset_all_relays();
		set_relay_state(SEL1, ON);
		set_relay_state(IO2, ON); //дополнительно разымакаем 
		set_relay_state(IO3, ON);

		// Включаем все реле, кроме исключенного
		RelayIndex relays[] = {IO4, IO5, IO6, IO7};
		for (uint8_t i = 0; i < 4; i++)
		{
				if (relays[i] != exclude_relay)
				{
						set_relay_state(relays[i], ON);
				}
		}

		while(str8.flag_diagnostic_start_stop)
		{
			send_transkon("SEND=2;", 7);

			uint8_t IO[4];
		
			if (sscanf((char *)rx_buffer, "In = %hhd,%hhd,%hhd,%hhd", &IO[0], &IO[1], &IO[2], &IO[3]) == 4)
			{
					// Сравниваем полученные значения с ожидаемыми
					if (IO[0] == expected_IO[0] && IO[1] == expected_IO[1] &&
							IO[2] == expected_IO[2] && IO[3] == expected_IO[3])
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
	}
	
	if(flag_next != 1)
	{
		str8.flag_isError = 1;
	}
	
	TIM8->CCR2 = pwm_0; // сброс в 0
	reset_all_relays();
	str0.flag_test = 0;
	str8.flag_isTesting = 0;
	str8.counter_test = 0;
}

void io_test(uint16_t pwm_value, RelayIndex exclude_relay, uint8_t expected_IO[4], uint8_t flag_bit)
{
    TIM8->CCR2 = pwm_value; // Устанавливаем PWM

    reset_all_relays();
    set_relay_state(SEL1, ON);
		set_relay_state(IO2, ON); //дополнительно разымакаем 
		set_relay_state(IO3, ON);


    // Включаем все реле, кроме исключенного
    RelayIndex relays[] = {IO4, IO5, IO6, IO7};
    for (uint8_t i = 0; i < 4; i++)
    {
        if (relays[i] != exclude_relay)
        {
            set_relay_state(relays[i], ON);
        }
    }

    for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
    {
        send_transkon("SEND=2;", 7);

        uint8_t IO[4];
        if (sscanf((char *)rx_buffer, "In = %hhd,%hhd,%hhd,%hhd", &IO[0], &IO[1], &IO[2], &IO[3]) == 4)
        {
            // Сравниваем полученные значения с ожидаемыми
            if (IO[0] == expected_IO[0] && IO[1] == expected_IO[1] &&
                IO[2] == expected_IO[2] && IO[3] == expected_IO[3])
            {
                str4.val |= (1 << flag_bit); // Устанавливаем бит во флаге
                break; // Успешный тест
            }
        }
    }
    counter_io++;
}

void high_level()
{
	reset_all_relays();
	set_relay_state(IO2, ON);	//размыкаем реле проверки / IO0 и IO1 NO по умолчанию
	set_relay_state(IO3, ON);
	set_relay_state(SEL2, ON); //подключаем IN PWM 
	
	send_transkon("O=1,1,1,1;", 10); 
	uint8_t v0,v1,v2,v3;
	if (sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3) == 4)
	{
		if (v0 == 1 && v1 == 1 && v2 == 1 && v3 == 1) counter_io++;
	}
}

void individual_test_high_level_IO3_IO0(uint8_t canal, uint8_t flag_next)
{
	if(flag_next == 0) // установка мода 0 для транскона
	{
		send_transkon(message_mode0_io, sizeof(message_mode0_io));

		uint8_t Mode = 1;
		if(sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 0) 
		{
			flag_next++;
		}
	}
	
	if(flag_next == 1)
	{
		reset_all_relays();
		set_relay_state(IO2, ON);	//размыкаем реле проверки IO2 и IO3 NO
		set_relay_state(IO3, ON);
		set_relay_state(SEL2, ON); //подключаем IN PWM 
		
		send_transkon("O=1,1,1,1;", 10); 
		uint8_t v0,v1,v2,v3 = 0;
		sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3);
		
		set_relay_state(SEL2, OFF); //отключаем IN PWM
			
		switch(canal)
		{
			case 3:
				if(v0 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO3, OFF); // включение IO3
					set_relay_state(IO2, ON);
					set_relay_state(IO4, ON);
					set_relay_state(IO5, ON);
					set_relay_state(IO6, ON);
					set_relay_state(IO7, ON);
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
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
				break;
	
			case 2:
				if(v1 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO2, OFF); // включение IO2
					set_relay_state(IO3, ON);
					set_relay_state(IO4, ON);
					set_relay_state(IO5, ON);
					set_relay_state(IO6, ON);
					set_relay_state(IO7, ON);
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
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
				break;
					
			case 1:
				if(v2 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO2, ON); // выключение
					set_relay_state(IO3, ON);
					set_relay_state(IO4, ON);
					set_relay_state(IO5, ON);
					set_relay_state(IO6, ON);
					set_relay_state(IO7, ON);
					
					set_relay_state(IO1, ON); // включение
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
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
				break;
					
			case 0:
				if(v3 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO2, ON); // выключение
					set_relay_state(IO3, ON);
					set_relay_state(IO4, ON);
					set_relay_state(IO5, ON);
					set_relay_state(IO6, ON);
					set_relay_state(IO7, ON);
					
					set_relay_state(IO0, ON); // включение
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
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
				break;
		}
	}
	
	if(flag_next != 2)
	{
		str8.flag_isError = 1;
	}
	else
	{
		reset_all_relays();
		
		while(1) // установка low level
		{
			send_transkon("O=0,0,0,0;", 10); 
			uint8_t v0,v1,v2,v3;
			if (sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3) == 4)
			{
				if (v0 == 0 && v1 == 0 && v2 == 0 && v3 == 0)
				{
					break;
				}
			}
		}
	}
	str0.flag_test = 0;
}

void test_high_level_IO3()
{
	set_relay_state(IO3, OFF); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
  {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str4.HL_IO3 = 1;
			set_relay_state(IO3, ON);
			break;
		}
	}
	counter_io++;
}

void test_high_level_IO2()
{
	set_relay_state(IO2, OFF); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
  {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str4.HL_IO2 = 1;
			set_relay_state(IO2, ON);
			break;
		}
	}
	counter_io++;
}

void test_high_level_IO1()
{
	set_relay_state(IO1, ON); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
  {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str4.HL_IO1 = 1;
			set_relay_state(IO1, OFF);
			break;
		}
	}
	counter_io++;
}

void test_high_level_IO0()
{
	set_relay_state(IO0, ON); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
  {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str4.HL_IO0 = 1;
			set_relay_state(IO0, OFF);
			break;
		}
	}
	counter_io++;
}

void low_level()
{
	reset_all_relays();
	while(1)
	{
		send_transkon("O=0,0,0,0;", 10); 
		uint8_t v0,v1,v2,v3;
		if (sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3) == 4)
		{
			if (v0 == 0 && v1 == 0 && v2 == 0 && v3 == 0)
			{
				break;
			}
		}
	}
	counter_io++;
}

void mode_set_1()
{
	reset_all_relays();
	while(1)
	{
		send_transkon("MODE=1;", 7); 
		uint8_t Mode = 0;
		if (sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 1) break; 
	}
	counter_io++;
}

void individual_test_io_next(uint16_t pwm_value, RelayIndex exclude_relay, uint8_t expected_IO[4], uint8_t flag_next)
{
	reset_all_relays();
	TIM8->CCR2 = pwm_0;
	
	if(flag_next == 0)
	{
			send_transkon("MODE=1;", 7); 
			uint8_t Mode = 0;
			if (sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 1)
			{	
				flag_next++;
			}
	}
	
	
	if(flag_next == 1)
	{
		TIM8->CCR2 = pwm_value; // Устанавливаем ШИМ

    // Сбрасываем и настраиваем реле
    reset_all_relays();
    set_relay_state(SEL1,ON); // Подключаем ШИМ
    set_relay_state(IO4, ON);  // IO4 всегда включено
    set_relay_state(IO5, ON);  // IO5 всегда включено
    set_relay_state(IO6, ON);  // IO6 всегда включено
    set_relay_state(IO7, ON);  // IO7 всегда включено
  
    // Устанавливаем нужное состояние реле в зависимости от функции
    switch (exclude_relay) {
        case IO0:
            set_relay_state(IO0, ON); //подключаем 
						set_relay_state(IO1, OFF); 
            set_relay_state(IO2, ON);  
            set_relay_state(IO3, ON);
            break;
        
        case IO1:
            set_relay_state(IO0, OFF); 
            set_relay_state(IO1, ON);
            set_relay_state(IO2, ON);
            set_relay_state(IO3, ON);
            break;

        case IO2:
            set_relay_state(IO0, OFF);
            set_relay_state(IO1, OFF);
            set_relay_state(IO2, OFF);  
            set_relay_state(IO3, ON);
            break;
        
        case IO3:
            set_relay_state(IO0, OFF);
            set_relay_state(IO1, OFF);
            set_relay_state(IO2, ON);  
            set_relay_state(IO3, OFF);
            break;

        default:
            break;
    }

    // Проверяем ввод
        send_transkon("SEND=2;", 7);
        uint8_t IO0, IO1, IO2, IO3;
				
        if (sscanf((char *)rx_buffer, "In = %hhd,%hhd,%hhd,%hhd", &IO0, &IO1, &IO2, &IO3) == 4) 
					{
						while(str8.flag_diagnostic_start_stop)
							{
								
								if (IO0 == expected_IO[0] && IO1 == expected_IO[1] &&
										IO2 == expected_IO[2] && IO3 == expected_IO[3]) 
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
	}
	
	if(flag_next != 1)
	{
		str8.flag_isError = 1;
	}
	
	TIM8->CCR2 = pwm_0;
	reset_all_relays();
	str0.flag_test = 0;
}

void test_io_next(uint16_t pwm_value, RelayIndex exclude_relay, uint8_t expected_IO[4], uint8_t flag_bit) {
    TIM8->CCR2 = pwm_value; // Устанавливаем ШИМ

    // Сбрасываем и настраиваем реле
    reset_all_relays();
    set_relay_state(SEL1,ON); // Подключаем ШИМ
    set_relay_state(IO4, ON);  // IO4 всегда включено
    set_relay_state(IO5, ON);  // IO5 всегда включено
    set_relay_state(IO6, ON);  // IO6 всегда включено
    set_relay_state(IO7, ON);  // IO7 всегда включено
  
    // Устанавливаем нужное состояние реле в зависимости от функции
    switch (exclude_relay) {
        case IO0:
            set_relay_state(IO0, ON); //подключаем 
						set_relay_state(IO1, OFF); 
            set_relay_state(IO2, ON);  
            set_relay_state(IO3, ON);
            break;
        
        case IO1:
            set_relay_state(IO0, OFF); 
            set_relay_state(IO1, ON);
            set_relay_state(IO2, ON);
            set_relay_state(IO3, ON);
            break;

        case IO2:
            set_relay_state(IO0, OFF);
            set_relay_state(IO1, OFF);
            set_relay_state(IO2, OFF);  
            set_relay_state(IO3, ON);
            break;
        
        case IO3:
            set_relay_state(IO0, OFF);
            set_relay_state(IO1, OFF);
            set_relay_state(IO2, ON);  
            set_relay_state(IO3, OFF);
            break;

        default:
            break;
    }

    // Проверяем ввод
    for (uint8_t attempt = 0; attempt < ch_test_io; attempt++) {
        send_transkon("SEND=2;", 7);

        uint8_t IO0, IO1, IO2, IO3;
        if (sscanf((char *)rx_buffer, "In = %hhd,%hhd,%hhd,%hhd", &IO0, &IO1, &IO2, &IO3) == 4) {
            if (IO0 == expected_IO[0] && IO1 == expected_IO[1] &&
                IO2 == expected_IO[2] && IO3 == expected_IO[3]) {
                str5.val |= (1 << flag_bit); // Устанавливаем бит во флаге
                break;
            }
        }
    }
    counter_io++;
}

void high_level_next()
{
	reset_all_relays();
	set_relay_state(IO4, ON);	//размыкаем реле проверки 
	set_relay_state(IO5, ON);
	set_relay_state(IO6, ON);	//размыкаем реле проверки 
	set_relay_state(IO7, ON);
	
	set_relay_state(SEL2, ON); //подключаем IN PWM 
	
	while(1)
	{
		send_transkon("O=1,1,1,1;", 10); 
		uint8_t v0,v1,v2,v3;
		if (sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3) == 4)
		{
			if (v0 == 1 && v1 == 1 && v2 == 1 && v3 == 1) break;
		}
	}
	counter_io++;
}

void individual_test_high_level_IO4_IO7(uint8_t canal, uint8_t flag_next)
{
	reset_all_relays();
	
	if(flag_next == 0) //установка мода 1
	{
			send_transkon("MODE=1;", 7); 
			uint8_t Mode = 0;
			if (sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 1)
			{	
				flag_next++;
			}
	}
	
	if(flag_next == 1) // установка высокого уровня на выводах транскона
	{
		reset_all_relays();
		set_relay_state(IO4, ON);	//размыкаем реле проверки 
		set_relay_state(IO5, ON);
		set_relay_state(IO6, ON);	//размыкаем реле проверки 
		set_relay_state(IO7, ON);
		set_relay_state(IO2, ON);
		set_relay_state(IO3, ON);
		
		set_relay_state(SEL2, ON); //подключаем IN PWM 
		
		send_transkon("O=1,1,1,1;", 10); 
		uint8_t v0,v1,v2,v3;
		sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3);
		
		set_relay_state(SEL2, OFF); //отключаем IN PWM
				
		switch(canal)
		{
			case 4:
				if(v0 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO3, ON); // включение IO3
					set_relay_state(IO2, ON);
					set_relay_state(IO4, OFF);
					set_relay_state(IO5, ON);
					set_relay_state(IO6, ON);
					set_relay_state(IO7, ON);
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
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
				break;
			
			case 5:
				if(v1 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO3, ON); // включение IO3
					set_relay_state(IO2, ON);
					set_relay_state(IO4, ON);
					set_relay_state(IO5, OFF);
					set_relay_state(IO6, ON);
					set_relay_state(IO7, ON);
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
						{
							GOOD_LED_ON();// мигание светодиодом ;
							set_led_state(ERROR_Led, OFF);
						}
						else
						{
							set_led_state(ERROR_Led, ON);
						}	
					}
				}
				break;
				
			case 6:
				if(v2 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO3, ON); // включение IO3
					set_relay_state(IO2, ON);
					set_relay_state(IO4, ON);
					set_relay_state(IO5, ON);
					set_relay_state(IO6, OFF);
					set_relay_state(IO7, ON);
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
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
				break;
				
			case 7:
				if(v3 == 1)
				{
					flag_next++;
				}
				else
				{
					set_led_state(ERROR_Led, ON);
				}
						
				if(flag_next == 2)
				{
					reset_all_relays();
					set_relay_state(IO3, ON); // включение IO3
					set_relay_state(IO2, ON);
					set_relay_state(IO4, ON);
					set_relay_state(IO5, ON);
					set_relay_state(IO6, ON);
					set_relay_state(IO7, OFF);
					
					set_relay_state(SEL2, ON);
					
					while(str8.flag_diagnostic_start_stop)
					{
						bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
						if(value) 
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
				break;
		}
	}
	
	if(flag_next != 2)
	{
		str8.flag_isError = 1;
	}
	else
	{
		reset_all_relays();
		
		while(1) // установка low level
		{
			send_transkon("O=0,0,0,0;", 10); 
			uint8_t v0,v1,v2,v3;
			if (sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3) == 4)
			{
				if (v0 == 0 && v1 == 0 && v2 == 0 && v3 == 0)
				{
					break;
				}
			}
		}
	}
	str0.flag_test = 0;
}

void test_high_level_IO4()
{
	set_relay_state(IO4, OFF); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
  {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str5.HL_IO4 = 1;
			set_relay_state(IO4, ON);
			break;
		}
	}
	counter_io++;
}

void test_high_level_IO5()
{
	set_relay_state(IO5, OFF); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
  {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str5.HL_IO5 = 1;
			set_relay_state(IO5, ON);
			break;
		}
	}
	counter_io++;
}

void test_high_level_IO6()
{
	set_relay_state(IO6, OFF); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
  {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str5.HL_IO6 = 1;
			set_relay_state(IO6, ON);
			break;
		}
	}
	counter_io++;
}

void test_high_level_IO7()
{
	set_relay_state(IO7, OFF); //подключаем контакт 
	for (uint8_t attempt = 0; attempt < ch_test_io; attempt++)
    {
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value) 
		{
			str5.HL_IO7 = 1;
			set_relay_state(IO7, ON);
			break;
		}
	}
	counter_io++;
	
	set_relay_state(IO7, OFF);
	for(uint8_t attempt = 0; attempt < ch_test_io; attempt++){
		vTaskDelay(250);
		bool value = !HAL_GPIO_ReadPin(IN_PWM2_GPIO_Port, IN_PWM2_Pin);
		if(value)
		{
			str5.HL_IO7 = 1;
			set_relay_state(IO7, ON);
			break;
		}
	}
	counter_io++;
}


void test_io()
{
    TIM8->CCR2 = pwm_0; // Сброс PWM

      switch (counter_io)
    {
        case 0:   //mode - 0 | тест IO4 - IO7
            io_test(pwm_24v, IO4, (uint8_t[]){1, 0, 0, 0}, 0);  // Тест IO4
            break;

        case 1: 
            io_test(pwm_24v, IO5, (uint8_t[]){0, 1, 0, 0}, 1);  // Тест IO5
            break;

        case 2: 
            io_test(pwm_24v, IO6, (uint8_t[]){0, 0, 1, 0}, 2);  // Тест IO6
            break;

        case 3: 
            io_test(pwm_24v, IO7, (uint8_t[]){0, 0, 0, 1}, 3);	// Тест IO7
            break;

        case 4:
            io_test(pwm_7_5v, IO4, (uint8_t[]){1, 0, 0, 0}, 4);
            break;
				
				case 5:
            io_test(pwm_7_5v, IO5, (uint8_t[]){0, 1, 0, 0}, 5);
            break;
				
				case 6:
            io_test(pwm_7_5v, IO6, (uint8_t[]){0, 0, 1, 0}, 6);
            break;
				
				case 7:
            io_test(pwm_7_5v, IO7, (uint8_t[]){0, 0, 0, 1}, 7);
            break;
				
				case 8:
            io_test(pwm_4_5v, IO4, (uint8_t[]){0, 0, 0, 0}, 8);
            break;
				
				case 9:
            io_test(pwm_4_5v, IO5, (uint8_t[]){0, 0, 0, 0}, 9);
            break;
				
				case 10:
            io_test(pwm_4_5v, IO6, (uint8_t[]){0, 0, 0, 0}, 10);
            break;
				
				case 11:
            io_test(pwm_4_5v, IO7, (uint8_t[]){0, 0, 0, 0}, 11);
            break;

				case 12:
            high_level(); //mode - 0 | тест IO0 - IO3
            break;
				
				case 13:
						test_high_level_IO3();
						break;
				
				case 14:
						test_high_level_IO2();
						break;
				
				case 15:
						test_high_level_IO1();
						break;
				
				case 16:
						test_high_level_IO0();
						break;
				
				case 17:
					  low_level(); //брасываем высокий уровень 
						break;
				
				case 18:
						mode_set_1();	//переключаем на mode - 1
						break;
				
				case 19:
						test_io_next(pwm_24v, IO0, (uint8_t[]){1, 0, 0, 0}, 0);
						break;
				
				case 20:
						test_io_next(pwm_24v, IO1, (uint8_t[]){0, 1, 0, 0}, 1);
						break;
				
				case 21:
						test_io_next(pwm_24v, IO2, (uint8_t[]){0, 0, 1, 0}, 2);
						break;
				
				case 22:
						test_io_next(pwm_24v, IO3, (uint8_t[]){0, 0, 0, 1}, 3);
						break;
				
				case 23:
					  test_io_next(pwm_7_5v, IO0, (uint8_t[]){1, 0, 0, 0}, 4);
						break;
						
				case 24:
					  test_io_next(pwm_7_5v, IO1, (uint8_t[]){0, 1, 0, 0}, 5);
						break;
						
				case 25:
					  test_io_next(pwm_7_5v, IO2, (uint8_t[]){0, 0, 1, 0}, 6);
						break;
						
				case 26:
					  test_io_next(pwm_7_5v, IO3, (uint8_t[]){0, 0, 0, 1}, 7);
						break;
						
				case 27:
					  test_io_next(pwm_4_5v, IO0, (uint8_t[]){0, 0, 0, 0}, 8);
						break;
						
				case 28:
					  test_io_next(pwm_4_5v, IO1, (uint8_t[]){0, 0, 0, 0}, 9);
						break;
						
				case 29:
					  test_io_next(pwm_4_5v, IO2, (uint8_t[]){0, 0, 0, 0}, 10);
						break;
						
				case 30:
					  test_io_next(pwm_4_5v, IO3, (uint8_t[]){0, 0, 0, 0}, 11);
						break;
				
				case 31:
						high_level_next();
						break;
				
				case 32:
						test_high_level_IO4();
						break;
				
				case 33:
						test_high_level_IO5();
						break;
							
				case 34:
						test_high_level_IO6();
						break;
							
				case 35:
						test_high_level_IO7();
						break;
				
				case 36:
						low_level(); //сбрасываем высокий уровень 
						break;
				
				case 37:
					//	counter_io = 0;
						ModbusRegister[11] = str4.val;
						ModbusRegister[12] = str5.val;
						counter++;
					break;
    }	
}
