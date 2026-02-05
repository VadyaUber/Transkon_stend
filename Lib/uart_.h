#ifndef UART__H
#define UART__H

#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"

#include <ModbusRTU_Slave.h>

#define rx_buffer_size 35  //буфер uart транскон 
extern uint8_t rx_buffer[rx_buffer_size];

extern bool flag_get_data;

void send_transkon(char *data, uint8_t size);

#endif 
