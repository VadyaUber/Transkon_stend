#ifndef TEMP__H
#define TEMP__H

#include <math.h>

#include "functions.h"
#include "uart_.h"

void test_temp(void);
void individual_test_temp(float R_constant, RelayIndex relay_resist, uint8_t canal_number, uint8_t flag_next);

extern uint8_t counter_temp;

#endif 
