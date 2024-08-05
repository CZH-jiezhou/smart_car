/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "car.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//distance related variable
int Car_Distance[4] = {0,0,0,0};//used for store 4 motors' distance
int Total_Distance = 0;//used for store toltal distance

//oled related variable
char show_buff [20] ={'\0'};//oled show buff
char* init = "please wait";// system init information

//four_tracking related variable
uint8_t L2 = 0;
uint8_t L1 = 0;
uint8_t R2 = 0;
uint8_t R1 = 0;

float Tracking_Error_Integral = 0;
float Tracking_Error_Kp = 0;

int count = 0;

float Yaw_goal = 0;

//light and sounds variable
RGB_Color current_color = red;
uint8_t buzzer_on = 0;
uint32_t signal_timer = 0;
const uint32_t signal_duration = 500; // Signal duration in milliseconds

//µ÷²ÎÓÃ
int speed_low = 17;
int speed_hight = 30;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6)
	{
		if(uwTick > 100)//skip a lital time to wait system stable
		{
			//Get total distance
			Encoder_Update_Count();
			Encoder_Get_ALL(Car_Distance);
			Total_Distance = (Car_Distance[0] + Car_Distance[1] + Car_Distance[2] + Car_Distance[3])/4;
			
			//Get cumulative_angle
			Incremental_IMU_Measurement();
			if(refresh_yaw == 1)
			{
				Yaw_now = cumulative_angle;
				refresh_yaw = 0;
				Encoder_Count_Set_Zero();
			}
			
			//Get four_tracking data
			four_tracking_refresh(&L2,&L1,&R1,&R2);
			
			if(L1 == 0 && R1 == 1 && R2 == 1 && L2 == 1)
			{
				Tracking_Error_Integral += 1;
			}
			else if(L1 == 1 && R1 == 0 && R2 == 1 && L2 == 1)
			{
				Tracking_Error_Integral -= 1;
			}
			else if(L1 == 0 && R1 == 0 && R2 == 1 && L2 == 1)
			{
				Tracking_Error_Integral = 0;
			}
			else if(L1 == 1 && R1 == 1 && R2 == 0 && L2 == 1)
			{
				Tracking_Error_Integral -= 2;
			}
			else if(L1 == 1 && R1 == 1 && R2 == 1 && L2 == 0)
			{
				Tracking_Error_Integral += 2;
			}
		
			
			if(mode == 5)
			{
				
				if(state == 0)
				{
					Set_Car_Speed(0,0,0,0);
					current_color = red; // Set the color for the next signal
					buzzer_on = 1; // Turn on the buzzer
					signal_timer = HAL_GetTick(); // Set the signal timer
				}
			
				if(state == 1)
				{
				
					if(count == 0)
					{
						Yaw_goal = -37;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 1)
					{
						Yaw_goal = 217;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 2)
					{
						Yaw_goal = -38;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 3)
					{
						Yaw_goal = 217;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 4)
					{
						Yaw_goal = -38;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 5)
					{
						Yaw_goal = 217;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 6)
					{
						Yaw_goal = -39;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 7)
					{
						Yaw_goal = 217;
						Car_Go_Yaw(Yaw_goal,60);
					}
					else if(count == 8)
					{
						state = 0;
					}

					
					if(Total_Distance >= 8832)
					{
						count ++;
						state  = 2;
						current_color = green; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
						Tracking_Error_Integral = 0;
					}
				}
				
				if(state == 2)
				{
					if(count == 1)
					{
						Set_Car_Speed(27-kp_tracking*(int)Tracking_Error_Integral,27-kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral);

					}
					
					else if(count == 2)
					{
						Set_Car_Speed(50-kp_tracking*(int)Tracking_Error_Integral,50-kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral);
					}
					
					else if(count == 3)
					{
						Set_Car_Speed(27-kp_tracking*(int)Tracking_Error_Integral,27-kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral);
					}
					
					else if(count == 4)
					{
						Set_Car_Speed(50-kp_tracking*(int)Tracking_Error_Integral,50-kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral);
					}
					
					else if(count == 5)
					{
						Set_Car_Speed(27-kp_tracking*(int)Tracking_Error_Integral,27-kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral);
					}
					
					else if(count == 6)
					{
						Set_Car_Speed(50-kp_tracking*(int)Tracking_Error_Integral,50-kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral);
					}
					
					else if(count == 7)
					{
						Set_Car_Speed(27-kp_tracking*(int)Tracking_Error_Integral,27-kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral,50+kp_tracking*(int)Tracking_Error_Integral);
					}
					
					else if(count == 8)
					{
						Set_Car_Speed(50-kp_tracking*(int)Tracking_Error_Integral,50-kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral,27+kp_tracking*(int)Tracking_Error_Integral);
					}
					
					if(Total_Distance >= 16932 && (R2 + R1 + L1 + L2) == 4)
					{
						state = 1;
						Encoder_Count_Set_Zero();
						current_color = blue; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
					}
				}
			}
			
			if(mode == 4)
			{
				if(state == 100)
				{
					Set_Car_Speed(0,0,0,0);
				}
				
				if(state == 0)
				{
					Set_Car_Speed(0,0,0,0);
					current_color = red; // Set the color for the next signal
					buzzer_on = 1; // Turn on the buzzer
					signal_timer = HAL_GetTick(); // Set the signal timer
					state = 100;
				}
			
				if(state == 1)
				{
				
					if(count == 0)
					{
						Yaw_goal = -36;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 1)
					{
						Yaw_goal = 215;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 2)
					{
						Yaw_goal = -38;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 3)
					{
						Yaw_goal = 215;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 4)
					{
						Yaw_goal =-38;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 5)
					{
						Yaw_goal = 215;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 6)
					{
						Yaw_goal = -38;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 7)
					{
						Yaw_goal = 214.5;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 8)
					{
						state = 0;
					}

					
					if(Total_Distance >= 8832)
					{
						count ++;
						state  = 2;
						current_color = green; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
						Tracking_Error_Integral = 0;
					}
				}
				
				if(state == 2)
				{
					if(count == 1)
					{
						Set_Car_Speed(speed_low-2*(int)Tracking_Error_Integral,speed_low-2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral);

					}
					
					else if(count == 2)
					{
						Set_Car_Speed(speed_hight-2*(int)Tracking_Error_Integral,speed_hight-2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral);
					}
					
					else if(count == 3)
					{
						Set_Car_Speed(speed_low-2*(int)Tracking_Error_Integral,speed_low-2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral);
					}
					
					else if(count == 4)
					{
						Set_Car_Speed(speed_hight-2*(int)Tracking_Error_Integral,speed_hight-2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral);
					}
					
					else if(count == 5)
					{
						Set_Car_Speed(speed_low-2*(int)Tracking_Error_Integral,speed_low-2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral);
					}
					
					else if(count == 6)
					{
						Set_Car_Speed(speed_hight-2*(int)Tracking_Error_Integral,speed_hight-2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral);
					}
					
					else if(count == 7)
					{
						Set_Car_Speed(speed_low-2*(int)Tracking_Error_Integral,speed_low-2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral,speed_hight+2*(int)Tracking_Error_Integral);
					}
					
					else if(count == 8)
					{
						Set_Car_Speed(speed_hight-2*(int)Tracking_Error_Integral,speed_hight-2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral,speed_low+2*(int)Tracking_Error_Integral);
					}
					
					if(Total_Distance >= 17932 && (R2 + R1 + L1 + L2) == 4)
					{
						state = 1;
						Encoder_Count_Set_Zero();
						current_color = blue; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
					}
				}
			}
			
			if(mode == 3)
			{
				if(state == 100)
				{
					Set_Car_Speed(0,0,0,0);
				}
				
				if(state == 0)
				{
					Set_Car_Speed(0,0,0,0);
					current_color = red; // Set the color for the next signal
					buzzer_on = 1; // Turn on the buzzer
					signal_timer = HAL_GetTick(); // Set the signal timer
					state = 100;
				}
			
				if(state == 1)
				{
				
					if(count == 0)
					{
						Yaw_goal = -36;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 1)
					{
						Yaw_goal = 215;
						Car_Go_Yaw(Yaw_goal,30);
					}
					else if(count == 2)
					{
						state = 0;
					}

					
					if(Total_Distance >= 8832)
					{
						count ++;
						state  = 2;
						current_color = green; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
						Tracking_Error_Integral = 0;
					}
				}
				
				if(state == 2)
				{
					if(count == 1)
					{
						Set_Car_Speed(17-2*(int)Tracking_Error_Integral,17-2*(int)Tracking_Error_Integral,30+2*(int)Tracking_Error_Integral,30+2*(int)Tracking_Error_Integral);
					}
					
					else if(count == 2)
					{
						Set_Car_Speed(30-2*(int)Tracking_Error_Integral,30-2*(int)Tracking_Error_Integral,17+2*(int)Tracking_Error_Integral,17+2*(int)Tracking_Error_Integral);
					}
					
					if(Total_Distance >= 17932 && (R2 + R1 + L1 + L2) == 4)
					{
						state = 1;
						Encoder_Count_Set_Zero();
						current_color = blue; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
					}
				}
			}
			
			if(mode == 2)
			{
				if(state == 100)
				{
					Set_Car_Speed(0,0,0,0);
				}
				
				if(state == 0)
				{
					Set_Car_Speed(0,0,0,0);
					current_color = red; // Set the color for the next signal
					buzzer_on = 1; // Turn on the buzzer
					signal_timer = HAL_GetTick(); // Set the signal timer
					state = 100;
				}
			
				if(state == 1)
				{
					if(count == 2)
					{
						state = 0;
					}
					Yaw_goal = count * -180;
					Car_Go_Yaw(Yaw_goal,30);
					
					if(Total_Distance >= 6900)
					{
						count ++;
						state  = 2;
						current_color = green; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
					}
				}
				
				if(state == 2)
				{
					Set_Car_Speed(30-2*(int)Tracking_Error_Integral,30-2*(int)Tracking_Error_Integral,17+2*(int)Tracking_Error_Integral,17+2*(int)Tracking_Error_Integral);
					if(Total_Distance >= 16000 && (R2 + R1 + L1 + L2) == 4)
					{
						state = 1;
						Encoder_Count_Set_Zero();
						current_color = blue; // Set the color for the next signal
						buzzer_on = 1; // Turn on the buzzer
						signal_timer = HAL_GetTick(); // Set the signal timer
					}
				}
			}
			
			if(mode == 1)
			{
				if(state == 100)
				{
					Set_Car_Speed(0,0,0,0);
				}
				if(state == 1)
				{
					Car_Go_Yaw(Yaw_goal,30);
					if(Total_Distance >= 7500)
					{
						state = 0;
					}
				}
				
				if(state == 0)
				{
					Set_Car_Speed(0,0,0,0);
					current_color = red; // Set the color for the next signal
					buzzer_on = 1; // Turn on the buzzer
					signal_timer = HAL_GetTick(); // Set the signal timer
					state = 100;
				}
			}
			
		}
	}
}


void Handle_Signals()
{
    if(buzzer_on)
    {
        Set_Buzzer(1); // Turn on the buzzer
        Set_RGB(RGB_Max, current_color); // Set the RGB color
        if(HAL_GetTick() - signal_timer >= signal_duration)
        {
            Set_Buzzer(0); // Turn off the buzzer after the duration
            buzzer_on = 0; // Reset the buzzer flag
            RGB_OFF_ALL; // Turn off RGB 
        }
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  
  Car_Init();
  OLED_Draw_Line(init,1,0,1);
  HAL_Delay(1000);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  sprintf(show_buff,"%d %d %d %d      ",speed_M1,speed_M2,speed_M3,speed_M4);
//	  OLED_Draw_Line(show_buff,1,0,1);
//	  sprintf(show_buff,"stop:%d  ",stop);
//	  OLED_Draw_Line(show_buff,2,0,1);
//	  sprintf(show_buff,"kp_tracking:%d ",kp_tracking);
//	  OLED_Draw_Line(show_buff,3,0,1);
	  Handle_Signals();
//	  printf("L2:%d, L1:%d, R1;%d, R2;%d\r\n",L2,L1,R1,R2);

//	  HAL_Delay(100);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
