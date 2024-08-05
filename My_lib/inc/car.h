#ifndef __CAR_H
#define __CAR_H

#include "tim.h"
#include "bsp_rgb.h"
#include "bsp_buzzer.h"
#include "key.h"
#include "motor.h"
#include "encoder.h"
#include "four_tracking.h"
#include "delay.h"
#include "oled.h"
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "PID.h"

//与车的状态控制相关变量
extern int refresh_yaw;
extern int state;
extern int go_strate_flag;
extern float Yaw_now;
extern int tracking_flag;

//与IMU相关的变量
extern float Yaw_angle;
extern float cumulative_angle;
extern float previous_angle;
extern float delta_angle;

extern int kp_tracking;
extern uint16_t stop;

void Incremental_IMU_Measurement(void);
void Car_Init(void);
void Set_Car_Speed(int motor1,int motor2,int motor3,int motor4);
void Car_Spin_Right(int16_t speed);
void Car_Right_Turn(float angle);
void Car_Set_Yaw(float angle);
void Car_Go_Yaw(float yaw,int speed);
void Car_Go_Yaw_fast(float yaw,int speed);
void four_tracking_refresh(uint8_t* L2,uint8_t* L1,uint8_t* R1,uint8_t* R2);


#endif 
