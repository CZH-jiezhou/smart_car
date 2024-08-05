#ifndef __FOUR_TRACKING_H
#define __FOUR_TRACKING_H

#include "main.h"

typedef enum irtracking_pin
{
	X1 = 1, // ”“±ﬂRGBÃΩ’’µ∆ Right RGB searchlight
	X2,		// ◊Û±ﬂRGBÃΩ’’µ∆ Left RGB searchlight
	X3,
	X4
} Irtracking_Pin;

uint8_t Read_irtracking(Irtracking_Pin irtracking);

#endif 
