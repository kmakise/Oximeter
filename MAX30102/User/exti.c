#include "exti.h"


void GPIO_CONFIG(void)
{
	RCC_APB2PeriphClockCmd(LED0_APB2Periph,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin		=	LED0_GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(LED0_PORT,&GPIO_InitStruct);
	GPIO_SetBits(LED0_PORT,LED0_GPIO_Pin);
//	RCC_APB2PeriphClockCmd(LED1_APB2Periph,ENABLE);
//	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_Out_PP;
//	GPIO_InitStruct.GPIO_Pin		=	LED1_GPIO_Pin;
//	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
//	GPIO_Init(LED1_PORT,&GPIO_InitStruct);
	
//	RCC_APB2PeriphClockCmd(KEY1_APB2Periph,ENABLE);
//	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_IPU;
//	GPIO_InitStruct.GPIO_Pin		=	KEY1_GPIO_Pin;
//	GPIO_Init(KEY1_PORT,&GPIO_InitStruct);
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	
//	GPIO_EXTILineConfig(KEY1_GPIO_PortSource,KEY1_GPIO_PinSource);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	
//	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_InitStruct.NVIC_IRQChannel 									= KEY1_NVIC_IRQChannel;
//	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority 				= 1;
//	NVIC_Init(&NVIC_InitStruct);
//	
//	EXTI_InitTypeDef EXTI_InitStruct;
//	EXTI_InitStruct.EXTI_Line 		=	KEY1_EXTI_Line;
//	EXTI_InitStruct.EXTI_LineCmd	=	ENABLE;
//	EXTI_InitStruct.EXTI_Mode			=	EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger	= EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStruct);
//	
//	
//	RCC_APB2PeriphClockCmd(KEY0_APB2Periph,ENABLE);
//	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_IPU;
//	GPIO_InitStruct.GPIO_Pin		=	KEY0_GPIO_Pin;
//	GPIO_Init(KEY0_PORT,&GPIO_InitStruct);
//	
//	
//	GPIO_EXTILineConfig(KEY0_GPIO_PortSource,KEY0_GPIO_PinSource);
//	
//	NVIC_InitStruct.NVIC_IRQChannel 									= KEY0_NVIC_IRQChannel;
//	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority 				= 2;
//	NVIC_Init(&NVIC_InitStruct);

//	EXTI_InitStruct.EXTI_Line 		=	KEY0_EXTI_Line;
//	EXTI_InitStruct.EXTI_LineCmd	=	ENABLE;
//	EXTI_InitStruct.EXTI_Mode			=	EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger	= EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStruct);
}

