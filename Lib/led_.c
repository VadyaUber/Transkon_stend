#include "led_.h"


// Массив всех led
Led led_arr[LED_COUNT] = {
	{KZ_Led_GPIO_Port, KZ_Led_Pin, OFF},
	{ERROR_Led_GPIO_Port, ERROR_Led_Pin, OFF},
	{GOOD_Led_0_GPIO_Port, GOOD_Led_0_Pin, OFF},
	{CONNECT_Led_GPIO_Port, CONNECT_Led_Pin, OFF}
};


void set_led_state(LedIndex led_name, PinState state) // Универсальная функция управления led
{
	if (led_name >= LED_COUNT) return; // Проверка на корректность индекса
	
	Led* led = &led_arr[led_name]; // Получить указатель на реле
	led->state = state;               // Обновить текущее состояние

	HAL_GPIO_WritePin(led->port, led->pin, (GPIO_PinState)state);
}


void reset_all_led() // Сброс всех GPIO LED
{/*
	for (LedIndex i = KZ_Led; i < LED_COUNT; i++) 
	{
		HAL_GPIO_WritePin(led_arr[i].port, led_arr[i].pin, GPIO_PIN_RESET);
	}*/
	set_led_state(KZ_Led, OFF);
	set_led_state(ERROR_Led, OFF);
	GOOD_LED_OFF();
	set_led_state(CONNECT_Led, OFF);
}

void set_all_led() // Включение всех GPIO LED
{
	/*
	for (LedIndex i = KZ_Led; i < LED_COUNT; i++) 
	{
		HAL_GPIO_WritePin(led_arr[i].port, led_arr[i].pin, GPIO_PIN_SET);
	}*/
	set_led_state(KZ_Led, ON);
	set_led_state(ERROR_Led, ON);
	GOOD_LED_ON();
	set_led_state(CONNECT_Led, ON);
}

void GOOD_LED_ON()
{
	HAL_GPIO_WritePin(GOOD_Led_1_GPIO_Port, GOOD_Led_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GOOD_Led_0_GPIO_Port, GOOD_Led_0_Pin, GPIO_PIN_SET);
}

void GOOD_LED_OFF()
{
	HAL_GPIO_WritePin(GOOD_Led_1_GPIO_Port, GOOD_Led_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GOOD_Led_0_GPIO_Port, GOOD_Led_0_Pin, GPIO_PIN_RESET);
}

void STATUS_LED_ON()
{
	HAL_GPIO_WritePin(GOOD_Led_1_GPIO_Port, GOOD_Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GOOD_Led_0_GPIO_Port, GOOD_Led_0_Pin, GPIO_PIN_RESET);
}

void STATUS_LED_OFF()
{
	HAL_GPIO_WritePin(GOOD_Led_1_GPIO_Port, GOOD_Led_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GOOD_Led_0_GPIO_Port, GOOD_Led_0_Pin, GPIO_PIN_RESET);
}

void TOGGLE_STATUS_LED()
{
	static bool flag_;
	if (flag_)
	{
		flag_=!flag_;
		STATUS_LED_ON();
	}
	else
	{
		flag_=!flag_;
		STATUS_LED_OFF();
	}
	
}
