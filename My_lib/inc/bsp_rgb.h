/*
 * bsp_rgb.h
 *
 *  Created on: Oct 9, 2023
 *      Author: YB-101
 */

#ifndef BSP_RGB_H_
#define BSP_RGB_H_

#include "stdint.h"
/* 左右探照灯定义 Definition of left and right searchlights*/
typedef enum car_rgb
{
	RGB_R = 1, // 右边RGB探照灯 Right RGB searchlight
	RGB_L,	   // 左边RGB探照灯 Left RGB searchlight
	RGB_Max
} car_RGB;
/*RGB灯颜色定义 RGB light color definition*/
typedef enum rgb_color
{
	red = 0,
	green,
	blue,
	yellow,
	purple,
	lake,
	write,
	Max_color
} RGB_Color;

#define set GPIO_PIN_SET
#define reset GPIO_PIN_RESET

#define RRGB_R(x) HAL_GPIO_WritePin(GPIOE, RRGB_R_Pin, x); // 控制右边的红色RGB灯 Control the red RGB light on the right
#define RRGB_G(x) HAL_GPIO_WritePin(GPIOE, RRGB_G_Pin, x); // 控制右边的绿色RGB灯 Control the green RGB light on the right
#define RRGB_B(x) HAL_GPIO_WritePin(GPIOE, RRGB_B_Pin, x); // 控制右边的蓝色RGB灯 Control the blue RGB light on the right

#define LRGB_R(x) HAL_GPIO_WritePin(GPIOG, LRGB_R_Pin, x); // 控制左边的红色RGB灯 Control the red RGB light on the left
#define LRGB_G(x) HAL_GPIO_WritePin(GPIOE, LRGB_G_Pin, x); // 控制左边的绿色RGB灯 Control the green RGB light on the left
#define LRGB_B(x) HAL_GPIO_WritePin(GPIOG, LRGB_B_Pin, x); // 控制左边的蓝色RGB灯 Control the blue RGB light on the left

#define RRGB_SET(R, G, B) \
	;                     \
	RRGB_R(R);            \
	RRGB_G(G);            \
	RRGB_B(B);
#define LRGB_SET(R, G, B) \
	LRGB_R(R);            \
	LRGB_G(G);            \
	LRGB_B(B);

#define RGB_OFF_R RRGB_SET(reset, reset, reset); // 关闭右边的所有RGB灯 Turn off all RGB lights on the right
#define RGB_OFF_L LRGB_SET(reset, reset, reset); // 关闭左边的所有RGB灯 Turn off all RGB lights on the left
#define RGB_OFF_ALL \
	RGB_OFF_R;      \
	RGB_OFF_L; // 关闭左右两边的所有RGB灯 Turn off all RGB lights on the left and right sides

void Set_RGB(car_RGB light, RGB_Color color);
void Set_color_R(RGB_Color color);
void Set_color_L(RGB_Color color);

#endif /* BSP_RGB_H_ */
