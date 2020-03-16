/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

#include <stdbool.h>	
#include "gpio.h"
#include "tim.h"
	
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
#define SW_Pin GPIO_PIN_4
#define SW_GPIO_Port GPIOA
#define MQ7_CTL_Pin GPIO_PIN_0
#define MQ7_CTL_GPIO_Port GPIOB
#define PMC7003_RST_Pin GPIO_PIN_1
#define PMC7003_RST_GPIO_Port GPIOB
#define PMC7003_SET_Pin GPIO_PIN_2
#define PMC7003_SET_GPIO_Port GPIOB
#define KQ6600_F_Pin GPIO_PIN_13
#define KQ6600_F_GPIO_Port GPIOB
#define LCD_BACK_Pin GPIO_PIN_14
#define LCD_BACK_GPIO_Port GPIOB
#define LCD_DATA_Pin GPIO_PIN_15
#define LCD_DATA_GPIO_Port GPIOB
#define LCD_WR_Pin GPIO_PIN_8
#define LCD_WR_GPIO_Port GPIOA
#define LCD_RD_Pin GPIO_PIN_11
#define LCD_RD_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

void MX_NVIC_Init(void);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
