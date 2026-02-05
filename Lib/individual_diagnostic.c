#include "individual_diagnostic.h"

uint16_t pwm_4v_ind = 1993;   
uint16_t pwm_12v_ind = 1231;  
uint16_t pwm_20v_ind = 469;

uint16_t pwm_4_5v_ind = 1946; 
uint16_t pwm_7_5v_ind = 1660;  
uint16_t pwm_24v_ind = 0;

uint16_t R1_constant_ind = 750;
uint16_t R2_constant_ind = 1000;
uint16_t R3_constant_ind = 1100;
uint16_t R4_constant_ind = 1330;
uint16_t R5_constant_ind = 2150;

void individual_diagnostic()
{
	uint8_t test_number = str8.counter_test; // запись номера теста
	str8.flag_diagnostic_ready = 0;
	str8.flag_isTesting = 1;
	
	set_led_state(ERROR_Led, OFF);
	GOOD_LED_OFF();
	
	reset_all_relays(); // сброс всех реле в анчальное состояние
	
	switch(test_number)
	{
		case 1: // test rs485
			individual_test_rs485();
			break;
		
		case 2:
			individual_test_can();
			break;
		
		case 3:
			individual_test_temp(R1_constant_ind, R1,0,0); // RT1 - Режим 768
			break;
		
		case 4:
			individual_test_temp(R1_constant_ind, R1,1,0); // RT2 - Режим 768
			break;
		
		case 5:
			individual_test_temp(R1_constant_ind, R1,2,0); // RT3 - Режим 768
			break;
		
		case 6:
			individual_test_temp(R1_constant_ind, R1,3,0); // RT4 - Режим 768
			break;
		
		case 7:
			individual_test_temp(R2_constant_ind, R2,0,0); // RT1 - Режим 1000
			break;
		
		case 8:
			individual_test_temp(R2_constant_ind, R2,1,0); // RT2 - Режим 1000
			break;
		
		case 9:
			individual_test_temp(R2_constant_ind, R2,2,0); // RT3 - Режим 1000
			break;
		
		case 10:
			individual_test_temp(R2_constant_ind, R2,3,0); // RT4 - Режим 1000
			break;
		
		case 11:
			individual_test_temp(R3_constant_ind, R3,0,0); // RT1 - Режим 1100
			break;
		
		case 12:
			individual_test_temp(R3_constant_ind, R3,1,0); // RT2 - Режим 1100
			break;
		
		case 13:
			individual_test_temp(R3_constant_ind, R3,2,0); // RT3 - Режим 1100
			break;
		
		case 14:
			individual_test_temp(R3_constant_ind, R3,3,0); // RT4 - Режим 1100
			break;
		
		case 15:
			individual_test_temp(R4_constant_ind, R4,0,0); // RT1 - Режим 1330
			break;
		
		case 16:
			individual_test_temp(R4_constant_ind, R4,1,0); // RT2 - Режим 1330
			break;
		
		case 17:
			individual_test_temp(R4_constant_ind, R4,2,0); // RT3 - Режим 1330
			break;
		
		case 18:
			individual_test_temp(R4_constant_ind, R4,3,0); // RT4 - Режим 1330
			break;
			
		case 19:
			individual_test_temp(R5_constant_ind, R5,0,0); // RT1 - Режим 2100
			break;
		
		case 20:
			individual_test_temp(R5_constant_ind, R5,1,0); // RT2 - Режим 2100
			break;
		
		case 21:
			individual_test_temp(R5_constant_ind, R5,2,0); // RT3 - Режим 2100
			break;
		
		case 22:
			individual_test_temp(R5_constant_ind, R5,3,0); // RT4 - Режим 2100
			break;
		
		case 23:
			individual_test_ai(A14, pwm_4v_ind, pwm_4v_constant, "SEND=8;", "VoCH4 = %hd", 0); 
			break;
		
		case 24:
			individual_test_ai(A14, pwm_12v_ind, pwm_12v_constant, "SEND=8;", "VoCH4 = %hd", 0);
			break;
		
		case 25:
			individual_test_ai(A14, pwm_20v_ind, pwm_20v_constant, "SEND=8;", "VoCH4 = %hd", 0); 
			break;
		
		case 26:
			individual_test_ai(A15, pwm_4v_ind, pwm_4v_constant, "SEND=9;", "VoCH5 = %hd", 0); 
			break;
		
		case 27:
			individual_test_ai(A15, pwm_12v_ind, pwm_12v_constant, "SEND=9;", "VoCH5 = %hd", 0); 
			break;
		
		case 28:
			individual_test_ai(A15, pwm_20v_ind, pwm_20v_constant, "SEND=9;", "VoCH5 = %hd", 0); 
			break;
		
		case 29:
			individual_test_ai(KL15, pwm_4v_ind, pwm_4v_constant, "SEND=10;", "VoKI15 = %hd", 0); 
			break;
		
		case 30:
			individual_test_ai(KL15, pwm_12v_ind, pwm_12v_constant, "SEND=10;", "VoKI15 = %hd", 0); 
			break;
		
		case 31:
			individual_test_ai(KL15, pwm_20v_ind, pwm_20v_constant, "SEND=10;", "VoKI15 = %hd", 0);
			break;
		
		case 32:
			individual_test_io_next(pwm_24v_ind, IO0, (uint8_t[]){1, 0, 0, 0}, 0);
			break;
			
		case 33:
			individual_test_io_next(pwm_24v_ind, IO1, (uint8_t[]){0, 1, 0, 0}, 0);
			break;
			
		case 34:
			individual_test_io_next(pwm_24v_ind, IO2, (uint8_t[]){0, 0, 1, 0}, 0);
			break;
			
		case 35:
			individual_test_io_next(pwm_24v_ind, IO3, (uint8_t[]){0, 0, 0, 1}, 0);
			break;
		
		case 36:
			individual_io_test(pwm_24v_ind, IO4, (uint8_t[]){1, 0, 0, 0}, 0); // Тест IO4
			break;
			
		case 37:
			individual_io_test(pwm_24v_ind, IO5, (uint8_t[]){0, 1, 0, 0}, 0); // Тест IO5
			break;
			
		case 38:
			individual_io_test(pwm_24v_ind, IO6, (uint8_t[]){0, 0, 1, 0}, 0);  // Тест IO6
			break;
			
		case 39:
			individual_io_test(pwm_24v_ind, IO7, (uint8_t[]){0, 0, 0, 1}, 0);	// Тест IO7
			break;
			
		case 40:
			individual_test_io_next(pwm_7_5v_ind, IO0, (uint8_t[]){1, 0, 0, 0}, 0);
			break;
			
		case 41:
			individual_test_io_next(pwm_7_5v_ind, IO1, (uint8_t[]){0, 1, 0, 0}, 0);
			break;
				
		case 42:
			individual_test_io_next(pwm_7_5v_ind, IO2, (uint8_t[]){0, 0, 1, 0}, 0);
			break;
		
		case 43:
			individual_test_io_next(pwm_7_5v_ind, IO3, (uint8_t[]){0, 0, 0, 1}, 0);
			break;
			
		case 44:
			individual_io_test(pwm_7_5v_ind, IO4, (uint8_t[]){1, 0, 0, 0}, 0); // Тест IO4
			break;
			
		case 45:
			individual_io_test(pwm_7_5v_ind, IO5, (uint8_t[]){0, 1, 0, 0}, 0); // Тест IO5
			break;
			
		case 46:
			individual_io_test(pwm_7_5v_ind, IO6, (uint8_t[]){0, 0, 1, 0}, 0);  // Тест IO6
			break;
			
		case 47:
			individual_io_test(pwm_7_5v_ind, IO7, (uint8_t[]){0, 0, 0, 1}, 0);	// Тест IO7
			break;
			
		case 48:
			individual_test_io_next(pwm_4_5v_ind, IO0, (uint8_t[]){0, 0, 0, 0}, 0);
			break;
				
		case 49:
			individual_test_io_next(pwm_4_5v_ind, IO1, (uint8_t[]){0, 0, 0, 0}, 0);
			break;
			
		case 50:
			individual_test_io_next(pwm_4_5v_ind, IO2, (uint8_t[]){0, 0, 0, 0}, 0);
			break;
			
		case 51:
			individual_test_io_next(pwm_4_5v_ind, IO3, (uint8_t[]){0, 0, 0, 0}, 0);
			break;
			
		case 52:
			individual_io_test(pwm_4_5v_ind, IO4, (uint8_t[]){0, 0, 0, 0}, 0); // Тест IO4
			break;
			
		case 53:
			individual_io_test(pwm_4_5v_ind, IO5, (uint8_t[]){0, 0, 0, 0}, 0); // Тест IO5
			break;
			
		case 54:
			individual_io_test(pwm_4_5v_ind, IO6, (uint8_t[]){0, 0, 0, 0}, 0);  // Тест IO6
			break;
			
		case 55:
			individual_io_test(pwm_4_5v_ind, IO7, (uint8_t[]){0, 0, 0, 0}, 0);	// Тест IO7
			break;
			
		case 56:
			individual_test_high_level_IO3_IO0(0,0); // Тест high level IO0
			break;
		
		case 57:
			individual_test_high_level_IO3_IO0(1,0); // Тест high level IO1
			break;
		
		case 58:
			individual_test_high_level_IO3_IO0(2,0);	// Тест high level IO2
			break;
		
		case 59:
			individual_test_high_level_IO3_IO0(3,0);	// Тест high level IO3
			break;
			
		case 60:
			individual_test_high_level_IO4_IO7(4, 0); // тест high level IO4
			break;
		
		case 61:
			individual_test_high_level_IO4_IO7(5, 0); // тест high level IO5
			break;
		
		case 62:
			individual_test_high_level_IO4_IO7(6, 0); // тест high level IO6
			break;
		
		case 63:
			individual_test_high_level_IO4_IO7(7, 0); // тест high level IO7
			break;
		
		case 64:
			individual_test_0_pwm(0); // тест pwm IO0
			break;
		
		case 65:
			individual_test_1_pwm(0); // тест pwm IO1
			break;
		
		case 66:
			individual_test_pwm_generic(IO2, 2, 500, 50, 0); // тест pwm IO2
			break;
		
		case 67:
			individual_test_pwm_generic(IO3, 3, 500, 50, 0); // тест pwm IO3
			break;
		
		case 68:
			individual_test_pwm_generic(IO4, 4, 500, 50, 0); // тест pwm IO4
			break;
		
		case 69:
			individual_test_pwm_generic(IO5, 5, 500, 50, 0); // тест pwm IO5
			break;
		
		case 70:
			individual_test_pwm_generic(IO6, 6, 500, 50, 0); // тест pwm IO6
			break;
		
		case 71:
			individual_test_pwm_generic(IO7, 7, 500, 50, 0); // тест pwm IO7
			break;
	}
	
}

