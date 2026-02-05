#include "relay_.h"

shift_register_ shift_reg = { .val = 0 };	// Глобальная переменная для сдвигового регистра


void send_shift_register() // Функция для отправки данных через SPI
{
    uint8_t aTxBuffer[1] = { shift_reg.val };
    
    HAL_GPIO_WritePin(GPIOC, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t*)aTxBuffer, 1, 10);
    HAL_GPIO_WritePin(GPIOC, SPI1_CS_Pin, GPIO_PIN_SET);
}

	
// Массив всех реле
Relay relays_arr[RELAY_COUNT] = {
    {RELAY_TYPE_GPIO, R1_GPIO_Port, R1_Pin, OFF},
    {RELAY_TYPE_GPIO, R2_GPIO_Port, R2_Pin, OFF},
    {RELAY_TYPE_GPIO, R3_GPIO_Port, R3_Pin, OFF},
    {RELAY_TYPE_GPIO, R4_GPIO_Port, R4_Pin, OFF},
    {RELAY_TYPE_GPIO, R5_GPIO_Port, R5_Pin, OFF},
    {RELAY_TYPE_GPIO, SEL_RT1_GPIO_Port, SEL_RT1_Pin, OFF},
    {RELAY_TYPE_GPIO, SEL_RT2_GPIO_Port, SEL_RT2_Pin, OFF},
    {RELAY_TYPE_GPIO, SEL_RT3_GPIO_Port, SEL_RT3_Pin, OFF},
    {RELAY_TYPE_GPIO, SEL_RT4_GPIO_Port, SEL_RT4_Pin, OFF},
    {RELAY_TYPE_GPIO, SEL1_GPIO_Port, SEL1_Pin, OFF},
    {RELAY_TYPE_GPIO, SEL2_GPIO_Port, SEL2_Pin, OFF},
    {RELAY_TYPE_GPIO, IO2_GPIO_Port, IO2_Pin, OFF},
    {RELAY_TYPE_GPIO, IO3_GPIO_Port, IO3_Pin, OFF},
    {RELAY_TYPE_GPIO, IO4_GPIO_Port, IO4_Pin, OFF},
    {RELAY_TYPE_GPIO, IO5_GPIO_Port, IO5_Pin, OFF},
    {RELAY_TYPE_GPIO, IO6_GPIO_Port, IO6_Pin, OFF},
    {RELAY_TYPE_GPIO, IO7_GPIO_Port, IO7_Pin, OFF},
		{RELAY_TYPE_GPIO, KZ_Relay_GPIO_Port, KZ_Relay_Pin, OFF},
    {RELAY_TYPE_SHIFT_REG, NULL, 0, OFF}, // IO0 через сдвиговый регистр
    {RELAY_TYPE_SHIFT_REG, NULL, 0, OFF}, // IO1 через сдвиговый регистр
    {RELAY_TYPE_SHIFT_REG, NULL, 0, OFF}, // A15 через сдвиговый регистр
    {RELAY_TYPE_SHIFT_REG, NULL, 0, OFF}, // A14 через сдвиговый регистр
    {RELAY_TYPE_SHIFT_REG, NULL, 0, OFF}  // KL15 через сдвиговый регистр
};



void set_relay_state(RelayIndex relay_name, PinState state) // Универсальная функция управления реле
{
    if (relay_name >= RELAY_COUNT) return; // Проверка на корректность индекса

    Relay* relay = &relays_arr[relay_name]; // Получить указатель на реле
    relay->state = state;               // Обновить текущее состояние

    if (relay->type == RELAY_TYPE_GPIO) // Управление реле через GPIO
		{
        HAL_GPIO_WritePin(relay->port, relay->pin, (GPIO_PinState)state);
    } 
		else if (relay->type == RELAY_TYPE_SHIFT_REG) // Управление реле через сдвиговый регистр
		{
        switch(relay_name) 
				{
            case IO2: shift_reg.IO2 = state; break;
            case IO1: shift_reg.IO1 = state; break;
            case A15: shift_reg.A15 = state; break;
            case IO0: shift_reg.IO0 = state; break;
            case KL15: shift_reg.KL15 = state; break;
            default: break;
        }
        send_shift_register();	// Отправить данные в сдвиговый регистр
    }
}


void reset_all_relays() // Сброс всех GPIO реле
{
    for (RelayIndex i = R1; i < RELAY_COUNT; i++) 
		{
        if (relays_arr[i].type == RELAY_TYPE_GPIO) 
				{
            HAL_GPIO_WritePin(relays_arr[i].port, relays_arr[i].pin, GPIO_PIN_RESET);
        } 
				else if (relays_arr[i].type == RELAY_TYPE_SHIFT_REG) 
				{
           switch(i) 
						{
							case IO2: shift_reg.IO2 = OFF; break;
							case IO1: shift_reg.IO1 = OFF; break;
							case A15: shift_reg.A15 = OFF; break;
							case IO0: shift_reg.IO0 = OFF; break;
							case KL15: shift_reg.KL15 = OFF; break;
							default: break;
						}
        }
        relays_arr[i].state = OFF; // Обновляем состояние
    }
    send_shift_register();	// Отправляем обновленный сдвиговый регистр
}

void toggle_relay_state(RelayIndex relay_name) // Toggle relay pin
{
    if (relay_name >= RELAY_COUNT) return; // Проверка на корректность индекса

    Relay* relay = &relays_arr[relay_name]; // Получить указатель на реле
	
    HAL_GPIO_TogglePin(relay->port, relay->pin);  
}
