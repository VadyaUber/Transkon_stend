#ifndef IO__H
#define IO__H

#include "tim.h"

#include "functions.h"

void test_io(void);
void individual_io_test(uint16_t pwm_value, RelayIndex exclude_relay, uint8_t expected_IO[4], uint8_t flag_next);
void individual_test_high_level_IO3_IO0(uint8_t canal, uint8_t flag_next);
void individual_test_io_next(uint16_t pwm_value, RelayIndex exclude_relay, uint8_t expected_IO[4], uint8_t flag_next);
void individual_test_high_level_IO4_IO7(uint8_t canal, uint8_t flag_next);
extern uint8_t counter_io;

#endif 
