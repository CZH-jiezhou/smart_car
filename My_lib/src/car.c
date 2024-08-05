#include "car.h"

//与车的状态控制相关变量

int refresh_yaw = 0;//用来更新当前的yaw值
int state = 1;
int go_strate_flag = 0;
int tracking_flag = 0;

//与IMU相关的变量

float Yaw_angle = 0.0;//用来储存以角度为单位的yaw
float cumulative_angle = 0.0;//累加yaw值的变化以实现增量式imu
float previous_angle = 0.0;//用来储存之前的yaw值
float delta_angle = 0.0;//用来储存10ms内变化的yaw值
float Yaw_now = 0;//用来储存当前的yaw值

//调参用变量
int kp_tracking =6;
uint16_t stop = 1;
/*
 * 初始化定时器123458 Initialize TIM1.2.3.4.5.8
 * */
void Car_Init(void)

{
	// 启动tim1的pwm输出 Start the pwm output of tim1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	// 启动tim8的pwm输出 Start the pwm output of tim8
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

	TIM2->CNT = 0x7fff;
	// 启动tim2的编码器模式 Start the encoder mode of tim2
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM3->CNT = 0x7fff;
	// 启动tim3的编码器模式 Start the encoder mode of tim3
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	TIM4->CNT = 0x7fff;
	// 启动tim4的编码器模式 Start the encoder mode of tim4
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);

	TIM5->CNT = 0x7fff;
	// 启动tim5的编码器模式 Start the encoder mode of tim5
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	
	//关闭RGB灯
	RGB_OFF_ALL;
	
	//启用串口接收中断1
	HAL_UART_Receive_IT(&huart1, &Received_Data_IMU, 1);
	
	//启用串口接收中断2
	HAL_UART_Receive_IT(&huart2, &Received_Data_Cam, 1);
	
	//开启定时器6中断
	HAL_TIM_Base_Start_IT(&htim6);
	
	//OLED初始化
	OLED_Init();
	
	//PID初始化
	PID_Init(&OpenMv,0.2,0,0,0,40);
	PID_Init(&turn,-3,0,-9,0,70);//-0.5,-10,0,2,70
	PID_Init(&turn_fast,-2,-1,0,5,70);
	PID_Init(&speed_motor1,100,10,0,3600,3600);
	PID_Init(&speed_motor2,100,10,0,3600,3600);
	PID_Init(&speed_motor3,100,10,0,3600,3600);
	PID_Init(&speed_motor4,100,10,0,3600,3600);
	PID_Init(&location,0.01,0.1,0,2,30);
	
	//初始化完成后串口输出测试
	printf("car init finish\r\n");
}


//与车的控制相关的函数

//此函数用于控制每个轮子的转速，速度范围-70到70
void Set_Car_Speed(int motor1,int motor2,int motor3,int motor4)
{
	PID_Calc(&speed_motor1,-motor1,speed_M1);

	PID_Calc(&speed_motor2,-motor2,speed_M2);

	PID_Calc(&speed_motor3,motor3,speed_M3);

	PID_Calc(&speed_motor4,motor4,speed_M4);
	
	Motion_Set_Pwm(-speed_motor1.output,-speed_motor2.output,speed_motor3.output,speed_motor4.output);
}

//此函数用于控制车原地旋转，速度是正数则顺时针旋转，负数则逆时针旋转，速度的大小控制旋转的速度
void Car_Spin_Right(int16_t speed)
{
	Set_Car_Speed(speed, speed, -speed, -speed);
}



//此函数用于PID控制小车的旋转角度，角度为正时是顺时针，向右旋转
void Car_Right_Turn(float angle)
{
	PID_Calc(&turn,Yaw_now-angle,cumulative_angle);
	Car_Spin_Right((int16_t)turn.output);
}


//此函数用于设置小车的IMU角度
void Car_Set_Yaw(float angle)
{
	PID_Calc(&turn,angle,cumulative_angle);
	Car_Spin_Right((int16_t)turn.output);
	
}

//此函数用于设置小车前进的方向以及前进的速度
void Car_Go_Yaw(float yaw,int speed)
{
	
	PID_Calc(&turn,yaw,cumulative_angle);
	Set_Car_Speed(speed + (int16_t)turn.output,speed + (int16_t)turn.output,speed - (int16_t)turn.output,speed - (int16_t)turn.output);
}


//此函数用于更快速度的转向
void Car_Go_Yaw_fast(float yaw,int speed)
{
	
	PID_Calc(&turn_fast,yaw,cumulative_angle);
	Set_Car_Speed(speed + (int16_t)turn.output,speed + (int16_t)turn.output,speed - (int16_t)turn.output,speed - (int16_t)turn.output);
}

//与IMU相关的函数

//此函数用于实现增量式的角度测量，需要放到定时器中断里每10ms调用一次
void Incremental_IMU_Measurement(void)
{
	Yaw_angle = Yaw / 32768.0 * 180.0;

	delta_angle = Yaw_angle - previous_angle;
	//突变点的处理算法
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



//与四路灰度巡线相关函数
void four_tracking_refresh(uint8_t* L2,uint8_t* L1,uint8_t* R1,uint8_t* R2)
{
	*L2 = Read_irtracking(X2);
	*L1 = Read_irtracking(X1);
	*R2 = Read_irtracking(X4);
	*R1 = Read_irtracking(X3);
}
