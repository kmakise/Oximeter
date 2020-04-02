/**
  ******************************************************************************
  * @file    motoS.c
  * @author  roc.li
  * @version V3.5.2
  * @date    24-December-2015
  * @brief   步进电机S曲线驱动程序
  *
	*          26-December-2014 
	*					  在此文件中，根据V3.5.0进行修改，主要修改了速度模式下的电机速度的变化
	*            1.增加电机驱动器分频数、丝杆螺距两个参数
	*            2.增加由速度（mm/min）转换成频率（Hz）的函数
	*            3.增加电机工作后速度随意改变的函数
	*
	*					 24-December-2015 
	*						在此文件中，根据V3.5.1进行修改，主要修改了moto应用引脚定义方式，位置
	*						模式下的电机工作方式
	*            1.moto控制引脚用到GPIOA、GPIOD、GPIOE等上时候，原来只能涉及GPIOA或
	*							 者其他一个，同时还增加了用到JTAG引脚时的设置
	*            2.对位置模式下电机最低速度和最高速度随意设置
	*            3.对位置模式下要走脉冲数在加速和减速中分配问题
  ******************************************************************************
**/
#ifndef __MOTOS__H__
#define __MOTOS__H__
/***************************************************include*********************************************/
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include <math.h>
#include <stdio.h>

/**************************************************电机驱动IO口定义(用户修改)***************************/
#define MOTO_GPIOA																		//电机所用的GPIO
#define MOTO_GPIOD																		//电机所用的GPIO
#define MOTO_GPIOE																		//电机所用的GPIO
#define JTAGDisable                                   //JTAG禁用
#define MOTO_PULSE_PORT					GPIOE
#define MOTO_PULSE_PIN					GPIO_Pin_2						//电机脉冲IO
#define MOTO_DIRCTION_PORT			GPIOD
#define MOTO_DIRECTION_PIN			GPIO_Pin_3				  	//电机方向IO
#define MOTO_ENABLE1_PORT				GPIOD									
#define MOTO_ENABLE1_PIN				GPIO_Pin_1						//电机1使能IO
#define MOTO_ENABLE2_PORT				GPIOA							
#define MOTO_ENABLE2_PIN				GPIO_Pin_15						//电机2使能IO
/*所用定时器范围从TIM2-TIM7*/
#define PULSE_USE_TIM3																//定义电机脉冲产生所用的时钟
#define PULSE_CHANGE_USE_TIM4													//定义脉冲频率更改所用的时钟
#define PULSE_TIM								TIM3									//成生脉冲的定时器
#define PULSE_CHANGE_TIM				TIM4									//改变频率的定时器
/***************************************************参数定义（用户修改）********************************/
#define Discrete_Accuracy 500							//离散精度
#define ACC								5000						//加加速度,固定值，通过反复实验确定,此值决定了到达目标速度的时间
#define DIVIDE						160						//电机驱动器分频数
#define WIREROD           1               //丝杆螺距(mm)
#define POSITION_MODE									  	//定义位置模式
//#define SPEED_MODE										 	//定义速度模式  以上二选一

/* 此宏用来配置电机的控制线，无需修改
 * 1 - 灭
 - 0 - 亮
 */
#define HIGH  		0
#define LOW 			1
#define TOGGLE 		2
#define Moto_Pluse_Port(a)	if (a==HIGH)	\
														GPIO_SetBits(MOTO_PULSE_PORT,MOTO_PULSE_PIN);\
														if(a==LOW)		\
														GPIO_ResetBits(MOTO_PULSE_PORT,MOTO_PULSE_PIN);\
														if(a==TOGGLE)		\
														GPIO_WriteBit(MOTO_PULSE_PORT,MOTO_PULSE_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(MOTO_PULSE_PORT,MOTO_PULSE_PIN)))
						
#define Moto_Direction_Port(a)	if (a==HIGH)	\
																GPIO_SetBits(MOTO_DIRCTION_PORT,MOTO_DIRECTION_PIN);\
																if(a==LOW)		\
																GPIO_ResetBits(MOTO_DIRCTION_PORT,MOTO_DIRECTION_PIN)

#define Moto_Enable1_Port(a)	if (a==HIGH)	\
															GPIO_SetBits(MOTO_ENABLE1_PORT,MOTO_ENABLE1_PIN);\
															if(a==LOW)		\
															GPIO_ResetBits(MOTO_ENABLE1_PORT,MOTO_ENABLE1_PIN)

