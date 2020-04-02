#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#define USART1_PORT 1
#define USART2_PORT 0
#define USART3_PORT 0
#define UART4_PORT 	0
#define UART5_PORT 	0

#if	USART1_PORT
#define USART_TX_CLK 							RCC_APB2Periph_GPIOA
#define USART_TX_PORT 						GPIOA
#define USART_TX_GPIO_Pin 				GPIO_Pin_9
#define USART_RX_CLK 							RCC_APB2Periph_GPIOA
#define USART_RX_PORT 						GPIOA
#define USART_RX_GPIO_Pin 				GPIO_Pin_10
#define USART_CLKCMD							RCC_APB2PeriphClockCmd
#define USART_CLK 								RCC_APB2Periph_USART1
#define USART_PORT								USART1
#define USART_BAD									115200
#define USART_IRQChannel          USART1_IRQn
#define USART_IRQHandler					USART1_IRQHandler
#elif	USART2_PORT
#define USART_TX_CLK 							RCC_APB2Periph_GPIOA
#define USART_TX_PORT 						GPIOA
#define USART_TX_GPIO_Pin 				GPIO_Pin_2
#define USART_RX_CLK 							RCC_APB2Periph_GPIOA
#define USART_RX_PORT 						GPIOA
#define USART_RX_GPIO_Pin 				GPIO_Pin_3
#define USART_CLKCMD							RCC_APB1PeriphClockCmd
#define USART_CLK 								RCC_APB1Periph_USART2
#define USART_PORT								USART2
#define USART_BAD									115200
#define USART_IRQChannel          USART2_IRQn
#define USART_IRQHandler					USART2_IRQHandler

#elif	USART3_PORT
#define USART_TX_CLK 							RCC_APB2Periph_GPIOB
#define USART_TX_PORT 						GPIOB
#define USART_TX_GPIO_Pin 				GPIO_Pin_10
#define USART_RX_CLK 							RCC_APB2Periph_GPIOB
#define USART_RX_PORT 						GPIOB
#define USART_RX_GPIO_Pin 				GPIO_Pin_11
#define USART_CLKCMD							RCC_APB1PeriphClockCmd
#define USART_CLK 								RCC_APB1Periph_USART3
#define USART_PORT								USART3
#define USART_BAD									115200
#define USART_IRQChannel          USART3_IRQn
#define USART_IRQHandler					USART3_IRQHandler

#elif	UART4_PORT
#define USART_TX_CLK 							RCC_APB2Periph_GPIOC
#define USART_TX_PORT 						GPIOC
#define USART_TX_GPIO_Pin 				GPIO_Pin_10
#define USART_RX_CLK 							RCC_APB2Periph_GPIOC
#define USART_RX_PORT 						GPIOC
#define USART_RX_GPIO_Pin 				GPIO_Pin_11
#define USART_CLKCMD							RCC_APB1PeriphClockCmd
#define USART_CLK 								RCC_APB1Periph_UART4
#define USART_PORT								UART4
#define USART_BAD									115200
#define USART_IRQChannel          UART4_IRQn
#define USART_IRQHandler					UART4_IRQHandler

#elif	UART5_PORT
#define USART_TX_CLK 							RCC_APB2Periph_GPIOC
#define USART_TX_PORT 						GPIOC
#define USART_TX_GPIO_Pin 				GPIO_Pin_12
#define USART_RX_CLK 							RCC_APB2Periph_GPIOD
#define USART_RX_PORT 						GPIOD
#define USART_RX_GPIO_Pin 				GPIO_Pin_2
#define USART_CLKCMD							RCC_APB1PeriphClockCmd
#define USART_CLK 								RCC_APB1Periph_UART5
#define USART_PORT								UART5
#define USART_BAD									115200
#define USART_IRQChannel          UART5_IRQn
#define USART_IRQHandler					UART5_IRQHandler
#endif


void USART_CONFIG(void);
void Usart_SendByte(USART_TypeDef* USARTx,uint8_t data);
void Usart_SendString(USART_TypeDef* USARTx,uint8_t *str);
#endif

