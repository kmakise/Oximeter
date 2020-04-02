#include "usart.h"

void USART_CONFIG(void)
{	
	RCC_APB2PeriphClockCmd(USART_TX_CLK|USART_RX_CLK,ENABLE);
	USART_CLKCMD(USART_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin		=	USART_TX_GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin		=	USART_RX_GPIO_Pin;
	GPIO_Init(USART_RX_PORT,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitTypeDef;
	USART_InitTypeDef.USART_BaudRate						= USART_BAD;
	USART_InitTypeDef.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitTypeDef.USART_Mode								= USART_Mode_Rx|USART_Mode_Tx;
	USART_InitTypeDef.USART_Parity							= USART_Parity_No;
	USART_InitTypeDef.USART_StopBits						= USART_StopBits_1;
	USART_InitTypeDef.USART_WordLength					= USART_WordLength_8b;
	USART_Init(USART_PORT,&USART_InitTypeDef);
	
//	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_InitStruct.NVIC_IRQChannel 									= USART_IRQChannel;
//	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority 				= 1;
//	NVIC_Init(&NVIC_InitStruct);
//	
//	USART_ITConfig(USART_PORT,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART_PORT,ENABLE);
}


void Usart_SendByte(USART_TypeDef* USARTx,uint8_t data)
{
	USART_SendData(USARTx,data);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
}

void Usart_SendString(USART_TypeDef* USARTx,uint8_t *str)
{
	while(*str)
	{	
		Usart_SendByte(USARTx,*str++);
	}
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART_PORT, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART_PORT, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART_PORT, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART_PORT);
}


