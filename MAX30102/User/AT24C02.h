#ifndef _AT24C02_H
#define _AT24C02_H
#include "stm32f10x.h"                  // Device header
#include "iic.h"


uint8_t AT24C02_IIC_Write_Array(uint8_t device_addr,uint16_t register_addr,u8 *Data,uint16_t _usSize);

#endif

