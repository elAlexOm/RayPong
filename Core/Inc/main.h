/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EN_3V3_Pin GPIO_PIN_13
#define EN_3V3_GPIO_Port GPIOC
#define EN_5V0_Pin GPIO_PIN_14
#define EN_5V0_GPIO_Port GPIOC
#define ESP_UART6_TX_Pin GPIO_PIN_0
#define ESP_UART6_TX_GPIO_Port GPIOC
#define ESP_UART6_RX_Pin GPIO_PIN_1
#define ESP_UART6_RX_GPIO_Port GPIOC
#define MUTE_Pin GPIO_PIN_2
#define MUTE_GPIO_Port GPIOC
#define SHDN_Pin GPIO_PIN_3
#define SHDN_GPIO_Port GPIOC
#define ESP_PD_Pin GPIO_PIN_0
#define ESP_PD_GPIO_Port GPIOA
#define ESP_RST_Pin GPIO_PIN_1
#define ESP_RST_GPIO_Port GPIOA
#define BAT_LVL_Pin GPIO_PIN_2
#define BAT_LVL_GPIO_Port GPIOA
#define LED_PWR_Pin GPIO_PIN_3
#define LED_PWR_GPIO_Port GPIOA
#define LED_PWM_TIMER_Pin GPIO_PIN_6
#define LED_PWM_TIMER_GPIO_Port GPIOA
#define LED_PWM_SPI_Pin GPIO_PIN_7
#define LED_PWM_SPI_GPIO_Port GPIOA
#define ADC_IN_Pin GPIO_PIN_4
#define ADC_IN_GPIO_Port GPIOC
#define PWR_ON_Pin GPIO_PIN_5
#define PWR_ON_GPIO_Port GPIOC
#define PWR_ON_EXTI_IRQn EXTI4_15_IRQn
#define PLEER_1_Pin GPIO_PIN_0
#define PLEER_1_GPIO_Port GPIOB
#define PLEER_1_EXTI_IRQn EXTI0_1_IRQn
#define PLEER_2_Pin GPIO_PIN_1
#define PLEER_2_GPIO_Port GPIOB
#define PLEER_2_EXTI_IRQn EXTI0_1_IRQn
#define W25_WP_Pin GPIO_PIN_6
#define W25_WP_GPIO_Port GPIOC
#define W25_HOLD_Pin GPIO_PIN_7
#define W25_HOLD_GPIO_Port GPIOC
#define VOLUME_PWM_Pin GPIO_PIN_8
#define VOLUME_PWM_GPIO_Port GPIOC
#define LATCH_Pin GPIO_PIN_9
#define LATCH_GPIO_Port GPIOC
#define CLEAR_Pin GPIO_PIN_8
#define CLEAR_GPIO_Port GPIOA
#define CLK_Pin GPIO_PIN_9
#define CLK_GPIO_Port GPIOA
#define S_DATA1_Pin GPIO_PIN_10
#define S_DATA1_GPIO_Port GPIOA
#define S_DATA2_Pin GPIO_PIN_11
#define S_DATA2_GPIO_Port GPIOA
#define S_DATA3_Pin GPIO_PIN_12
#define S_DATA3_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOB
#define LED_Y_Pin GPIO_PIN_6
#define LED_Y_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define USARTx                            USART6

#define WIFI_SSID                         "elAlex"
#define WIFI_PASSWORD                     "K8ei5a5ejh"

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
