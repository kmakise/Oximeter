/**
  ******************************************************************************
  * @file    Rotating_Disk_Control.c
  * @author  roc.li
  * @version V3.5.2
  * @date    24-December-2015
  * @brief   转动圆盘控制
	*
	*					 24-December-2015 
	*						本程序主要完成乐视得视力康复仪转轮控制
  ******************************************************************************
**/
#ifndef __ROTATING_DISK_CONTROL__H_
#define __ROTATING_DISK_CONTROL__H_


/***************************************************include*********************************************/
#include "stm32f10x.h"
#include "motos.h"
/***************************************************IO口定义(用户修改)**********************************/
#define ROTATING_DISK_GPIOA																		//转轮所用的GPIO
#define ROTATING_DISK_LEFT_ZERO_POINT_PORT		GPIOA
#define ROTATING_DISK_LEFT_ZERO_POINT_PIN     GPIO_Pin_1			//左转轮零点IO
#define ROTATING_DISK_RIGHT_ZERO_POINT_PORT   GPIOA
#define ROTATING_DISK_RIGHT_ZERO_POINT_PIN    GPIO_Pin_2      //右转轮零点IO
/***************************************************参数定义（用户修改）********************************/
#define ROTATING_DISK_TOTAL_PULSE					307200              //转轮一圈总脉冲数
#define ROTATING_DISK_LENS_NUMBER					9	                  //转轮镜片数
#define ROTATING_DISK_MOTO_START_SPEED		100									//转轮电机工作起始速度
#define ROTATING_DISK_MOTO_END_SPEED			20000								//转轮电机工作最高速度

/* 此宏用来检测转轮限位状态，无需修改
 * 1 - 灭
 * 0 - 亮
 */
#define ROTATING_DISK_LEFT_ZERO_POINT		GPIO_ReadInputDataBit(ROTATING_DISK_LEFT_ZERO_POINT_PORT,ROTATING_DISK_LEFT_ZERO_POINT_PIN)//左转轮零点
#define ROTATING_DISK_RIGHT_ZERO_POINT		GPIO_ReadInputDataBit(ROTATING_DISK_RIGHT_ZERO_POINT_PORT,ROTATING_DISK_RIGHT_ZERO_POINT_PIN)//右转轮零点

/***************************************************类型定义********************************************/
typedef struct
{
	u8 Left_Number; 					//左转轮编号
	u8 Right_Number;					//右转轮编号
	u8 Left_Dirction;					//左转轮方向
	u8 Right_Dirction;				//右转轮方向
	u32 Target_Left_Pulse;		//左转轮目标脉冲数
	u32 Target_Right_Pulse;		//右转轮目标脉冲数
	
}tRotating_Disk;

/***************************************************输出变量********************************************/

/***************************************************输出函数********************************************/
/*************************************************
函数: void Rotating_Disk_Init(void)
功能: 转轮限位IO配置
参数: 无
返回: 无
**************************************************/
extern void Rotating_Disk_Init(void);
/*************************************************
函数: u16 Rotating_Disk_Init(u8 Left_Number, u8 Right_Number)
功能: 转轮左右转动设置  
参数: 无
返回: 无
**************************************************/
extern u8 Rotating_Disk_Control(u8 Left_Number, u8 Right_Number);



#endif
