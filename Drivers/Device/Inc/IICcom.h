#ifndef __IICCOM_H
#define __IICCOM_H

#include "stm32f1xx_hal.h"

//#define STDLIB	//标准库和HAL库的切换

/*模拟IIC端口输出输入定义*/

#ifdef STDLIB

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7

#else

#define SCL_H         HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)//GPIOB->BSRR = GPIO_PIN_6 
#define SCL_L         HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)
   
#define SDA_H         HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
#define SDA_L         HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)

#define SCL_read      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)//GPIOB->IDR  & GPIO_PIN_6
#define SDA_read      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)

#endif /*STDLIB*/


#define FALSE 0
#define TRUE	1

void I2C_GPIO_Config(void);                                //IIC引脚初始化

void I2C_delay(void);

uint8_t I2C_Start(void); //IIC开始
void I2C_Stop(void);//IIC停止

void I2C_Ack(void);//IIC发送
void I2C_NoAck(void);

uint8_t I2C_WaitAck(void); //返回为:=1有ACK,=0无ACK  等待接收
void I2C_SendByte(uint8_t SendByte);//数据从高位到低位//发送数据
unsigned char I2C_RadeByte(void);//数据从高位到低位//读取字节
uint8_t Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);//单字节写入
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);//单字节读取


#endif /*__IICCOM_H*/

