#include "delay.h"

static uint32_t g_fac_us = 0; /* us延时倍乘数 */

// 函数功能:延迟初始化
// 传递参数:无
// Function function: Delay initialization
// Pass parameters: None
void Delay_Init(void)
{
	// Because it is the main frequency of 72MHz 一般是72
	// Due to the presence of_ The systick has been configured in Init, so there is no need to reconfigure it here
	g_fac_us = 72; /* 由于在HAL_Init中已对systick做了配置，所以这里无需重新配置 */
}

/**********************************************************
** 函数名: Delay_us
** 功能描述: 延时nus，nus为要延时的us数(用时钟摘取法来做us延时).
** 输入参数: nus
** 输出参数: 无
**Function name: Delay_ Us
**Function Description: Delay Nus, where Nus is the number of us to delay (using clock picking method to do us delay)
**Input parameter: nus
**Output parameter: None
***********************************************************/
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; /*The value of LOAD LOAD的值  */
	ticks = nus * g_fac_us;			 /*Number of beats required 需要的节拍数 */
	told = SysTick->VAL;			 /*Counter value at the beginning of entry 刚进入时的计数器值 */
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
			{
				tcnt += told - tnow; /*Please note that SYSTEM is a decreasing counter 这里注意一下SYSTICK是一个递减的计数器就可以了 */
			}
			else
			{
				tcnt += reload - tnow + told;
			}
			told = tnow;
			if (tcnt >= ticks)
			{
				break; /*Exit if the time exceeds/equals the time to be delayed 时间超过/等于要延迟的时间,则退出 */
			}
		}
	}
}

/**********************************************************
** 函数名: Delay_ms
** 功能描述: 延时nus，nus为要延时的us数(用时钟摘取法来做us延时).
** 输入参数: nus
** 输出参数: 无
** Function name: Delay_ Ms
**Function Description: Delay Nus, where Nus is the number of us to delay (using clock picking method to do us delay)
**Input parameter: nus
**Output parameter: None
***********************************************************/
void delay_ms(uint16_t nms)
{
	delay_us((uint32_t)(nms * 1000)); /*Normal mode delay 普通方式延时 */
}

// 函数功能:解算的us延迟函数
// 传递参数:n:延迟时间
// Function function: Solved us delay function
// Pass parameter: n: Delay time
void delay_us_mpu(uint32_t n)
{
	uint8_t j;
	while (n--)
		for (j = 0; j < 10; j++)
			;
}

// 函数功能:解算的ms延迟函数
// 传递参数:n:延迟时间
// Function function: MS delay function solved
// Pass parameter: n: Delay time
void delay_ms_mpu(uint32_t n)
{
	while (n--)
		delay_us_mpu(1000);
}

void get_ms(unsigned long *time)
{
}
