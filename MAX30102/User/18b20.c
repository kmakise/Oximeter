#include "18b20.h"
#include "usart.h"
#include "OLED_IIC.h"

void Ds18b20_GPIO_OUT(void)
{	
	RCC_APB2PeriphClockCmd(Ds18b20_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin		=	Ds18b20_Pin;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(Ds18b20_PORT,&GPIO_InitStruct);
}

void Ds18b20_GPIO_IN(void)
{	
	RCC_APB2PeriphClockCmd(Ds18b20_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin		=	Ds18b20_Pin;
	GPIO_Init(Ds18b20_PORT,&GPIO_InitStruct);
}

uint8_t Ds18b20_Init(void)
{
	uint8_t ack=1;
	Ds18b20_GPIO_OUT();
	DQ_0();
	Delay_us(500);
	DQ_1();
	Delay_us(60);
	Ds18b20_GPIO_IN();
	ack=DQ_Read();
	Delay_us(180);
	DQ_1();
	return ack;
}

void Ds18b20_Write_Byte(uint8_t byte)
{
	uint8_t i;
	Ds18b20_GPIO_OUT();
	for(i=0 ; i<8 ; i++)
	{
		if (byte&0x01) 
		{
				DQ_0();// Write 1
				Delay_us(2);                            
				DQ_1();
				Delay_us(60);             
		}
		else 
		{
				DQ_0();// Write 0
				Delay_us(60);             
				DQ_1();
				Delay_us(2);                          
		}
		byte=byte>>1;
	}
}

uint8_t DS18B20_Read_Write()
{
	uint8_t i;
	uint8_t byte;        		 //byte为要接收到的数据
	for(i=0 ; i<8 ; i++)
	{
		Ds18b20_GPIO_OUT();//SET PG11 OUTPUT
		DQ_0(); 
		Delay_us(2);
		DQ_1();
		Ds18b20_GPIO_IN();//SET PG11 INPUT
		Delay_us(12);
		byte >>= 1;    
		if(DQ_Read())
			byte |= 0x80;
		Delay_us(50);           
					 
	}
	return byte;
}

/************************ 读取温度 ************************/
int16_t DS18B20_Read_Temperature(void)  	//启动温度转换，读取温度
{
	u8 tl=0,th=0;
	if(Ds18b20_Init())
		return 0x7fff;
  Ds18b20_Write_Byte(0xCC);
  Ds18b20_Write_Byte(0x44);//启动DS18B20进行温度转换

  if(Ds18b20_Init())
		return 0x7fff;	
  Ds18b20_Write_Byte(0xCC);//忽略64位ROM地址，直接向DS18B20发温度转换命令，适用于一个从机工作
  Ds18b20_Write_Byte(0xBE);//读DS18B20内部RAM中9字节的温度数据
  tl=DS18B20_Read_Write();//读低8位
  th=DS18B20_Read_Write();//读高8位
	return (th<<8)+tl;
}

/************************ 处理温度 ************************/
void deal_Temp(void)
{
	float 	Temp=0;
	Temp=DS18B20_Read_Temperature();
	Temp=Temp*0.0625; 
	printf("指令温度%0.2f",Temp);
	OLED_Printf_EN(0,0,"Temp:%0.2f`C",Temp);
	Delay_ms(200);
}

