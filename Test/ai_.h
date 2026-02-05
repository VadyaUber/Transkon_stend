#ifndef AI__H
#define AI__H

#include "cmath"
#include "tim.h"
#include "functions.h"

void test_ai(void);
void individual_test_ai(RelayIndex relay, uint16_t pwm_value, uint16_t pwm_constant, char* send_comand, char* get_comand, uint8_t flag_next);

extern uint16_t pwm_4v_constant;
extern uint16_t pwm_12v_constant;
extern uint16_t pwm_20v_constant;
extern float prothent_ai;
extern uint8_t counter_ai;

#endif 
