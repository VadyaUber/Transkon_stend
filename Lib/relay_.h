#ifndef RELAY__H
#define RELAY__H

#include "spi.h"
#include "stdbool.h"
#pragma anon_unions


typedef union{
    struct {
        bool reserv0:1; 
        bool IO2:1;  //IO0   // Реле через сдвиговый 
        bool IO1:1;
				bool A15:1;
			  bool IO0:1; //A14
				bool KL15:1;
				bool reserv1:1;
				bool reserv2:1; 
    };
    uint8_t val;	
}shift_register_;	// Сдвиговый регистр




typedef enum 	// Перечисление для индексов реле
{
    R1 = 0,
    R2,
    R3,
    R4,
    R5,
		SEL_RT1,
		SEL_RT2,
		SEL_RT3,
		SEL_RT4,
		SEL1,
		SEL2,
		IO4, //IO2
		IO3,
		IO6, //IO4
		IO5,
		A14, //IO6
		IO7,
		KZ,
		IO2, //IO0    //сдвиговый регистр
		IO1,
		A15,
		IO0, //A14
		KL15,
    RELAY_COUNT // Общее количество реле
} RelayIndex;


typedef enum {	// Тип управления реле
    RELAY_TYPE_GPIO,      // Управляется через GPIO
    RELAY_TYPE_SHIFT_REG  // Управляется через сдвиговый регистр
} RelayType;


typedef struct { // Универсальная структура для реле
    RelayType type;
    GPIO_TypeDef* port;
    uint16_t pin;
    bool state;        // Текущее состояние реле
} Relay;


void set_relay_state(RelayIndex relay, PinState state);
void reset_all_relays(void);

void toggle_relay_state(RelayIndex relay_name);

#endif 
