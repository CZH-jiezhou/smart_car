#ifndef __FOUR_TRACKING_H
#define __FOUR_TRACKING_H

#include "main.h"

typedef enum irtracking_pin
{
	X1 = 1, // �ұ�RGB̽�յ� Right RGB searchlight
	X2,		// ���RGB̽�յ� Left RGB searchlight
	X3,
	X4
} Irtracking_Pin;

uint8_t Read_irtracking(Irtracking_Pin irtracking);

#endif 
