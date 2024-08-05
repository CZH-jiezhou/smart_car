/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;

extern UART_HandleTypeDef huart5;

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
	
//UART1 IMU

extern uint8_t Received_Data_IMU;
extern int16_t acc_x;
extern int16_t acc_y;
extern int16_t acc_z;
extern int16_t angle_vx;
extern int16_t angle_vy;
extern int16_t angle_vz;
extern int16_t Role;
extern int16_t Pitch;
extern int16_t Yaw;

//UART2 K210/Other Cam

extern uint8_t mode;
extern uint8_t Received_Data_Cam;

//UART3 OpenMv

extern uint8_t Received_Data_OpenMv;
extern uint8_t X;
extern uint8_t Width;

//UART4 Screen

//UART5 Bluetooth



int Data_Test(uint8_t end);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
int IMU_Init_Yaw(void);
/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

