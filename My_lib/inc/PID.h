#ifndef __PID_H
#define __PID_H

#include "car.h"

typedef struct
{
   	float kp, ki, kd; //����ϵ��
    float error, lastError; //���ϴ����
    float integral, maxIntegral; //���֡������޷�
    float output, maxOutput; //���������޷�
}PID;

void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut);

void PID_Calc(PID *pid, float reference, float feedback);

extern PID OpenMv;
extern PID turn;
extern PID speed_motor1;
extern PID speed_motor2;
extern PID speed_motor3;
extern PID speed_motor4;
extern PID location;
extern PID turn_fast;

#endif 
