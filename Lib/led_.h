#ifndef LED__H
#define LED__H

#include "stdbool.h"
#include "gpio.h"



typedef enum 	// Перечисление для индексов lED
{
    KZ_Led = 0,
    ERROR_Led,
    GOOD_Led,
    CONNECT_Led,
    LED_COUNT // Общее количество lED
} LedIndex;


typedef struct { // Универсальная структура для реле
    GPIO_TypeDef* port;
    uint16_t pin;
    bool state;        // Текущее состояние реле
} Led;


void set_led_state(LedIndex led_name, PinState state);
void set_all_led(void);
void reset_all_led(void);

void GOOD_LED_ON(void);
void GOOD_LED_OFF(void);

void STATUS_LED_ON(void);
void STATUS_LED_OFF(void);
void TOGGLE_STATUS_LED(void);

#endif 
