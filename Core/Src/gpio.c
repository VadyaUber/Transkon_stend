/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CONNECT_Led_Pin|SPI1_CS_Pin|SEL1_Pin|SEL2_Pin
                          |IO2_Pin|OE_Pin|IO3_Pin|IO4_Pin
                          |IO5_Pin|IO6_Pin|IO7_Pin|KZ_Led_Pin
                          |ERROR_Led_Pin|GOOD_Led_0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GOOD_Led_1_Pin|PC_UART2_EN_Pin|Transcon_UART1_EN_Pin|KZ_Relay_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, R4_Pin|R5_Pin|SEL_RT1_Pin|SEL_RT2_Pin
                          |SEL_RT3_Pin|SEL_RT4_Pin|R1_Pin|R2_Pin
                          |R3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CONNECT_Led_Pin SPI1_CS_Pin SEL1_Pin SEL2_Pin
                           IO2_Pin OE_Pin IO3_Pin IO4_Pin
                           IO5_Pin IO6_Pin IO7_Pin KZ_Led_Pin
                           ERROR_Led_Pin GOOD_Led_0_Pin */
  GPIO_InitStruct.Pin = CONNECT_Led_Pin|SPI1_CS_Pin|SEL1_Pin|SEL2_Pin
                          |IO2_Pin|OE_Pin|IO3_Pin|IO4_Pin
                          |IO5_Pin|IO6_Pin|IO7_Pin|KZ_Led_Pin
                          |ERROR_Led_Pin|GOOD_Led_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : GOOD_Led_1_Pin PC_UART2_EN_Pin Transcon_UART1_EN_Pin KZ_Relay_Pin */
  GPIO_InitStruct.Pin = GOOD_Led_1_Pin|PC_UART2_EN_Pin|Transcon_UART1_EN_Pin|KZ_Relay_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Button_ReStart_Pin */
  GPIO_InitStruct.Pin = Button_ReStart_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button_ReStart_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KZ_Pin Button_Start_Pin */
  GPIO_InitStruct.Pin = KZ_Pin|Button_Start_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : R4_Pin R5_Pin SEL_RT1_Pin SEL_RT2_Pin
                           SEL_RT3_Pin SEL_RT4_Pin R1_Pin R2_Pin
                           R3_Pin */
  GPIO_InitStruct.Pin = R4_Pin|R5_Pin|SEL_RT1_Pin|SEL_RT2_Pin
                          |SEL_RT3_Pin|SEL_RT4_Pin|R1_Pin|R2_Pin
                          |R3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
