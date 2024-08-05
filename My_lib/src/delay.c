#include "delay.h"

static uint32_t g_fac_us = 0; /* us��ʱ������ */

// ��������:�ӳٳ�ʼ��
// ���ݲ���:��
// Function function: Delay initialization
// Pass parameters: None
void Delay_Init(void)
{
	// Because it is the main frequency of 72MHz һ����72
	// Due to the presence of_ The systick has been configured in Init, so there is no need to reconfigure it here
	g_fac_us = 72; /* ������HAL_Init���Ѷ�systick�������ã��������������������� */
}

/**********************************************************
** ������: Delay_us
** ��������: ��ʱnus��nusΪҪ��ʱ��us��(��ʱ��ժȡ������us��ʱ).
** �������: nus
** �������: ��
**Function name: Delay_ Us
**Function Description: Delay Nus, where Nus is the number of us to delay (using clock picking method to do us delay)
**Input parameter: nus
**Output parameter: None
***********************************************************/
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; /*The value of LOAD LOAD��ֵ  */
	ticks = nus * g_fac_us;			 /*Number of beats required ��Ҫ�Ľ����� */
	told = SysTick->VAL;			 /*Counter value at the beginning of entry �ս���ʱ�ļ�����ֵ */
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
			{
				tcnt += told - tnow; /*Please note that SYSTEM is a decreasing counter ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
			}
			else
			{
				tcnt += reload - tnow + told;
			}
			told = tnow;
			if (tcnt >= ticks)
			{
				break; /*Exit if the time exceeds/equals the time to be delayed ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
			}
		}
	}
}

/**********************************************************
** ������: Delay_ms
** ��������: ��ʱnus��nusΪҪ��ʱ��us��(��ʱ��ժȡ������us��ʱ).
** �������: nus
** �������: ��
** Function name: Delay_ Ms
**Function Description: Delay Nus, where Nus is the number of us to delay (using clock picking method to do us delay)
**Input parameter: nus
**Output parameter: None
***********************************************************/
void delay_ms(uint16_t nms)
{
	delay_us((uint32_t)(nms * 1000)); /*Normal mode delay ��ͨ��ʽ��ʱ */
}

// ��������:�����us�ӳٺ���
// ���ݲ���:n:�ӳ�ʱ��
// Function function: Solved us delay function
// Pass parameter: n: Delay time
void delay_us_mpu(uint32_t n)
{
	uint8_t j;
	while (n--)
		for (j = 0; j < 10; j++)
			;
}

// ��������:�����ms�ӳٺ���
// ���ݲ���:n:�ӳ�ʱ��
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
