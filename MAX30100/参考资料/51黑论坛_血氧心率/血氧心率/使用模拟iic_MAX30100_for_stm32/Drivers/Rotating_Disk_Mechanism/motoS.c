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
#include "motoS.h"
#include <stdio.h>
/************************************变量申明***********************************/
tMoto Moto;
uint16_t MotoS_Tab[Discrete_Accuracy];		//曲线表(内容为S曲线的频率表HZ)
uint16_t *pMotoS_Tab;											//指向曲线表的指针
__IO uint16_t Frq_Change_Count = 0;				//改变频率的次数
__IO enum STATE Change_Frq_Flag = FALSE;	//改变频率的标志
__IO uint32_t Pulse_Count = 0;						//脉冲输出个数
__IO enum MotoStat	Moto_Stat	=	STOP;			//电机状态
#ifdef POSITION_MODE
	__IO enum STATE AddState = FALSE;				//加速走完标志
	__IO enum STATE StaticSpeedStage = FALSE;//均速阶段标志位
#endif
/*************************************函数申明*********************************/

/***************************************************
描述  ：脉冲产生中断向量配置
输入  : 无
输出  ：无
调用  ：内部调用
***************************************************/
static void NVIC_PULSE_TIM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//先定义结构体变量
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置中断优先级分组  P.83
  
  /* 中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = 
	#ifdef PULSE_USE_TIM2
	TIM2_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM3
	TIM3_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM4
	TIM4_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM5
	TIM5_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM6
	TIM6_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM7
	TIM7_IRQn ;
	#endif
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
  NVIC_Init(&NVIC_InitStructure);//中断向量初始化
}
/***************************************************
描述  ：配置脉冲输出定时器
输入  : 用于生成脉冲频率
输出  ：无
调用  ：内部调用
***************************************************/
void PULSE_TIM_Mode_Config(uint16_t frq)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(
	#ifdef PULSE_USE_TIM2
	RCC_APB1Periph_TIM2,
	#endif
	#ifdef PULSE_USE_TIM3
	RCC_APB1Periph_TIM3,
	#endif
	#ifdef PULSE_USE_TIM4
	RCC_APB1Periph_TIM4,
	#endif
	#ifdef PULSE_USE_TIM5
	RCC_APB1Periph_TIM5,
	#endif
	#ifdef PULSE_USE_TIM6
	RCC_APB1Periph_TIM6,
	#endif
	#ifdef PULSE_USE_TIM7
	RCC_APB1Periph_TIM7,
	#endif
	ENABLE);
	
	NVIC_PULSE_TIM_Configuration();
	
	TIM_DeInit(PULSE_TIM);//重设缺省值
	TIM_TimeBaseInitStruct.TIM_Period = (uint16_t)(10000000/(frq*4.16667))-1;//计数次数
	TIM_TimeBaseInitStruct.TIM_Prescaler = 15-1;//10us的计数周期Tcnt=72M/15=0.41666us
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(PULSE_TIM,&TIM_TimeBaseInitStruct);
	TIM_ARRPreloadConfig(PULSE_TIM,ENABLE); //允许重装 TIM_Period
	TIM_ClearFlag(PULSE_TIM, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
	TIM_ITConfig(PULSE_TIM,TIM_IT_Update,ENABLE);   
	//TIM_Cmd(PULSE_TIM, ENABLE);
}
/***************************************************
描述  ：产生更改频率事件中断向量配置
输入  : 无
输出  ：无
调用  ：内部调用
***************************************************/
static void NVIC_PULSE_CHANGE_TIM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//先定义结构体变量
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置中断优先级分组  P.83
  
  /* 中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = 
	#ifdef PULSE_CHANGE_USE_TIM2
	TIM2_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM3
	TIM3_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM4
	TIM4_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM5
	TIM5_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM6
	TIM6_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM7
	TIM7_IRQn;
	#endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
  NVIC_Init(&NVIC_InitStructure);//中断向量初始化
}
/***************************************************
描述  ：用于周期性的更改脉冲频率
输入  : 中断频率
输出  ：无
调用  ：内部调用
***************************************************/
void PULSE_CHANGE_TIM_Mode_Config(uint16_t frq)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(
		#ifdef PULSE_CHANGE_USE_TIM2
			RCC_APB1Periph_TIM2 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM3
			RCC_APB1Periph_TIM3 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM4
			RCC_APB1Periph_TIM4 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM5
			RCC_APB1Periph_TIM5 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM6
			RCC_APB1Periph_TIM6 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM7
			RCC_APB1Periph_TIM7 , 
		#endif
		ENABLE);
	
		NVIC_PULSE_CHANGE_TIM_Configuration();//配置中断
	
    TIM_DeInit(PULSE_CHANGE_TIM);//重设缺省值
    TIM_TimeBaseStructure.TIM_Period=(uint16_t)(100000/frq)-1;		/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= 720 - 1;				    /* 时钟预分频数 72M/720 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
	
    TIM_TimeBaseInit(PULSE_CHANGE_TIM, &TIM_TimeBaseStructure);
    TIM_ClearFlag(PULSE_CHANGE_TIM, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(PULSE_CHANGE_TIM,TIM_IT_Update,ENABLE);
    //TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);																		/* 开启时钟 *
}
/***************************************************
简介：Moto输出端口配置
参数：无
输出: 无
***************************************************/
void Moto_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOX的外设时钟*/
	#ifdef MOTO_GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	#endif
	#ifdef MOTO_GPIOB
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	#endif
	#ifdef MOTO_GPIOC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	#endif
	#ifdef MOTO_GPIOD
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	#endif
	#ifdef MOTO_GPIOE
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	#endif
	#ifdef MOTO_GPIOF
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	#endif	
	#ifdef JTAGDisable
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	#endif
	/*选择要控制的GPIOx引脚*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_PULSE_PIN ;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOC*/
  	GPIO_Init(MOTO_PULSE_PORT, &GPIO_InitStructure);		
	
	/*选择要控制的GPIOx引脚*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_DIRECTION_PIN ;	
	/*调用库函数，初始化GPIOx*/
  	GPIO_Init(MOTO_DIRCTION_PORT, &GPIO_InitStructure);
	
	/*选择要控制的GPIOx引脚*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_ENABLE1_PIN ;	 
	/*调用库函数，初始化GPIOx*/
  	GPIO_Init(MOTO_ENABLE1_PORT, &GPIO_InitStructure);
		
	/*选择要控制的GPIOx引脚*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_ENABLE2_PIN ;	
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14 ;
	/*调用库函数，初始化GPIOx*/
  	GPIO_Init(MOTO_ENABLE2_PORT, &GPIO_InitStructure);
