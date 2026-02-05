#include "rs485_.h"

#define ch_test_rs485 10 // количество попыток


uint16_t hex_to_int(char c) // Функция для преобразования символов шестнадцатеричной строки в 16-битное значение
{
    if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'A' && c <= 'F') return c - 'A' + 10;

    return 0;
}

uint16_t hex_string_to_short(const char *hex) // Функция для преобразования 4 символов шестнадцатеричной строки в 16-битное значение
{
    return (hex_to_int(hex[0]) << 12) |
           (hex_to_int(hex[1]) << 8) |
           (hex_to_int(hex[2]) << 4) |
            hex_to_int(hex[3]);
}

void test_485()
{
    for (uint8_t attempt = 0; attempt < ch_test_rs485; attempt++) 
    {	
			memset(rx_buffer, 0, rx_buffer_size);
        send_transkon("SEND=15;", 8); // запрос UID
				/*char* start_ptr = NULL;
				
				char temp_rxbuffer[rx_buffer_size]; // Локальный буфер
				memcpy(temp_rxbuffer, rx_buffer, rx_buffer_size);
				memset(rx_buffer, 0, rx_buffer_size);		*/
				
			/*
				for (int i = 0; i < rx_buffer_size - 6; i++) // 6 = длина "UID = "
				{ 
						if (rx_buffer[i] == 'U' && rx_buffer[i+1] == 'I' && rx_buffer[i+2] == 'D' &&
								rx_buffer[i+3] == ' ' && rx_buffer[i+4] == '=' && rx_buffer[i+5] == ' ') 
						{
								start_ptr = (char*)&rx_buffer[i + 6]; // Пропустить "UID = "
								break;
						}
				}

				if (start_ptr != NULL) 
				{
						char hex_id[30] = ""; // hex id чипа
						
						for (int i = 0; i < 24; i++) 
						{
								hex_id[i] = start_ptr[i];
						}
						hex_id[24] = '\0'; // Добавляем завершающий символ

						uint16_t registers[6];
						for (int i = 0; i < 6; i++) 
						{
								registers[i] = hex_string_to_short(&hex_id[i * 4]);
								ModbusRegister[i+1] = registers[i]; // заполняем 1-6 регистры UID
						}

						str0.Transkon_485 = 1; // 485 исправен
						counter++; // движение по циклу теста
						return; // Успешное завершение теста
				}
      */
			
			if(rx_buffer[0] != 0 && rx_buffer[1] != 0 ) 
			{
				str0.Transkon_485 = 1; // 485 исправен
				counter++; // движение по циклу теста
				return; // Успешное завершение теста
			}
    }

    // Если все попытки завершились неудачей
    str0.Transkon_485 = 0; // 485 НЕ исправен
    counter = 6; // конец теста
}

void individual_test_rs485()
{
    while (str8.flag_diagnostic_start_stop)
    {	
			memset(rx_buffer, 0, rx_buffer_size);
			send_transkon("SEND=15;", 8); // запрос UID
			
			if(rx_buffer[0] != 0 && rx_buffer[1] != 0 ) 
			{
				GOOD_LED_ON();// мигание светодиодом 
				set_led_state(ERROR_Led, OFF);
			}
			else
			{
				set_led_state(ERROR_Led, ON);
			}
    }
		
		str0.flag_test = 0;
}
/*
void test_485_permach()
{
	memset(rx_buffer, 0, rx_buffer_size);
	send_transkon("SEND=15;", 8); // запрос UID
		
	if(rx_buffer[0] != 0 && rx_buffer[1] != 0 ) 
	{
		str0.Transkon_485 = 1; // 485 исправен
	}
	else
	{
		str0.Transkon_485 = 0; // 485 НЕ исправен
		reset();
		obnull();
	}
	
}*/
