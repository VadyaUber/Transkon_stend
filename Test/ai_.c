#include "ai_.h"

//#define debug_

#define ch_test_ai 10 // количество попыток

#define pwm0 2399 //0v PWM

#ifdef NOMBER_Sten_1
uint16_t pwm_4v = 1993;   
uint16_t pwm_12v = 1231;  
uint16_t pwm_20v = 469;		
#endif 

#ifdef NOMBER_Sten_2
uint16_t pwm_4v = 2008;   
uint16_t pwm_12v = 1256;  
uint16_t pwm_20v = 504;		
#endif 

#ifdef NOMBER_Sten_3
uint16_t pwm_4v = 2000;   
uint16_t pwm_12v = 1234;  
uint16_t pwm_20v = 467;		
#endif 


uint16_t pwm_4v_constant = 4000;
uint16_t pwm_12v_constant = 12000;
uint16_t pwm_20v_constant = 20000;


float prothent_ai = 1.3; //процент прохождения
uint8_t prothent_filling_arr = 10; //процент при котором происходит записиь в массив 

uint8_t counter_ai = 0; //счетрчик движения по алгоритму проверки



uint32_t FiltringADC(uint16_t* aray, int size) {
  uint64_t MathWait = 0;
  uint32_t TekValue = 0;
  int n = 0;
  if (size == 0) return 0;
  for (int i = 0; i < size; i++) {
    MathWait += aray[i];
  }
  MathWait = MathWait / size;
  for (int i = 0; i < size; i++) {
    if (aray[i] < (MathWait + MathWait * 0.3) && aray[i] > (MathWait - MathWait * 0.3)) 
    {
      TekValue += aray[i];
      n++;
    }
  }
  if (n == 0) return 0;
  return  TekValue / n;
}


void AI_Test(RelayIndex relay, uint16_t pwm_value, uint16_t pwm_constant, uint16_t modbus_base_register, uint8_t shift_str, char* send_comand, char* get_comand) 
{
    TIM8->CCR2 = pwm_value;

    reset_all_relays();
    set_relay_state(SEL1, ON);
    set_relay_state(relay, ON);

    uint16_t temp_arr_val_ai[ch_test_ai] = {0}; // Массив полученных данных

    vTaskDelay(1000);

    for (uint8_t attempt = 0; attempt < ch_test_ai; attempt++) // 10 запросов
    {
        uint16_t val = 0;
        for (uint8_t i = 0; i < 10; i++) // 10 попыток на 1 запрос
        {
            send_transkon(send_comand, 7);
            uint16_t temp_val = 0;
            if (sscanf((char *)rx_buffer, get_comand, &temp_val) == 1)
            {
                memset(rx_buffer, 0, rx_buffer_size);
                if (temp_val != 0) // Вернул не 0
                {
                    val = temp_val;
                    break;
                }
            }
        }
        temp_arr_val_ai[attempt] = val; // Заполняем массив
    }

    uint16_t val_ = FiltringADC(temp_arr_val_ai, ch_test_ai); // Фильтруем

    float pr = val_ - pwm_constant;
    pr = fabs(pr);
    pr /= 30000;
    pr *= 100;

    /* Пузырьковая сортировка */
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (temp_arr_val_ai[j] > temp_arr_val_ai[j + 1])
            {
                uint16_t b = temp_arr_val_ai[j];
                temp_arr_val_ai[j] = temp_arr_val_ai[j + 1];
                temp_arr_val_ai[j + 1] = b;
            }
        }
    }

    ModbusRegister[modbus_base_register] = temp_arr_val_ai[0];
    ModbusRegister[modbus_base_register + 1] = val_;
    ModbusRegister[modbus_base_register + 2] = temp_arr_val_ai[9];
    uint16_t temp_pr = pr * 100;
    ModbusRegister[modbus_base_register + 3] = temp_pr;

    if (pr <= prothent_ai) // Прошел по %
    {
        str3.val |= (1 << shift_str);
    }
    counter_ai++;
}


/*===========================================================*/

char message_mode0_ai[] = "MODE=0;";