//		GPIO_Init(GPIOA, &GPIO_InitStructure);

}
/***************************************************
函数功能：生成运行曲线
输入参数：起始频率(最小30HZ)，目标频率(最大65000HZ)
输出返回：NONE
***************************************************/
void Moto_Curve_Create(float Start_Frq,float Destination_Frq)
{
	uint16_t i;
	float T = 0;
	Moto.count = 0;
	pMotoS_Tab = MotoS_Tab;
	/*对结构体进行赋值*/
	Moto.Start_Frq = Start_Frq;
	Moto.Destination_Frq = Destination_Frq;
	/*计算相关参数*/
	Moto.Stay_Time = 2.0/Moto.Start_Frq;
	Moto.Inflection_Frq = (Moto.Start_Frq + Moto.Destination_Frq)/2.0;
	Moto.Inflection_Time = sqrt(2*(Moto.Inflection_Frq - Moto.Start_Frq)/(float)ACC);
	/*生成S曲线*/
	while(T < Moto.Inflection_Time)//拐点前的曲线
	{
		Moto.Frquency = Moto.Start_Frq + ACC*T*T/2.0;
		*pMotoS_Tab = (uint16_t)Moto.Frquency;//离散的频率点放进表格中
		pMotoS_Tab++;
		T+=Moto.Stay_Time;
		Moto.count++;
	}
	while(Moto.Frquency < Moto.Destination_Frq-1)//拐点后的曲线
	{
		Moto.Frquency = 2*ACC*T*sqrt(2*(Moto.Inflection_Frq - Moto.Start_Frq)/(float)ACC)
										-ACC*T*T/2.0
										-2*Moto.Inflection_Frq
										+3*Moto.Start_Frq;
		*pMotoS_Tab = (uint16_t)Moto.Frquency;//离散的频率点放进表格中
		pMotoS_Tab++;
		T+=Moto.Stay_Time;
		Moto.count++;
	}
	pMotoS_Tab = MotoS_Tab;//复位指针备用

	/*从串口输出频率表*/
	/*printf("\r\n频率值\r\n");
	for(i=0;i<Moto.count;i++)
	{
		printf("%d,",MotoS_Tab[i]);
	}*/
	/*把表格中的频率转换成定时器初值*/
	//printf("\r\n定时器值\r\n");
	for(i=0;i<Moto.count;i++)
	{
		MotoS_Tab[i]=(uint16_t)(10000000/(MotoS_Tab[i]*4.16667));
		//printf("%d,",MotoS_Tab[i]);
	}
	/*输出表格长度*/
	//printf("\r\n 表格长度=%d\r\n",Moto.count);
}

