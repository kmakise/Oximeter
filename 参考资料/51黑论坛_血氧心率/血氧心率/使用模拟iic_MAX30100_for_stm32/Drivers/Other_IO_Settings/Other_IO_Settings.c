#include "Other_IO_Settings.h"
/*************************************************
函数: void Other_GPIO_Configuration(void）
功能: GPIO配置
参数: 无
返回: 无
**************************************************/
void Other_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;        //定义GPIO初始化结构体
	
	/******指示灯******/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_11 |GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		//置高
	GPIO_SetBits(GPIOC,GPIO_Pin_12);		//置高

	//蜂鸣器和摆纱架电机IO设置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_11; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
