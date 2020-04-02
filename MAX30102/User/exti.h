#ifndef _EXTI_H
#define _EXTI_H
#include "stm32f10x.h"

#define LED0_PORT 						GPIOA
#define LED0_GPIO_Pin 				GPIO_Pin_1
#define LED0_APB2Periph 			RCC_APB2Periph_GPIOA

#define LED1_PORT 						GPIOC
#define LED1_GPIO_Pin 				GPIO_Pin_13
#define LED1_APB2Periph 			RCC_APB2Periph_GPIOC

#define KEY0_PORT 						GPIOC
#define KEY0_GPIO_Pin 				GPIO_Pin_12
#define KEY0_APB2Periph 			RCC_APB2Periph_GPIOC
#define KEY0_GPIO_PortSource	GPIO_PortSourceGPIOC
#define KEY0_GPIO_PinSource		GPIO_PinSource12
#define KEY0_NVIC_IRQChannel 	EXTI15_10_IRQn
#define KEY0_EXTI_Line     		EXTI_Line12

#define KEY1_PORT 						GPIOD
#define KEY1_GPIO_Pin 				GPIO_Pin_2
#define KEY1_APB2Periph 			RCC_APB2Periph_GPIOD
#define KEY1_GPIO_PortSource	GPIO_PortSourceGPIOD
#define KEY1_GPIO_PinSource		GPIO_PinSource2
#define KEY1_NVIC_IRQChannel 	EXTI2_IRQn
#define KEY1_EXTI_Line     		EXTI_Line2

void GPIO_CONFIG(void);

#endif

