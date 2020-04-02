#include "AT24C02.h"

uint8_t AT24C02_IIC_Write_Busy(uint8_t device_addr,uint16_t register_addr)
{
	IIC_Start();
	IIC_Send_Byte(device_addr+0);
	if (ack == 1)//非应答
	{
		return 0;
	}
	else//应答
	{
		return 1;
	}
}

uint8_t AT24C02_IIC_Write_Array(uint8_t device_addr,uint16_t register_addr,u8 *Data,uint16_t _usSize)
{
	uint16_t i;
	IIC_Start();
	IIC_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	IIC_Send_Byte(register_addr);
	if (ack == 1)return 0;
	for (i = 0; i < _usSize; i++)
	{
		IIC_Send_Byte(*Data++);
		register_addr++;
		if(register_addr%8==0)
		{
			IIC_Stop();
			while(!AT24C02_IIC_Write_Busy(device_addr,register_addr));
			IIC_Send_Byte(register_addr);//最后一次应答，已经写入设备地址
			if (ack == 1)return 0;
		}
	}
	IIC_Stop();
	while(!AT24C02_IIC_Write_Busy(device_addr,register_addr));
	return 0;
}

