#ifndef __ULTRASONIC__H__
#define __ULTRASONIC__H__
/***************************************************include*********************************************/
#include "stm32f10x.h"
#include "stm32f10x_tim.h"


/**************************************************电机驱动IO口定义(用户修改)***************************/


extern u16 Ultrasonic_Time;	//超声波回显时间
extern u16 Ultrasonic_Number;


extern void Ultrasonic_Init(void);

#endif
