#include "car.h"

//�복��״̬������ر���

int refresh_yaw = 0;//�������µ�ǰ��yawֵ
int state = 1;
int go_strate_flag = 0;
int tracking_flag = 0;

//��IMU��صı���

float Yaw_angle = 0.0;//���������ԽǶ�Ϊ��λ��yaw
float cumulative_angle = 0.0;//�ۼ�yawֵ�ı仯��ʵ������ʽimu
float previous_angle = 0.0;//��������֮ǰ��yawֵ
float delta_angle = 0.0;//��������10ms�ڱ仯��yawֵ
float Yaw_now = 0;//�������浱ǰ��yawֵ

//�����ñ���
int kp_tracking =6;
uint16_t stop = 1;
/*
 * ��ʼ����ʱ��123458 Initialize TIM1.2.3.4.5.8
 * */
void Car_Init(void)

{
	// ����tim1��pwm��� Start the pwm output of tim1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	// ����tim8��pwm��� Start the pwm output of tim8
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

	TIM2->CNT = 0x7fff;
	// ����tim2�ı�����ģʽ Start the encoder mode of tim2
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM3->CNT = 0x7fff;
	// ����tim3�ı�����ģʽ Start the encoder mode of tim3
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	TIM4->CNT = 0x7fff;
	// ����tim4�ı�����ģʽ Start the encoder mode of tim4
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM5->CNT = 0x7fff;
	// ����tim5�ı�����ģʽ Start the encoder mode of tim5
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	
	//�ر�RGB��
	RGB_OFF_ALL;
	
	//���ô��ڽ����ж�1
	HAL_UART_Receive_IT(&huart1, &Received_Data_IMU, 1);
	
	//���ô��ڽ����ж�2
	HAL_UART_Receive_IT(&huart2, &Received_Data_Cam, 1);
	
	//������ʱ��6�ж�
	HAL_TIM_Base_Start_IT(&htim6);
	
	//OLED��ʼ��
	OLED_Init();
	
	//PID��ʼ��
	PID_Init(&OpenMv,0.2,0,0,0,40);
	PID_Init(&turn,-3,0,-9,0,70);//-0.5,-10,0,2,70
	PID_Init(&turn_fast,-2,-1,0,5,70);
	PID_Init(&speed_motor1,100,10,0,3600,3600);
	PID_Init(&speed_motor2,100,10,0,3600,3600);
	PID_Init(&speed_motor3,100,10,0,3600,3600);
	PID_Init(&speed_motor4,100,10,0,3600,3600);
	PID_Init(&location,0.01,0.1,0,2,30);
	
	//��ʼ����ɺ󴮿��������
	printf("car init finish\r\n");
}


//�복�Ŀ�����صĺ���

//�˺������ڿ���ÿ�����ӵ�ת�٣��ٶȷ�Χ-70��70
void Set_Car_Speed(int motor1,int motor2,int motor3,int motor4)
{
	PID_Calc(&speed_motor1,-motor1,speed_M1);

	PID_Calc(&speed_motor2,-motor2,speed_M2);

	PID_Calc(&speed_motor3,motor3,speed_M3);

	PID_Calc(&speed_motor4,motor4,speed_M4);
	
	Motion_Set_Pwm(-speed_motor1.output,-speed_motor2.output,speed_motor3.output,speed_motor4.output);
}

//�˺������ڿ��Ƴ�ԭ����ת���ٶ���������˳ʱ����ת����������ʱ����ת���ٶȵĴ�С������ת���ٶ�
void Car_Spin_Right(int16_t speed)
{
	Set_Car_Speed(speed, speed, -speed, -speed);
}



//�˺�������PID����С������ת�Ƕȣ��Ƕ�Ϊ��ʱ��˳ʱ�룬������ת
void Car_Right_Turn(float angle)
{
	PID_Calc(&turn,Yaw_now-angle,cumulative_angle);
	Car_Spin_Right((int16_t)turn.output);
}


//�˺�����������С����IMU�Ƕ�
void Car_Set_Yaw(float angle)
{
	PID_Calc(&turn,angle,cumulative_angle);
	Car_Spin_Right((int16_t)turn.output);
	
}

//�˺�����������С��ǰ���ķ����Լ�ǰ�����ٶ�
void Car_Go_Yaw(float yaw,int speed)
{
	
	PID_Calc(&turn,yaw,cumulative_angle);
	Set_Car_Speed(speed + (int16_t)turn.output,speed + (int16_t)turn.output,speed - (int16_t)turn.output,speed - (int16_t)turn.output);
}


//�˺������ڸ����ٶȵ�ת��
void Car_Go_Yaw_fast(float yaw,int speed)
{
	
	PID_Calc(&turn_fast,yaw,cumulative_angle);
	Set_Car_Speed(speed + (int16_t)turn.output,speed + (int16_t)turn.output,speed - (int16_t)turn.output,speed - (int16_t)turn.output);
}

//��IMU��صĺ���

//�˺�������ʵ������ʽ�ĽǶȲ�������Ҫ�ŵ���ʱ���ж���ÿ10ms����һ��
void Incremental_IMU_Measurement(void)
{
	Yaw_angle = Yaw / 32768.0 * 180.0;

	delta_angle = Yaw_angle - previous_angle;
	//ͻ���Ĵ����㷨
	if (delta_angle > 180.0)
	{
		delta_angle -= 360.0;
	}
	else if (delta_angle < -180.0)
	{
		delta_angle += 360.0;
	}
	
	cumulative_angle += delta_angle;

	previous_angle = Yaw_angle;
}



//����·�Ҷ�Ѳ����غ���
void four_tracking_refresh(uint8_t* L2,uint8_t* L1,uint8_t* R1,uint8_t* R2)
{
	*L2 = Read_irtracking(X2);
	*L1 = Read_irtracking(X1);
	*R2 = Read_irtracking(X4);
	*R1 = Read_irtracking(X3);
}
