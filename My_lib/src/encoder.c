#include "encoder.h"
#include "motor.h"

int g_Encoder_M1_Now = 0;
int g_Encoder_M2_Now = 0;
int g_Encoder_M3_Now = 0;
int g_Encoder_M4_Now = 0;
int speed_M1 = 0;
int speed_M2 = 0;
int speed_M3 = 0;
int speed_M4 = 0;


/**
 * @Brief: To read the encoder count, call every 10 milliseconds  读取编码器计数，需每10毫秒调用一次
 * @Note:
 * @Parm: Motor id：电机的ID号:MOTOR_ID_M1
 * @Retval: Returns encoder count data  返回编码器计数数据
 */
static int16_t Encoder_Read_CNT(uint8_t Motor_id)
{
	int16_t Encoder_TIM = 0;
	switch (Motor_id)
	{
	case MOTOR_ID_M1:
		Encoder_TIM = 0x7fff - (short)TIM4->CNT;
		TIM4->CNT = 0x7fff;
		break;
	case MOTOR_ID_M2:
		Encoder_TIM = 0x7fff - (short)TIM2->CNT;
		TIM2->CNT = 0x7fff;
		break;
	case MOTOR_ID_M3:
		Encoder_TIM = 0x7fff - (short)TIM5->CNT;
		TIM5->CNT = 0x7fff;
		break;
	case MOTOR_ID_M4:
		Encoder_TIM = 0x7fff - (short)TIM3->CNT;
		TIM3->CNT = 0x7fff;
		break;
	default:
		break;
	}
	return Encoder_TIM;
}

// 返回开机到现在总共统计的编码器的计数（单路）。
// Returns the total count of encoders from boot up to now (single channel)
int Encoder_Get_Count_Now(uint8_t Motor_id)
{
	if (Motor_id == MOTOR_ID_M1)
		return g_Encoder_M1_Now;
	if (Motor_id == MOTOR_ID_M2)
		return g_Encoder_M2_Now;
	if (Motor_id == MOTOR_ID_M3)
		return g_Encoder_M3_Now;
	if (Motor_id == MOTOR_ID_M4)
		return g_Encoder_M4_Now;
	return 0;
}

// 获取开机到现在总共的四路编码器计数。
// Get the total four - way encoder count up to now
void Encoder_Get_ALL(int *Encoder_all)
{
	Encoder_all[0] = g_Encoder_M1_Now;
	Encoder_all[1] = g_Encoder_M2_Now;
	Encoder_all[2] = g_Encoder_M3_Now;
	Encoder_all[3] = g_Encoder_M4_Now;
}

// 更新编码器的计数总值。需每10毫秒调用一次
// Update the count value of the encoder. call every 10 milliseconds
void Encoder_Update_Count(void)
{
	speed_M1 = Encoder_Read_CNT(MOTOR_ID_M1);
	speed_M2 = Encoder_Read_CNT(MOTOR_ID_M2);
	speed_M3 = Encoder_Read_CNT(MOTOR_ID_M3);
	speed_M4 = Encoder_Read_CNT(MOTOR_ID_M4);
	
	g_Encoder_M1_Now -= speed_M1;
	g_Encoder_M2_Now -= speed_M2;
	g_Encoder_M3_Now += speed_M3;
	g_Encoder_M4_Now += speed_M4;
}

void Encoder_Count_Set_Zero(void)
{
	g_Encoder_M1_Now = 0;
	g_Encoder_M2_Now = 0;
	g_Encoder_M3_Now = 0;
	g_Encoder_M4_Now = 0;
}

