#ifndef PWM_l_H
#define PMW_l_H

#include "functions.h"

void test_pwm(void);
void individual_test_pwm_generic(RelayIndex pin, uint8_t pwmIndexTarget, uint16_t pwmFreqTarget, uint8_t pwmDutyTarget, uint8_t flag_next);
void individual_test_0_pwm(uint8_t flag_next);
void individual_test_1_pwm(uint8_t flag_next);
extern uint8_t counter_pwm;
	
#endif 
