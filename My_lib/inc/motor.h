#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "tim.h"

#define PWM_M1_A TIM8->CCR1
#define PWM_M1_B TIM8->CCR2
#define PWM_M2_A TIM8->CCR3
#define PWM_M2_B TIM8->CCR4

#define PWM_M3_A TIM1->CCR1
#define PWM_M3_B TIM1->CCR2
#define PWM_M4_A TIM1->CCR3
#define PWM_M4_B TIM1->CCR4

#define MOTOR_IGNORE_PULSE (1600)
#define MOTOR_MAX_PULSE (3600)
#define MOTOR_FREQ_DIVIDE (0)

typedef enum
{
	MOTOR_ID_M1 = 0,
	MOTOR_ID_M2,
	MOTOR_ID_M3,
	MOTOR_ID_M4,
	MAX_MOTOR
} Motor_ID;

void Motor_Set_Pwm(uint8_t id, int16_t speed);
void Motor_Stop(uint8_t brake);
void Motion_Set_Pwm(int16_t Motor_1, int16_t Motor_2, int16_t Motor_3, int16_t Motor_4);


#endif
