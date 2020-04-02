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

#include "Rotating_Disk_Control.h"


/*************************************变量申明*********************************/
tRotating_Disk	Rotating_Disk;
/*************************************函数申明*********************************/

/*************************************************
函数: void Rotating_Disk_Control_GPIO_Config(void)
功能: 转轮限位IO配置
参数: 无
返回: 无
**************************************************/
void Rotating_Disk_Control_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	#ifdef ROTATING_DISK_GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOB
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOD
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOE
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOF
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	#endif
	
	/*选择要控制的GPIOx引脚*/	
		GPIO_InitStructure.GPIO_Pin = ROTATING_DISK_LEFT_ZERO_POINT_PIN ;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOx*/
  	GPIO_Init(ROTATING_DISK_LEFT_ZERO_POINT_PORT, &GPIO_InitStructure);
	
	/*选择要控制的GPIOx引脚*/	
		GPIO_InitStructure.GPIO_Pin = ROTATING_DISK_RIGHT_ZERO_POINT_PIN ;	
	/*调用库函数，初始化GPIOx*/
  	GPIO_Init(ROTATING_DISK_RIGHT_ZERO_POINT_PORT, &GPIO_InitStructure);
}
/*************************************************
函数: void Rotating_Disk_Init(void)
功能: 转轮初始化
参数: 无
返回: 无
**************************************************/
void Rotating_Disk_Init(void)
{
//	Moto_init();
//	/*左电机按照位置模式工作*/	
//	if(ROTATING_DISK_LEFT_ZERO_POINT == 0)
//	{
//		Moto_Enable1_Port(LOW);//左电机使能
//		Moto_Enable2_Port(HIGH);//右电机不使能
//		Moto_Direction_Port(HIGH);//设置电机方向
//		Start_Moto(ROTATING_DISK_TOTAL_PULSE,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//		while(ROTATING_DISK_LEFT_ZERO_POINT == 0);//等待左转轮到零点
//		TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
//		TIM_Cmd(PULSE_TIM, DISABLE);
//		Moto_Stat =	STOP;
//		Frq_Change_Count = 0;
//	}
//	Rotating_Disk.Left_Number = 1;//左转轮编号为1
//	Rotating_Disk.Left_Dirction = 1;
//	/*右电机按照位置模式工作*/	
//	if(ROTATING_DISK_RIGHT_ZERO_POINT == 0)
//	{
//		Moto_Enable1_Port(HIGH);//左电机不使能
//		Moto_Enable2_Port(LOW);//右电机使能
//		Moto_Direction_Port(HIGH);//设置电机方向
//		Start_Moto(ROTATING_DISK_TOTAL_PULSE,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//		while(ROTATING_DISK_RIGHT_ZERO_POINT == 0);//等待右转轮到零点
//		TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
//		TIM_Cmd(PULSE_TIM, DISABLE);
//		Moto_Stat =	STOP;
//		Frq_Change_Count = 0;
//	}
//	Rotating_Disk.Right_Number = 1;//右转轮编号为1
//	Rotating_Disk.Right_Dirction = 1;
}
/*************************************************
函数: u16 Rotating_Disk_Init(u8 Left_Number, u8 Right_Number)
功能: 转轮左右转动设置  
参数: 无
返回: 无
**************************************************/
u8 Rotating_Disk_Control(u8 Left_Number, u8 Right_Number)
{
//	u8 Roatating_Disk_Number;
	/*对输入参数纠错*/
	if(Left_Number > 9) Left_Number = 9;
	if(Left_Number < 1) Left_Number = 1;
	if(Right_Number > 9) Right_Number = 9;
	if(Right_Number < 1) Right_Number = 1;
//	
//	/*优化路径*/
//	if(Left_Number - Rotating_Disk.Left_Number <= ROTATING_DISK_LENS_NUMBER/2)//
//	{
//		Left_Number = Left_Number - Rotating_Disk.Left_Number;
//		Rotating_Disk.Left_Dirction = 1;
//	}
//	else 
//	{
//		Left_Number = ROTATING_DISK_LENS_NUMBER - Left_Number + Rotating_Disk.Left_Number;
//		Rotating_Disk.Left_Dirction = 0;
//	}
//	if(Right_Number - Rotating_Disk.Right_Number <= ROTATING_DISK_LENS_NUMBER/2)//
//	{
//		Right_Number = Right_Number -Rotating_Disk.Right_Number;
//		Rotating_Disk.Right_Dirction = 1;
//	}
//	else
//	{
//		Right_Number = ROTATING_DISK_LENS_NUMBER - Right_Number + Rotating_Disk.Right_Number;
//		Rotating_Disk.Right_Dirction = 0;
//	}
//	
//	Rotating_Disk.Target_Left_Pulse = (u32)((float)ROTATING_DISK_TOTAL_PULSE/(float)ROTATING_DISK_LENS_NUMBER*Left_Number);//计算出左转轮目标脉冲数
//	Rotating_Disk.Target_Right_Pulse = (u32)((float)ROTATING_DISK_TOTAL_PULSE/(float)ROTATING_DISK_LENS_NUMBER*Right_Number);//计算出右转轮目标脉冲数
//	/*左转轮工作*/
//	Moto_Enable1_Port(LOW);//左电机使能
//	Moto_Enable2_Port(HIGH);//右电机不使能
//	if(Rotating_Disk.Left_Dirction == 1) 
//		Moto_Direction_Port(HIGH);//设置电机方向
//	else 
//		Moto_Direction_Port(LOW);//设置电机方向
//	Start_Moto(Rotating_Disk.Target_Left_Pulse,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//	while( Moto_Stat != STOP );//等待电机停止
//	/*有转轮工作*/
//	Moto_Enable1_Port(HIGH);//左电机不使能
//	Moto_Enable2_Port(LOW);//右电机使能
//	if(Rotating_Disk.Right_Dirction == 1)
//		Moto_Direction_Port(HIGH);//设置电机方向
//	else
//		Moto_Direction_Port(LOW);//设置电机方向
//	Start_Moto(Rotating_Disk.Target_Right_Pulse,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//	while( Moto_Stat != STOP );//等待电机停止
//	
//	Roatating_Disk_Number = Rotating_Disk.Left_Number*10+Rotating_Disk.Right_Number;
//	return Roatating_Disk_Number;
}