void individual_test_ai(RelayIndex relay, uint16_t pwm_value, uint16_t pwm_constant, char* send_comand, char* get_comand, uint8_t flag_next)
{
	TIM8->CCR2=pwm0; // установка 0 напряжения
	
	reset_all_relays(); // сброс всех реле
	
	if(flag_next == 0) // установка мода 0 для транскона
	{
		send_transkon(message_mode0_ai, sizeof(message_mode0_ai));

		uint8_t Mode = 1;
		if(sscanf((char *)rx_buffer,"Mode = %hhd", &Mode) == 1 && Mode == 0) 
		{
			flag_next++;
		}
	}
	
	if(flag_next == 1)
	{
		TIM8->CCR2 = pwm_value;
		
		set_relay_state(IO2, ON); // выключение реле IO2-IO7
		set_relay_state(IO3, ON);
		set_relay_state(IO4, ON);
		set_relay_state(IO5, ON);
		set_relay_state(IO6, ON);
		set_relay_state(IO7, ON);
		
    set_relay_state(SEL1, ON);
    set_relay_state(relay, ON);

    uint16_t temp_arr_val_ai[ch_test_ai] = {0}; // Массив полученных данных

    vTaskDelay(1000);

    for (uint8_t attempt = 0; attempt < ch_test_ai; attempt++) // 10 запросов
    {
        uint16_t val = 0;
        for (uint8_t i = 0; i < 10; i++) // 10 попыток на 1 запрос
        {
            send_transkon(send_comand, 7);
            uint16_t temp_val = 0;
            if (sscanf((char *)rx_buffer, get_comand, &temp_val) == 1)
            {
                memset(rx_buffer, 0, rx_buffer_size);
                if (temp_val != 0) // Вернул не 0
                {
                    val = temp_val;
                    break;
                }
            }
        }
        temp_arr_val_ai[attempt] = val; // Заполняем массив
    }

    uint16_t val_ = FiltringADC(temp_arr_val_ai, ch_test_ai); // Фильтруем

    float pr = val_ - pwm_constant;
    pr = fabs(pr);
    pr /= 30000;
    pr *= 100;

    /* Пузырьковая сортировка */
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (temp_arr_val_ai[j] > temp_arr_val_ai[j + 1])
            {
                uint16_t b = temp_arr_val_ai[j];
                temp_arr_val_ai[j] = temp_arr_val_ai[j + 1];
                temp_arr_val_ai[j + 1] = b;
            }
        }
    }

    uint16_t temp_pr = pr * 100;

		while(str8.flag_diagnostic_start_stop)
		{
			if (pr <= prothent_ai) // Прошел по %
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
	
	if(flag_next != 1)
	{
		str8.flag_isError = 1;
	}
	
	TIM8->CCR2=pwm0;
	reset_all_relays();
	str0.flag_test = 0;
}


void test_ai()
{
	TIM8->CCR2=pwm0; 
	
	switch (counter_ai) 
	{	
		case 0:
			AI_Test(A14, pwm_4v, pwm_4v_constant, 39, 0, "SEND=8;", "VoCH4 = %hd");
			//AI14_Test(pwm_4v, pwm_4v_constant, 39, 0);
			break;
		
		case 1:
				AI_Test(A14, pwm_12v, pwm_12v_constant, 43, 1, "SEND=8;", "VoCH4 = %hd");
			 //AI14_Test(pwm_12v, pwm_12v_constant, 43, 1);
			break;
		
		case 2:
			AI_Test(A14, pwm_20v, pwm_20v_constant, 47, 2, "SEND=8;", "VoCH4 = %hd");
			 //AI14_Test(pwm_20v, pwm_20v_constant, 47, 2);
			break;
		
		case 3:
			AI_Test(A15, pwm_4v, pwm_4v_constant, 51, 3, "SEND=9;", "VoCH5 = %hd");
			//AI15_Test(pwm_4v, pwm_4v_constant, 51, 3);
			break;
		
		case 4:
			AI_Test(A15, pwm_12v, pwm_12v_constant, 55, 4, "SEND=9;", "VoCH5 = %hd");
			//AI15_Test(pwm_12v, pwm_12v_constant, 55, 4);
			break;
		
		case 5:
			AI_Test(A15, pwm_20v, pwm_20v_constant, 59, 5, "SEND=9;", "VoCH5 = %hd");
			//AI15_Test(pwm_20v, pwm_20v_constant, 59, 5);
			break;
		
		case 6:
			AI_Test(KL15, pwm_4v, pwm_4v_constant, 63, 6, "SEND=10;", "VoKI15 = %hd");
			break;
		
		case 7:
			AI_Test(KL15, pwm_12v, pwm_12v_constant, 67, 7, "SEND=10;", "VoKI15 = %hd");
			break;
		
		case 8:
			AI_Test(KL15, pwm_20v, pwm_20v_constant, 71, 8, "SEND=10;", "VoKI15 = %hd");
			break;
		
		case 9:
			//counter_ai = 0;
			ModbusRegister[13] = str3.val;
			TIM8->CCR2=pwm0;
			counter++;
			break;
	}
}



