#include "ultrasonic.h"

u16 Ultrasonic_Time=0;	//超声波回显时间
u16 Ultrasonic_Number=0;

/***************************************************
描述  ：脉冲产生中断向量配置
输入  : 无
输出  ：无
调用  ：内部调用
***************************************************/
static void Ultrasonic_NVIC_TIM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//先定义结构体变量
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置中断优先级分组  P.83
  
  /* 中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn ;
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
void Ultrasonic_TIM_Config(void)
{	
	/* 定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM5,ENABLE);
	
	Ultrasonic_NVIC_TIM_Configuration();//配置中断
	
	TIM_DeInit(TIM5);//重设缺省值
	TIM_TimeBaseInitStruct.TIM_Period = 2000;//计数次数
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	TIM_ARRPreloadConfig(TIM5,ENABLE); //允许重装 TIM_Period
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);   
}

/*************************************************
函数: NVIC_ConfigurationCoder(void)
功能: 配置嵌套向量中断控制器NVIC 
参数: 无
返回: 无
**************************************************/
void Ultrasonic_NVIC_GPIO_ConfigurationCoder(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //抢占式优先级别设置为无抢占优先级
	/*外部中断0（PA0）*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //指定中断源	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //指定响应优先级别0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);				
}
void Ultrasonic_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		/*开启LED的外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE); 

//PA11--ECHO
		/*选择要控制的GPIOA11引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOA11*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
			
//PA10--TRIG
		/*选择要控制的GPIOA10引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOA10*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		/* 关闭超声波模块*/
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);

/* 定义PA11为外部中断11输入通道（EXIT11） */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource11);
		EXTI_InitStructure.EXTI_Line = EXTI_Line11;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
		EXTI_Init(&EXTI_InitStructure);
		
		Ultrasonic_NVIC_GPIO_ConfigurationCoder();
	
}


void Ultrasonic_Init(void)
{
	Ultrasonic_TIM_Config();
	Ultrasonic_GPIO_Config();
}







