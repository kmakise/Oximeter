#include "iic.h"
#include "usart.h"
uint8_t ack;

void IIC_GPIO_INIT(void)
{
#if hardware
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin		=	GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin		=	GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	RCC_APB1PeriphClockCmd(IIC_CLK, ENABLE);  
	I2C_InitTypeDef  I2C_InitStructure; 
  I2C_InitStructure.I2C_Mode 								= I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle 					= I2C_DutyCycle_2;		              /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_OwnAddress1 				=	0X0a; 
  I2C_InitStructure.I2C_Ack 								= I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	    /* I2C的寻址模式 */
  I2C_InitStructure.I2C_ClockSpeed 					= IIC_CLK_Hz;	                      /* 通信速率 */
  I2C_Init(IIC_PORT, &I2C_InitStructure);	                                      /* IIC_PORT 初始化 */
  I2C_Cmd(IIC_PORT, ENABLE);  	                                                /* 使能 IIC_PORT */

#else
	RCC_APB2PeriphClockCmd(IIC_SCL_CLK | IIC_SDA_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_Out_OD ;
	GPIO_InitStruct.GPIO_Pin		=	IIC_SCL_GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin		=	IIC_SDA_GPIO_Pin;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStruct);
#endif
}



#if hardware
void IIC_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data)
{
  I2C_GenerateSTART(IIC_PORT, ENABLE);
  while(!I2C_CheckEvent(IIC_PORT, I2C_EVENT_MASTER_MODE_SELECT));
	
  I2C_Send7bitAddress(IIC_PORT, device_addr, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(IIC_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
	
  I2C_SendData(IIC_PORT, register_addr);
  while(!I2C_CheckEvent(IIC_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  I2C_SendData(IIC_PORT, data); 
  while(!I2C_CheckEvent(IIC_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
  I2C_GenerateSTOP(IIC_PORT, ENABLE);
}

uint8_t IIC_Read_Byte(uint8_t device_addr,uint8_t register_addr)
{
	uint8_t data;
	
	I2C_GenerateSTART(IIC_PORT,ENABLE);
	while(!I2C_CheckEvent(IIC_PORT,I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(IIC_PORT,device_addr,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(IIC_PORT,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(IIC_PORT,register_addr);
	while(!I2C_CheckEvent(IIC_PORT,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(IIC_PORT,ENABLE);
	while(!I2C_CheckEvent(IIC_PORT,I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(IIC_PORT,device_addr,I2C_Direction_Receiver);
	while(!I2C_CheckEvent(IIC_PORT,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_AcknowledgeConfig(IIC_PORT,DISABLE);
	while(I2C_GetFlagStatus(IIC_PORT,I2C_FLAG_RXNE) == RESET);
	
	data=I2C_ReceiveData(IIC_PORT);
	
	I2C_GenerateSTOP(IIC_PORT ,ENABLE);	
	
	return data;
}
#else

void IIC_Delay(void)
{
	uint8_t i=6; //i=10延时1.5us//这里可以优化速度 ，经测试最低到5还能写入
   while(i--);
}


void IIC_Start(void)
{
	IIC_SCL_1;
	IIC_SDA_1;//启始信号建立时间 0.6us 400KHz
	IIC_Delay();
	IIC_SDA_0;
	IIC_Delay();//启始信号保持时间0.6us
	IIC_SCL_0;
	IIC_Delay();//时钟低电平时间1.3us
}
 
void IIC_Stop(void)
{	
	IIC_SDA_0;
	IIC_SCL_1;
	IIC_Delay();//结束信号建立时间0.6us
	IIC_SDA_1;
	IIC_Delay();//总线空闲时间时间1.3us
}

void IIC_Send_Byte(uint8_t byte)
{
	uint8_t i;//先发送高位
	for(i=0;i<8;i++)
	{
		if(byte & 0x80)
		{
			IIC_SDA_1;
		}
		else
		{
			IIC_SDA_0;
		}
		IIC_Delay();
		IIC_SCL_1;
		IIC_Delay();
		IIC_SCL_0;
		IIC_Delay();
		byte<<=1;
	}
	IIC_SDA_1;  
	IIC_Delay();
	IIC_SCL_1;
	IIC_Delay();
	if(IIC_SDA_READ)
	{
		ack=1;
	}
	else
	{
		ack=0;
	}
	IIC_SCL_0;
	IIC_Delay();
}

uint8_t IIC_Receive_Byte(void)
{
   uint8_t receive=0;
   uint8_t i;//置数据线为输入方式
   for(i=0;i<8;i++)
   {     		
			receive<<=1;
			IIC_SCL_1;//置时钟线为高使数据线上数据有效
			IIC_Delay();
			if(IIC_SDA_READ)
			{
				receive++;//读数据位,接收的数据位放入retc中
			}
			IIC_SCL_0;  
			IIC_Delay();
   }
   return receive;
}

uint8_t IIC_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data)
{
	IIC_Start();
	IIC_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	IIC_Send_Byte(register_addr);   
	if (ack == 1)return 0;
	IIC_Send_Byte(data);
	if (ack == 1)return 0; 
	IIC_Stop();
	return 1;
}

void I2C_Ack(uint8_t a)
{ 
	if(a)
	{
		IIC_SDA_1;            //非应答
		IIC_Delay(); 
		IIC_SCL_1;  	
		IIC_Delay();  
		IIC_SCL_0; 	
		IIC_Delay();  		
	}
	else
	{
		IIC_SDA_0; 						//应答
		IIC_Delay(); 
		IIC_SCL_1;  
		IIC_Delay(); 
		IIC_SCL_0; 
		IIC_Delay(); 
		IIC_SDA_1; 
	}
}

uint8_t IIC_Read_Byte(uint8_t device_addr,uint8_t register_addr)
{
	uint8_t read_data;
	IIC_Start();
	IIC_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	IIC_Send_Byte(register_addr);
	if (ack == 1)return 0;
	IIC_Start();
	IIC_Send_Byte(device_addr+1);
	if (ack == 1)return 0;
	read_data = IIC_Receive_Byte();
	I2C_Ack(1);
	IIC_Stop();
	return read_data;
}

uint8_t IIC_Write_Array(uint8_t device_addr,uint16_t register_addr,u8 *Data,uint16_t Num)
{
	uint16_t i;
	IIC_Start();
	IIC_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	IIC_Send_Byte(register_addr);
	if (ack == 1)return 0;
	IIC_Start();
	IIC_Send_Byte(device_addr+1);
	if (ack == 1)return 0;
	for(i=0;i<Num;i++)
	{
		IIC_Send_Byte(*Data++);
		if (ack == 1)return 0;
	}
	IIC_Stop();
	return 1;
}

uint8_t IIC_Read_Array(uint8_t device_addr,uint16_t register_addr,uint8_t *Data,uint16_t Num)
{
	uint16_t i;
	IIC_Start();
	IIC_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	IIC_Send_Byte(register_addr);
	if (ack == 1)return 0;
	IIC_Start();
	IIC_Send_Byte(device_addr+1);
	if (ack == 1)return 0;
	for(i=0;i<Num;i++)
	{
		*Data++ = IIC_Receive_Byte();
		if(i==Num-1)
			I2C_Ack(1);
		else
			I2C_Ack(0);
	}
	IIC_Stop();
	return 1;
}

#endif