#ifdef SPEED_MODE
/***************************************************
函数功能：电机启动（Hz）（速度模式）
输入参数：start_speed:起动速度(50min)，end_speed:最高速度(65000max)
输出返回：NONE
***************************************************/
void Start_Moto_Pulse(uint16_t start_speed,uint16_t end_speed)
{
		/*对输入参数纠错*/
	if(start_speed<50)		start_speed = 50;
	if(end_speed>65000)		end_speed = 65000;
	if(Moto_Stat == STOP)//电机停止状态
	{
		/*生成曲线表*/	
		Moto_Curve_Create(start_speed,end_speed);
		PULSE_CHANGE_TIM_Mode_Config((uint16_t)(2.0/Moto.Stay_Time));
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//开启定时器
		TIM_Cmd(PULSE_TIM, ENABLE);//开启定时器
		Moto_Stat = ADD_SPEED;
	}
}
/***************************************************
函数功能：电机在工作时加速（Hz）（速度模式）
输入参数：start_speed:起动速度(50min)，end_speed:最高速度(65000max)
输出返回：NONE
***************************************************/
void ACC_Moto_Pulse(uint16_t start_speed,uint16_t end_speed)
{
	u8 i=0;
	if(start_speed<50)		start_speed = 50;
	if(end_speed>65000)		end_speed = 65000;
	if((Moto_Stat == FULL_SPEED) | (Moto_Stat == ADD_SPEED) | (Moto_Stat == RED_SPEED))
	{	
		Moto_Curve_Create(start_speed,end_speed);
		end_speed=(uint16_t)(10000000/(end_speed*4.16667));
		Frq_Change_Count = 0;
		for(i=0;i<Moto.count;i++)
		{
			if(MotoS_Tab[i]>Moto.NowSpeed)	
			{
				pMotoS_Tab++;
				Frq_Change_Count++;
			}
		}
		//
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//开启定时器		
		Moto_Stat=ACC_SPEED;	//电机于现在的速度（非0）加速度
	}
}
/***************************************************
函数功能：电机减速（mm/min）（速度模式）
输入参数：end_speed:最高速度(65000max)
输出返回：NONE
***************************************************/
void RED_Moto_Pulse(uint16_t end_speed)
{
	uint16_t i=0;
	end_speed=(uint16_t)(10000000/(end_speed*4.16667));
	if((Moto_Stat == FULL_SPEED)|(Moto_Stat == ADD_SPEED))//电机正加速或者全速运行状态
	{
		for(i=Moto.count;i>0;i--)
		{
			if(MotoS_Tab[i]<end_speed)
				Moto.count_RED=i;
		}
		//printf("%d,",Moto.count_i);
		Moto_Stat = RED_SPEED;//电机减速
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//开启定时器
	}
}
/***************************************************
函数功能：电机停止（速度模式）让电机进入停止阶段
输入参数：NONE
输出返回：NONE
***************************************************/
void Stop_Moto(void)
{
	if((Moto_Stat == FULL_SPEED)|(Moto_Stat == ADD_SPEED))//电机正加速或者全速运行状态
	{
		Moto_Stat = DEC_SPEED;//电机减速
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//开启定时器
	}
}
/***************************************************
函数功能：电机立刻停止（速度模式）让电机进入停止阶段
输入参数：NONE
输出返回：NONE
***************************************************/
void Right_Stop_Moto(void)
{
	TIM_Cmd(PULSE_TIM, DISABLE);
	Moto_Stat =	STOP;
	Frq_Change_Count = 0;
	//printf("\r\nstop!");
}

