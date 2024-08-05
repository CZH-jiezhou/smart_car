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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RRGB_R_Pin GPIO_PIN_2
#define RRGB_R_GPIO_Port GPIOE
#define RRGB_G_Pin GPIO_PIN_3
#define RRGB_G_GPIO_Port GPIOE
#define RRGB_B_Pin GPIO_PIN_4
#define RRGB_B_GPIO_Port GPIOE
#define X1_Pin GPIO_PIN_13
#define X1_GPIO_Port GPIOF
#define X2_Pin GPIO_PIN_14
#define X2_GPIO_Port GPIOF
#define X3_Pin GPIO_PIN_15
#define X3_GPIO_Port GPIOF
#define X4_Pin GPIO_PIN_0
#define X4_GPIO_Port GPIOG
#define LRGB_R_Pin GPIO_PIN_1
#define LRGB_R_GPIO_Port GPIOG
#define LRGB_G_Pin GPIO_PIN_7
#define LRGB_G_GPIO_Port GPIOE
#define LRGB_B_Pin GPIO_PIN_2
#define LRGB_B_GPIO_Port GPIOG
#define key1_Pin GPIO_PIN_3
#define key1_GPIO_Port GPIOG
#define key1_EXTI_IRQn EXTI3_IRQn
#define key2_Pin GPIO_PIN_4
#define key2_GPIO_Port GPIOG
#define key2_EXTI_IRQn EXTI4_IRQn
#define key3_Pin GPIO_PIN_5
#define key3_GPIO_Port GPIOG
#define key3_EXTI_IRQn EXTI9_5_IRQn
#define Buzzer_Pin GPIO_PIN_12
#define Buzzer_GPIO_Port GPIOG
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOG
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define MPU6050_AD0_Pin GPIO_PIN_1
#define MPU6050_AD0_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
extern float cumulative_angle;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
