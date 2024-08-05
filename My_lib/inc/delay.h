#ifndef __DELAY_H
#define __DELAY_H

#include "main.h"

void Delay_Init(void);
void delay_us(uint32_t nus); // Timing us ¶¨Ê±us
void delay_ms(uint16_t nms);

void delay_ms_mpu(uint32_t n);
void delay_us_mpu(uint32_t n);
void get_ms(unsigned long *time);

#endif