/***************************************************
函数功能：电机工作 （Hz）（速度模式）让电机进入停止阶段
输入参数：start_speed:起动速度(50min)，end_speed:最高速度(65000max)
输出返回：NONE
***************************************************/
void Work_Moto_Pulse(uint16_t start_speed,uint16_t end_speed)
{
	if(Moto_Stat==STOP)
		Start_Moto_Pulse(start_speed,end_speed);
	else if(Moto.nowSpeed > end_speed && end_speed != 0)
		RED_Moto_Pulse(end_speed);
	else if(Moto.nowSpeed < end_speed)
		ACC_Moto_Pulse(start_speed,end_speed);
	else if(end_speed == 0)		
		Stop_Moto();	
	Moto.nowSpeed=end_speed;
}
/***************************************************
函数功能：电机工作 （mm/min）（速度模式）让电机进入停止阶段
输入参数：end_speed:最高速度(2900mm/min)
输出返回：NONE
***************************************************/
void Work_Moto_Speed(uint32_t end_speed)
{
	if(end_speed>1000)	end_speed = 1000;
	end_speed=end_speed*DIVIDE/60/WIREROD;
	Work_Moto_Pulse(50,end_speed); 
}
#endif
/***************************************************
函数功能：电机启动（位置模式）
输入参数：step:要走的步数(这里指实际的脉冲数,需要考虑驱动的细分),
					start_speed:起动速度(50min)，end_speed:最高速度(65000max)
输出返回：NONE
***************************************************/
#ifdef POSITION_MODE
void Start_Moto(uint32_t steps,uint16_t start_speed,uint16_t end_speed)
{
	
	/*对输入参数纠错*/
	if(start_speed<50)		start_speed = 50;
	if(end_speed>65000)		end_speed = 65000;
	if(Moto_Stat == STOP)//电机停止状态
	{
	/*生成曲线表*/	
		Moto_Curve_Create(start_speed,end_speed);
		Moto.Step_Total = steps;//赋值需要走的脉冲个数
		Moto.Steps_Togo = steps;//赋值余下的脉冲数
		Moto.Step_Total_Half = (uint32_t)(steps/2);//赋值需要走的脉冲个数的一半
		PULSE_CHANGE_TIM_Mode_Config((uint16_t)(2.0/Moto.Stay_Time));
		Moto_Stat = ADD_SPEED;//加速阶段
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//开启定时器
		TIM_Cmd(PULSE_TIM, ENABLE);//开启定时器
	}
}
#endif

/***************************************************
函数功能：电机初始化
输入参数：NONE
输出返回：NONE
***************************************************/
void Moto_init(void)
{
	Moto_GPIO_Config();
	Moto_Pluse_Port(HIGH);
	Moto_Direction_Port(HIGH);
	Moto_Enable1_Port(HIGH);
	Moto_Enable2_Port(HIGH);
	PULSE_TIM_Mode_Config(50);
}

