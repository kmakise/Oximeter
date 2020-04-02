#ifndef _MAX30102_H
#define _MAX30102_H
#include "stm32f10x.h"                  // Device header
#include "iic.h"
#include "stdbool.h"
#define MAX30102_Device_address 			0xAE

//register addresses
#define REG_INTR_STATUS_1 	0x00
#define REG_INTR_STATUS_2 	0x01
#define REG_INTR_ENABLE_1 	0x02
#define REG_INTR_ENABLE_2 	0x03
#define REG_FIFO_WR_PTR 		0x04
#define REG_OVF_COUNTER 		0x05
#define REG_FIFO_RD_PTR 		0x06
#define REG_FIFO_DATA 			0x07
#define REG_FIFO_CONFIG 		0x08
#define REG_MODE_CONFIG 		0x09
#define REG_SPO2_CONFIG 		0x0A
#define REG_LED1_PA 				0x0C
#define REG_LED2_PA 				0x0D
#define REG_PILOT_PA 				0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 			0x1F
#define REG_TEMP_FRAC 			0x20
#define REG_TEMP_CONFIG 		0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 					0xFE
#define REG_PART_ID 				0xFF

#define MAX30102_INTPin_CLK 						RCC_APB2Periph_GPIOB
#define MAX30102_INTPin_PORT 						GPIOB
#define MAX30102_INTPin_Pin 						GPIO_Pin_9
#define MAX30102_INTPin_Read() 					GPIO_ReadInputDataBit(MAX30102_INTPin_PORT,MAX30102_INTPin_Pin)

#define Led_CLK 						RCC_APB2Periph_GPIOC
#define Led_PORT 						GPIOC
#define Led_Pin 						GPIO_Pin_9

#define SAMPLES_PER_SECOND 					100	//¼ì²âÆµÂÊ

extern uint16_t fifo_red;
extern uint16_t fifo_ir;

void MAX30102_GPIO(void);
uint8_t Max30102_reset(void);
void MAX30102_Config(void);
void max30102_read_fifo(void);

#endif


