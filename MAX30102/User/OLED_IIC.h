#ifndef _OLED_IIC_H
#define _OLED_IIC_H
#include "stm32f10x.h"                  // Device header
#include "iic.h"
#include "Font.h"
#include <stdarg.h>
#include <stdio.h>
#define OLED_Device_address 0x78
#define OLED_Device_Command 0x00
#define OLED_Device_Data 		0x40

void fill_picture(uint8_t fill_Data);
void OLED_Printf_EN(uint8_t row,uint8_t column,const char *Str,...);
void OLED_Picture(uint8_t *image);
void OLED_Init(void);

#endif
