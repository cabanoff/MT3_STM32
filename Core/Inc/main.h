/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define ADC_DRDY_Pin GPIO_PIN_10
#define ADC_DRDY_GPIO_Port GPIOB
#define ADC_RESET_Pin GPIO_PIN_12
#define ADC_RESET_GPIO_Port GPIOB
#define WIRE_VERIF_Pin GPIO_PIN_6
#define WIRE_VERIF_GPIO_Port GPIOC
#define FREQ_OUT_Pin GPIO_PIN_8
#define FREQ_OUT_GPIO_Port GPIOA
#define RANGE_SELECT_Pin GPIO_PIN_10
#define RANGE_SELECT_GPIO_Port GPIOC
#define SYNC_IN_Pin GPIO_PIN_2
#define SYNC_IN_GPIO_Port GPIOD
#define PULSE_COUNT_Pin GPIO_PIN_6
#define PULSE_COUNT_GPIO_Port GPIOB
#define FIRST_EDGE_Pin GPIO_PIN_7
#define FIRST_EDGE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
