#ifndef __ENCODER_H
#define __ENCODER_H
#include "main.h"
#include "usart.h"

// 轮子转一整圈，编码器获得的脉冲数:30*11*2*2
// One full turn of the wheel, the number of pulses picked up by the coder: 30*13*2*2
#define ENCODER_CIRCLE (1040)

void Encoder_Update_Count(void);
int Encoder_Get_Count_Now(uint8_t Motor_id);
void Encoder_Get_ALL(int *Encoder_all);
void Encoder_Count_Set_Zero(void);
extern int speed_M1;
extern int speed_M2;
extern int speed_M3;
extern int speed_M4;

#endif 
