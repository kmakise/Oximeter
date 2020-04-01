/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "motoS.h"
#include "ultrasonic.h"
#include <stdio.h>

u8 Ultrasonic_UP;	//超声波测距上升沿标识



/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void EXTI15_10_IRQHandler(void)
{
//超声波回显中断	
	//计时完成，读出计时时间
	if ( EXTI_GetITStatus(EXTI_Line11) != RESET )
	{
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))//下降沿
		{	
			if(Ultrasonic_UP==1)
			{				
				Ultrasonic_UP=0;
				/* 读出当前计数值 */
				Ultrasonic_Time = TIM_GetCounter(TIM5);								
				TIM_Cmd(TIM5 , DISABLE);	
				
			}
			else
			{
				TIM_Cmd(TIM5 , DISABLE);					
			}
		}
		//收到回显信号高电平，开始计时
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))//上升沿
		{				
			Ultrasonic_UP=1;		//上升沿标识
			Ultrasonic_Time=0;
			
			Ultrasonic_Number ++;
			TIM_SetCounter(TIM5,0);
			TIM_Cmd(TIM5 , ENABLE);				
		}
		EXTI_ClearFlag(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line11);		    //清除EXTI_Line0线路挂起标志位   
	}
	
}
///*************************************************
//函数: void TIM3_IRQHandler(void)
//功能: 定时器3中断服务程序
//参数: 无
//返回: 无
//**************************************************/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) /*检查TIM3更新中断发生与否*/
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /*清除TIMx更新中断标志 */			
//		Moto_Pulse_Create();//放在中断中以产生脉冲
//	}
//} 

///*************************************************
//函数: void TIM3_IRQHandler(void)
//功能: 定时器3中断服务程序
//参数: 无
//返回: 无
//**************************************************/
//void TIM4_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET ) /*检查TIM3更新中断发生与否*/
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /*清除TIMx更新中断标志 */
//		Moto_Pulse_Change();//放在中断中以改变频率
//	}
//} 

void TIM5_IRQHandler(void)
{		
	if(TIM_GetITStatus(TIM5 , TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);			
	}
	
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
