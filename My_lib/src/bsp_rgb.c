/*
 * bsp_rgb.c
 *
 *  Created on: Oct 9, 2023
 *      Author: YB-101
 */

#include "bsp_rgb.h"
#include "main.h"
/*
 *
 *打开左边或者右边的探照灯，并显示选择的颜色 Turn on the left or right searchlight and display the selected color
 *light：选择左右两边需要打开的RGB探照灯 Select the RGB searchlights that need to be turned on on the left and right sides
 *color：选择需要显示的颜色 Choose the color you want to display
 *
 *
 * */
void Set_RGB(car_RGB light, RGB_Color color)
{
	uint8_t Light_RGB = light;
	switch (Light_RGB)
	{
	case RGB_R:
		Set_color_R(color);
		break;
	case RGB_L:
		Set_color_L(color);
		break;
	case RGB_Max:
		Set_color_R(color);
		Set_color_L(color);
		break;
	default:
		RGB_OFF_ALL;
	}
}

/*
 * 打开右边的探照灯并显示颜色 Turn on the searchlight on the right and show the color
 * color：选择需要显示的颜色 Select the color to be displayed
 * */
void Set_color_R(RGB_Color color)
{
	switch (color)
	{
	case red:
		RRGB_SET(set, reset, reset);
		break;
	case green:
		RRGB_SET(reset, set, reset);
		break;
	case blue:
		RRGB_SET(reset, reset, set);
		break;
	case yellow:
		RRGB_SET(set, set, reset);
		break;
	case purple:
		RRGB_SET(set, reset, set);
		break;
	case lake:
		RRGB_SET(reset, set, set);
		break;
	case write:
		RRGB_SET(set, set, set);
		break;
	default:
		RGB_OFF_R;
	}
}

/*
 * 打开左边的探照灯并显示颜色 Turn on the left searchlight and show colors
 * color：选择需要显示的颜色 Select the color to be displayed
 * */
void Set_color_L(RGB_Color color)
{
	switch (color)
	{
	case red:
		LRGB_SET(set, reset, reset);
		break;
	case green:
		LRGB_SET(reset, set, reset);
		break;
	case blue:
		LRGB_SET(reset, reset, set);
		break;
	case yellow:
		LRGB_SET(set, set, reset);
		break;
	case purple:
		LRGB_SET(set, reset, set);
		break;
	case lake:
		LRGB_SET(reset, set, set);
		break;
	case write:
		LRGB_SET(set, set, set);
		break;
	default:
		RGB_OFF_L;
	}
}
