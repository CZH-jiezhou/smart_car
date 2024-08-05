/*
 * bsp_buzzer.c
 *
 *  Created on: Oct 9, 2023
 *      Author: YB-101
 */

#include "bsp_buzzer.h"
#include "main.h"
/*
 * 开启或者关闭蜂鸣器 Turn on or turn off the buzzer
 * i：0或者1 0：关闭蜂鸣器 1：打开蜂鸣器 i: 0 or 1 0: turn off the buzzer 1: turn on the buzzer
 * */
void Set_Buzzer(uint8_t i)
{
	if (i == 0)
	{
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET); // 关闭蜂鸣器 turn off the buzzer
	}
	else
	{
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET); // 开启蜂鸣器 turn on the buzzer
	}
}