/***************************************************
简介：电机驱动脉冲生成函数，此函数应该被放置在对应的中断回调中
参数：无
输出: 无
***************************************************/
void Moto_Pulse_Create(void)
{
	/*如果定义了速度模式*/
	#ifdef SPEED_MODE
		Moto_Pluse_Port(TOGGLE);
		if(Change_Frq_Flag == TRUE)//需要改变频率
		{
			TIM_SetAutoreload(PULSE_TIM,*pMotoS_Tab);//更改频率
			Change_Frq_Flag = FALSE;
		}
	#endif
	/*如果定义了位置模式*/
	#ifdef POSITION_MODE
		Moto_Pluse_Port(TOGGLE);
		if(GPIO_ReadOutputDataBit(MOTO_PULSE_PORT,MOTO_PULSE_PIN) == 0)//如果输出为低
		{
			Moto.Steps_Togo--;
			//usart_buf++;
			if((Moto.Steps_Togo == Moto.Step_Total_Half) && (AddState == FALSE))//未走完加速阶段，余下的脉冲等于一半
			{
				Moto_Stat = DEC_SPEED;//电机减速
				TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//开启定时器
			}
			else if((Moto.Steps_Togo > Moto.Step_Total_Half) && (AddState == TRUE) && (StaticSpeedStage == FALSE))//走完加速阶段，余下的脉冲大于一半步数
			{
				Moto.AddSpeed_Steps = Moto.Step_Total - Moto.Steps_Togo;				//得出加速的步数
				Moto.StaticSpeed_Steps = Moto.Steps_Togo - Moto.AddSpeed_Steps;	//得到均速运动步数
				StaticSpeedStage = TRUE;
			}
			else if(StaticSpeedStage == TRUE)//在均速阶段
			{
				if(Moto.Steps_Togo == (Moto.AddSpeed_Steps+1))//余下减速的脉冲
				{
					Moto_Stat = DEC_SPEED;//电机减速
					TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//开启定时器
				}
			}
			else if(Moto.Steps_Togo == 0)//走完步数
			{
				TIM_Cmd(PULSE_TIM, DISABLE);//关闭脉冲输出
				Moto_Pluse_Port(HIGH);
				//printf("\r\n%d",usart_buf/32);
			}
		}
		if(Change_Frq_Flag == TRUE)//需要改变频率
		{
			TIM_SetAutoreload(PULSE_TIM,*pMotoS_Tab);//更改频率
			Change_Frq_Flag = FALSE;
		}
	#endif
}
/***************************************************
简介：更改频率的中断回调。应该放置在对应的中断中
参数：无
输出: 无
***************************************************/
void Moto_Pulse_Change(void)
{
	/*如果定义了速度模式*/
	#ifdef SPEED_MODE
		if(Moto_Stat == ADD_SPEED)//电机启动
		{
			if(Frq_Change_Count < Moto.count-1)//加速度没走完
			{
				Change_Frq_Flag = TRUE;//标志频率需要更改
				pMotoS_Tab++;//下一个数
				Moto.NowSpeed=*pMotoS_Tab;
				Frq_Change_Count++;
			}
			else if(Frq_Change_Count == Moto.count-1)
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat = FULL_SPEED;
				//printf("\r\nStart!");
			}
		}
		else if(Moto_Stat == ACC_SPEED)//电机已非0的速度加速
		{
			if(Frq_Change_Count < Moto.count-1)//加速度没走完
			{
				Change_Frq_Flag = TRUE;//标志频率需要更改
				pMotoS_Tab++;//下一个数
				Moto.NowSpeed=*pMotoS_Tab;
				Frq_Change_Count++;
			}
			else if(Frq_Change_Count == Moto.count-1)
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat = FULL_SPEED;
				//printf("\r\nAcceleration!");
			}
		}
		else if(Moto_Stat == RED_SPEED)//电机降速
		{
			if(Frq_Change_Count > Moto.count_RED)//减速度没走完
			{
				Change_Frq_Flag = TRUE;//标志频率需要更改
				pMotoS_Tab--;//指向上一个数
				Moto.NowSpeed=*pMotoS_Tab;
				Frq_Change_Count--;
			}
			else if(Frq_Change_Count == Moto.count_RED)//走完减速
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat =	FULL_SPEED;
				//printf("\r\nDeceleration!");
			}
		}
		else if(Moto_Stat == DEC_SPEED)//电机减速停止
		{
			if(Frq_Change_Count > 0)//减速度没走完
			{
				Change_Frq_Flag = TRUE;//标志频率需要更改
				pMotoS_Tab--;//指向上一个数
				Frq_Change_Count--;
			}
			else if(Frq_Change_Count == 0)//走完减速
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				TIM_Cmd(PULSE_TIM, DISABLE);
				Moto_Stat =	STOP;
				//printf("\r\nstop!");
			}
		}
	#endif
	/*如果定义了位置模式*/
	#ifdef POSITION_MODE
		if(Moto_Stat == ADD_SPEED)
		{
			if(Frq_Change_Count < Moto.count-1)//加速度没走完
			{
				Change_Frq_Flag = TRUE;//标志频率需要更改
				pMotoS_Tab++;//下一个数
				Frq_Change_Count++;
			}
			else if(Frq_Change_Count == Moto.count-1)
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat = FULL_SPEED;
				AddState = TRUE;//标志完成加速
				//printf("\r\nadd end!");
			}
		}
		if(Moto_Stat == DEC_SPEED)
		{
			if(Frq_Change_Count > 0)//减速度没走完
			{
				Change_Frq_Flag = TRUE;//标志频率需要更改
				pMotoS_Tab--;//指向上一个数
				Frq_Change_Count--;
			}
			else if(Frq_Change_Count == 0)//走完减速
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				TIM_Cmd(PULSE_TIM, DISABLE);
				Moto_Stat =	STOP;
			}
		}
	#endif
}
