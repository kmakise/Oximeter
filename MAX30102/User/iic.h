#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"                  // Device header
#include "systick.h"
#define hardware                0

#if hardware
#define IIC_SCL_CLK 						RCC_APB2Periph_GPIOB
#define IIC_SCL_PORT 						GPIOB
#define IIC_SCL_GPIO_Pin 				GPIO_Pin_6
#define IIC_SDA_CLK 						RCC_APB2Periph_GPIOB
#define IIC_SDA_PORT 						GPIOB
#define IIC_SDA_GPIO_Pin 				GPIO_Pin_7

#define IIC_CLK_Hz 							200000
#define IIC_CLK									RCC_APB1Periph_I2C1
#define IIC_PORT								I2C1
void IIC_GPIO_INIT(void);
uint8_t IIC_Read_Byte(uint8_t device_addr,uint8_t register_addr);
void IIC_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data);

#else
#define IIC_SCL_CLK 						RCC_APB2Periph_GPIOB
#define IIC_SCL_PORT 						GPIOB
#define IIC_SCL_GPIO_Pin 				GPIO_Pin_6
#define IIC_SDA_CLK 						RCC_APB2Periph_GPIOB
#define IIC_SDA_PORT 						GPIOB
#define IIC_SDA_GPIO_Pin 				GPIO_Pin_7

#define IIC_SCL_1 			GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_GPIO_Pin)
#define IIC_SCL_0 			GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_GPIO_Pin)
#define IIC_SDA_1 			GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_GPIO_Pin)
#define IIC_SDA_0 			GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_GPIO_Pin)
#define	IIC_SDA_READ		GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_GPIO_Pin)

void IIC_GPIO_INIT(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t byte);
void I2C_Ack(uint8_t a);
uint8_t IIC_Receive_Byte(void);

uint8_t IIC_Read_Byte(uint8_t device_addr,uint8_t register_addr);
uint8_t IIC_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data);

uint8_t IIC_Read_Array(uint8_t device_addr,u16 register_addr,uint8_t *Data,u16 Num);
uint8_t IIC_Write_Array(uint8_t device_addr,u16 register_addr,u8 *Data,u16 Num);

extern uint8_t ack;
#endif





#endif

