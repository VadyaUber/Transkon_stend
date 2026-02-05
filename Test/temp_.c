#include "temp_.h"

#define ch_test_temp 10 // количество попыток

char message_mode0[] = "MODE=0;";
char chek_temp[] = "O=0,0,0,0;";

float prothent = 0.8;


/* наминал резисторов */
uint16_t R1_constant = 750;
uint16_t R2_constant = 1000;
uint16_t R3_constant = 1100;
uint16_t R4_constant = 1330;
uint16_t R5_constant = 2150;

uint8_t counter_temp = 0; //счетрчик движения по алгоритму проверки 

void Mode_0()
{
	send_transkon(message_mode0, sizeof(message_mode0));

	uint8_t Mode = 1;
	if(sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 0) 
	{
		counter_temp++;
	}

}

void individual_test_temp(float R_constant, RelayIndex relay_resist, uint8_t canal_number, uint8_t flag_next)
{

	char my_temp_rxbuffer[rx_buffer_size]; // Локальный буфер
	double Temp_ = 0.0;
	
	if(flag_next == 0) // установка мода 0 для транскона
	{
		send_transkon(message_mode0, sizeof(message_mode0));

		uint8_t Mode = 1;
		if(sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 0) 
		{
			flag_next++;
		}
	}
	
	if(flag_next == 1)
	{
		send_transkon(chek_temp, sizeof(chek_temp));

		uint8_t v0,v1,v2,v3;
		if (sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3) == 4)
		{
			if (v0 == 0 && v1 == 0 && v2 == 0 && v3 == 0)
			{
				flag_next++; //движение по циклу 
			}
		}
	}
	
	if(flag_next == 2)
	{
		// Массивы для настройки реле и сообщений
		RelayIndex relay_selectors[4] = {SEL_RT1, SEL_RT2, SEL_RT3, SEL_RT4};
		
		char *messages[] = {"SEND=4;", "SEND=5;", "SEND=6;", "SEND=7;"};
		char *ss[] = {"Temp CH1 = %lf", "Temp CH2 = %lf", "Temp CH3 = %lf", "Temp CH4 = %lf"};
		
		reset_all_relays();
		set_relay_state(relay_resist, ON);          // Включаем главный реле
		set_relay_state(relay_selectors[canal_number], ON);  // Включаем соответствующее реле

		while(str8.flag_diagnostic_start_stop)
		{ 
			send_transkon(messages[canal_number], 7);
			send_transkon(messages[canal_number], 7);
			
			memcpy(my_temp_rxbuffer, rx_buffer, rx_buffer_size);
			memset(rx_buffer, 0, rx_buffer_size);		
				
			if (sscanf(my_temp_rxbuffer, ss[canal_number], &Temp_) == 1)
			{
				float result = R_constant / Temp_; //1000
				result = result * 100;
				result = result - 100;
				result = fabs(result);
				
				if (result <= prothent) // Если разница в пределах допустимого
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
	
	if(flag_next != 2)
	{
		str8.flag_isError = 1;
	}
	
	reset_all_relays();
	str0.flag_test = 0;
}


void chek_temp_()
{
	send_transkon(chek_temp, sizeof(chek_temp));

	uint8_t v0,v1,v2,v3;
	if (sscanf((char *)rx_buffer,"Out = %hhd,%hhd,%hhd,%hhd", &v0, &v1, &v2, &v3) == 4)
	{
		if (v0 == 0 && v1 == 0 && v2 == 0 && v3 == 0)
		{
			counter_temp++; //движение по циклу 
		}
	}
}


void temp_test(float R_constant, RelayIndex main_relay, uint8_t modbus_offset, uint8_t bit_offset, uint8_t flag_group)
{
    // Массивы для настройки реле и сообщений
    RelayIndex relay_selectors[4] = {SEL_RT1, SEL_RT2, SEL_RT3, SEL_RT4};
    char *ss[] = {"Temp CH1 = %lf", "Temp CH2 = %lf", "Temp CH3 = %lf", "Temp CH4 = %lf"};
    char *messages[] = {"SEND=4;", "SEND=5;", "SEND=6;", "SEND=7;"};

    for (uint8_t i = 0; i < 4; i++) 
    {
        reset_all_relays();
        set_relay_state(main_relay, ON);          // Включаем главный реле
        set_relay_state(relay_selectors[i], ON);  // Включаем соответствующее реле
				
        for (uint8_t attempt = 0; attempt < ch_test_temp; attempt++) 
        {
            send_transkon(messages[i], 7);
						send_transkon(messages[i], 7);

						double Temp_ = 0.0;
						
						char temp_rxbuffer[rx_buffer_size]; // Локальный буфер
						memcpy(temp_rxbuffer, rx_buffer, rx_buffer_size);
						memset(rx_buffer, 0, rx_buffer_size);		
						
						if (sscanf(temp_rxbuffer, ss[i], &Temp_) == 1)
						{
							
								float result = R_constant / Temp_;
								result = result * 100;
								result = result - 100;
								result = fabs(result);

							
								ModbusRegister[i + modbus_offset] = Temp_ * 10;

								if (result <= prothent) // Если разница в пределах допустимого
								{
										if (flag_group == 1)
										{
												str1.val |= (1 << (i + bit_offset)); // Устанавливаем бит в str1
										}
										else if (flag_group == 2)
										{
												str2.val |= (1 << i); // Устанавливаем бит в str2
										}
										break; // Успешное завершение теста
								}
						}

        }
    }
    counter_temp++;
}


void test_temp()
{
    switch (counter_temp) 
    {
        case 0:
            Mode_0();
            break;

        case 1:
            chek_temp_();
            break;

        case 2: // Режим 768
            temp_test(R1_constant, R1, 15, 0, 1);
            break;

        case 3: // Режим 1000
            temp_test(R2_constant, R2, 19, 4, 1);
            break;

        case 4: // Режим 1100
            temp_test(R3_constant, R3, 23, 8, 1);
            break;

        case 5: // Режим 1330
            temp_test(R4_constant, R4, 27, 12, 1);
            break;

        case 6: // Режим 2100
            temp_test(R5_constant, R5, 31, 0, 2);
            break;

        case 7:
					//	counter_temp = 0;
						ModbusRegister[9] = str1.val;
						ModbusRegister[10] = str2.val;
            counter++;
            break;
    }
}