#define Moto_Enable2_Port(a)	if (a==HIGH)	\
															GPIO_SetBits(MOTO_ENABLE2_PORT,MOTO_ENABLE2_PIN);\
															if(a==LOW)		\
															GPIO_ResetBits(MOTO_ENABLE2_PORT,MOTO_ENABLE2_PIN)
/****************************************************类型定义******************************************/
typedef struct
{
	float 	 Start_Frq;					//起始频率(Hz)
	float 	 Destination_Frq;		//目标频率(Hz)							
	float 	 Stay_Time;					//离散间隔时间
	float		 Inflection_Frq;		//拐点频率
	float    Inflection_Time;   //拐点时间
	float		 Frquency;					//输出频率
	uint16_t count;							//离散点的数量
	uint16_t count_RED;         //电机降速的速度在曲线表中的下标
	uint16_t NowSpeed;					//现在速度（定时器初值）
	uint16_t nowSpeed;          //当前速度（mm/min）
	#ifdef POSITION_MODE
		uint32_t Step_Total;							//总的要走的脉冲数
		uint32_t Steps_Togo;							//电机余下的脉冲数
		uint32_t Step_Total_Half;					//电机要走的脉冲数的一半
		uint32_t AddSpeed_Steps;					//加速步数
		uint32_t StaticSpeed_Steps;				//均速运动的步数
	#endif
}tMoto;

enum STATE
{
	TRUE = 0,
	FALSE	
};
enum MotoStat
{
	STOP  = 0,				//电机停止
	ADD_SPEED,				//电机启动加速
	DEC_SPEED,				//电机减速
	RED_SPEED,        //电机降速
	ACC_SPEED,				//电机加速
	FULL_SPEED				//电机均速
};
/***************************************************输出变量*********************************/
extern uint16_t MotoS_Tab[Discrete_Accuracy];//曲线表(内容为S曲线的频率表HZ)
extern tMoto Moto;
extern uint16_t *pMotoS_Tab;
extern uint8_t flag;
extern __IO enum STATE Change_Frq_Flag;
extern __IO uint16_t Frq_Change_Count;			//改变频率的次数
extern __IO uint8_t	Change_Level_Flag;			//更改肪冲输出标志
extern __IO uint32_t Pulse_Count;						//脉冲输出个数
extern __IO enum MotoStat	Moto_Stat;				//电机状态
#ifdef POSITION_MODE
	extern __IO enum STATE AddState;				//加速走完标志
	extern __IO enum STATE StaticSpeedStage;//均速阶段标志位
#endif
/****************************************************输出函数******************************************/
/***************************************************
函数功能：电机初始化
输入参数：NONE
输出返回：NONE
***************************************************/
extern void Moto_init(void);
#ifdef SPEED_MODE
/***************************************************
函数功能：电机工作 （Hz）（速度模式）让电机进入停止阶段
输入参数：end_speed:最高速度(65000max)
输出返回：NONE
***************************************************/
extern void Work_Moto_Pulse(uint16_t start_speed,uint16_t end_speed);
/***************************************************
函数功能：电机工作 （mm/min）（速度模式）让电机进入停止阶段
输入参数：end_speed:最高速度(2000mm/min)
输出返回：NONE
***************************************************/
extern void Work_Moto_Speed(uint32_t end_speed);
/***************************************************
函数功能：电机立刻停止（速度模式）让电机进入停止阶段
输入参数：NONE
输出返回：NONE
***************************************************/
extern void Right_Stop_Moto(void);
#endif

#ifdef POSITION_MODE
/***************************************************
函数功能：电机启动（位置模式）
输入参数：step:要走的步数(脉冲数),start_speed:起动速度(50min)，end_speed:最高速度(65000max)
输出返回：NONE
***************************************************/
extern void Start_Moto(uint32_t steps,uint16_t start_speed,uint16_t end_speed);
#endif
/***************************************************
简介：电机驱动脉冲生成函数，此函数应该被放置在对应的中断回调中
参数：无
输出: 无
***************************************************/
extern void Moto_Pulse_Create(void);//放在中断中以产生脉冲
/***************************************************
简介：更改频率的中断回调。应该放置在对应的中断中
参数：无
输出: 无
***************************************************/
extern void Moto_Pulse_Change(void);//放在中断中以改变频率
#endif
