/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

typedef enum
{
		OFF = 0,
		ON = 1
} PinState;

extern uint16_t count_main;

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CONNECT_Led_Pin GPIO_PIN_13
#define CONNECT_Led_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_14
#define SPI1_CS_GPIO_Port GPIOC
#define SEL1_Pin GPIO_PIN_0
#define SEL1_GPIO_Port GPIOC
#define SEL2_Pin GPIO_PIN_1
#define SEL2_GPIO_Port GPIOC
#define IO2_Pin GPIO_PIN_2
#define IO2_GPIO_Port GPIOC
#define OE_Pin GPIO_PIN_3
#define OE_GPIO_Port GPIOC
#define GOOD_Led_1_Pin GPIO_PIN_0
#define GOOD_Led_1_GPIO_Port GPIOA
#define PC_UART2_EN_Pin GPIO_PIN_1
#define PC_UART2_EN_GPIO_Port GPIOA
#define PC_UART2_TX_Pin GPIO_PIN_2
#define PC_UART2_TX_GPIO_Port GPIOA
#define PC_UART2_RX_Pin GPIO_PIN_3
#define PC_UART2_RX_GPIO_Port GPIOA
#define Button_ReStart_Pin GPIO_PIN_4
#define Button_ReStart_GPIO_Port GPIOA
#define IO3_Pin GPIO_PIN_4
#define IO3_GPIO_Port GPIOC
#define IO4_Pin GPIO_PIN_5
#define IO4_GPIO_Port GPIOC
#define PWM_OUT_Pin GPIO_PIN_0
#define PWM_OUT_GPIO_Port GPIOB
#define KZ_Pin GPIO_PIN_1
#define KZ_GPIO_Port GPIOB
#define Button_Start_Pin GPIO_PIN_2
#define Button_Start_GPIO_Port GPIOB
#define R4_Pin GPIO_PIN_10
#define R4_GPIO_Port GPIOB
#define R5_Pin GPIO_PIN_11
#define R5_GPIO_Port GPIOB
#define SEL_RT1_Pin GPIO_PIN_12
#define SEL_RT1_GPIO_Port GPIOB
#define SEL_RT2_Pin GPIO_PIN_13
#define SEL_RT2_GPIO_Port GPIOB
#define SEL_RT3_Pin GPIO_PIN_14
#define SEL_RT3_GPIO_Port GPIOB
#define SEL_RT4_Pin GPIO_PIN_15
#define SEL_RT4_GPIO_Port GPIOB
#define IO5_Pin GPIO_PIN_6
#define IO5_GPIO_Port GPIOC
#define IO6_Pin GPIO_PIN_7
#define IO6_GPIO_Port GPIOC
#define IO7_Pin GPIO_PIN_8
#define IO7_GPIO_Port GPIOC
#define Transcon_UART1_EN_Pin GPIO_PIN_8
#define Transcon_UART1_EN_GPIO_Port GPIOA
#define Transcon_UART1_TX_Pin GPIO_PIN_9
#define Transcon_UART1_TX_GPIO_Port GPIOA
#define Transcon_UART1_RX_Pin GPIO_PIN_10
#define Transcon_UART1_RX_GPIO_Port GPIOA
#define KZ_Relay_Pin GPIO_PIN_11
#define KZ_Relay_GPIO_Port GPIOA
#define KZ_Led_Pin GPIO_PIN_10
#define KZ_Led_GPIO_Port GPIOC
#define ERROR_Led_Pin GPIO_PIN_11
#define ERROR_Led_GPIO_Port GPIOC
#define GOOD_Led_0_Pin GPIO_PIN_12
#define GOOD_Led_0_GPIO_Port GPIOC
#define IN_PWM2_Pin GPIO_PIN_2
#define IN_PWM2_GPIO_Port GPIOD
#define R1_Pin GPIO_PIN_5
#define R1_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_6
#define R2_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_7
#define R3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
