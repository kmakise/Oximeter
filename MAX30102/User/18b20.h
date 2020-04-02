#ifndef _18B20_H
#define _18B20_H
#include "stm32f10x.h"
#include "systick.h"

#define Ds18b20_CLK 						RCC_APB2Periph_GPIOB
#define Ds18b20_PORT 						GPIOB
#define Ds18b20_Pin 						GPIO_Pin_12

#define DQ_1()  		GPIO_SetBits(Ds18b20_PORT,Ds18b20_Pin)
#define DQ_0()  		GPIO_ResetBits(Ds18b20_PORT,Ds18b20_Pin)
#define DQ_Read() 	GPIO_ReadInputDataBit(Ds18b20_PORT,Ds18b20_Pin)

void Ds18b20_GPIO_OUT(void);
uint8_t Ds18b20_Init(void);
int16_t DS18B20_Read_Temperature(void);
void deal_Temp(void);
#endif