/*

void AI14_Test(uint16_t pwm_value, uint16_t pwm_constant, uint16_t modbus_base_register, uint8_t shift_str)
{
    TIM8->CCR2 = pwm_value;

    reset_all_relays();
    set_relay_state(SEL1, ON);
    set_relay_state(A14, ON);

    uint16_t temp_arr_val_ai[ch_test_ai] = {0}; // Массив полученных данных

    vTaskDelay(1000);

    for (uint8_t attempt = 0; attempt < ch_test_ai; attempt++) // 10 запросов
    {
        uint16_t val = 0;
        for (uint8_t i = 0; i < 10; i++) // 10 попыток на 1 запрос
        {
            send_transkon("SEND=8;", 7);
            uint16_t temp_val = 0;
            if (sscanf((char *)rx_buffer, "VoCH4 = %hd", &temp_val) == 1)
            {
                memset(rx_buffer, 0, rx_buffer_size);
                if (temp_val != 0) // Вернул не 0
                {
                    val = temp_val;
                    break;
                }
            }
        }
        temp_arr_val_ai[attempt] = val; // Заполняем массив
    }

    uint16_t val_ = FiltringADC(temp_arr_val_ai, ch_test_ai); // Фильтруем

    float pr = val_ - pwm_constant;
    pr = fabs(pr);
    pr /= 30000;
    pr *= 100;

    // Пузырьковая сортировка 
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (temp_arr_val_ai[j] > temp_arr_val_ai[j + 1])
            {
                uint16_t b = temp_arr_val_ai[j];
                temp_arr_val_ai[j] = temp_arr_val_ai[j + 1];
                temp_arr_val_ai[j + 1] = b;
            }
        }
    }

    ModbusRegister[modbus_base_register] = temp_arr_val_ai[0];
    ModbusRegister[modbus_base_register + 1] = val_;
    ModbusRegister[modbus_base_register + 2] = temp_arr_val_ai[9];
    uint16_t temp_pr = pr * 100;
    ModbusRegister[modbus_base_register + 3] = temp_pr;

    if (pr <= prothent_ai) // Прошел по %
    {
        str3.val |= (1 << shift_str);
    }
    counter_ai++;
}



void AI15_Test(uint16_t pwm_value, uint16_t pwm_constant, uint16_t modbus_base_register, uint8_t shift_str)
{
    TIM8->CCR2 = pwm_value;

    reset_all_relays();
    set_relay_state(SEL1, ON);
    set_relay_state(A15, ON);

    uint16_t temp_arr_val_ai[ch_test_ai] = {0}; // Массив полученных данных

    vTaskDelay(1000);

    for (uint8_t attempt = 0; attempt < ch_test_ai; attempt++) // 10 запросов
    {
        uint16_t val = 0;
        for (uint8_t i = 0; i < 10; i++) // 10 попыток на 1 запрос
        {
            send_transkon("SEND=9;", 7);
            uint16_t temp_val = 0;
            if (sscanf((char *)rx_buffer, "VoCH5 = %hd", &temp_val) == 1)
            {
                memset(rx_buffer, 0, rx_buffer_size);
                if (temp_val != 0) // Вернул не 0
                {
                    val = temp_val;
                    break;
                }
            }
        }
        temp_arr_val_ai[attempt] = val; // Заполняем массив
    }

    uint16_t val_ = FiltringADC(temp_arr_val_ai, ch_test_ai); // Фильтруем

    float pr = val_ - pwm_constant;
    pr = fabs(pr);
    pr /= 30000;
    pr *= 100;

    //Пузырьковая сортировка 
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (temp_arr_val_ai[j] > temp_arr_val_ai[j + 1])
            {
                uint16_t b = temp_arr_val_ai[j];
                temp_arr_val_ai[j] = temp_arr_val_ai[j + 1];
                temp_arr_val_ai[j + 1] = b;
            }
        }
    }

    ModbusRegister[modbus_base_register] = temp_arr_val_ai[0];
    ModbusRegister[modbus_base_register + 1] = val_;
    ModbusRegister[modbus_base_register + 2] = temp_arr_val_ai[9];
    uint16_t temp_pr = pr * 100;
    ModbusRegister[modbus_base_register + 3] = temp_pr;

    if (pr <= prothent_ai) // Прошел по %
    {
        str3.val |= (1 << shift_str);
    }
    counter_ai++;
}*/
