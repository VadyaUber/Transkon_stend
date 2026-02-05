#include "button_.h"

// Вспомогательная функция для обработки кнопки
bool processButton(GPIO_TypeDef *port, uint32_t pin, bool *flag, uint32_t *timer, uint32_t debounceTime)
{
    bool btnState = port->IDR & pin; // Считываем состояние кнопки

    if (btnState && !(*flag) && HAL_GetTick() - *timer > debounceTime)
    {
        *flag = true;
        *timer = HAL_GetTick();
        return false; // нажатие зафиксировано
    }
    if (!btnState && *flag && HAL_GetTick() - *timer > debounceTime)
    {
        *flag = false;
        *timer = HAL_GetTick();
        return true; // отпускание зафиксировано
    }
    return false; // ничего не произошло
}

// Оптимизированная функция button_start
void button_start()
{
    static bool flag = false;
    static uint32_t btnTimer = 0;

    if (processButton(GPIOB, GPIO_PIN_2, &flag, &btnTimer, 100)) // Кнопка старта PB2
    {
        if (str0.flag_test == 0)
        {
            str0.flag_test = 1; // начало теста
        }
    }
}

// Оптимизированная функция button_reset
void button_reset()
{
    if (str0.flag_test == 0)
    {
        static bool flag = false;
        static uint32_t btnTimer = 0;

        if (processButton(GPIOA, GPIO_PIN_4, &flag, &btnTimer, 100)) // Кнопка сброса PA4
        {
          reset(); // сброс
					obnull();
					
        }
    }
}
