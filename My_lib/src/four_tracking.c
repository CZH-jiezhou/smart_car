#include "four_tracking.h"

uint8_t Read_irtracking(Irtracking_Pin irtracking)
{
	switch (irtracking)
	{
	case X1:
		return HAL_GPIO_ReadPin(X1_GPIO_Port, X1_Pin); // ��ȡX1���ŵ�״̬ Read the status of X1 pin
		break;
	case X2:
		return HAL_GPIO_ReadPin(X2_GPIO_Port, X2_Pin); // ��ȡX2���ŵ�״̬ Read the status of X2 pin
		break;
	case X3:
		return HAL_GPIO_ReadPin(X3_GPIO_Port, X3_Pin); // ��ȡX3���ŵ�״̬ Read the status of X3 pin
		break;
	case X4:
		return HAL_GPIO_ReadPin(X4_GPIO_Port, X4_Pin); // ��ȡX4���ŵ�״̬ Read the status of X4 pin
		break;
	}
	return 0;
}
