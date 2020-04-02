/**
  ******************************************************************************
  * @file    ST7735.h
  * @author  张东
  * @version V1.0.0
  * @date    2019-12-27
  * @brief   ST7735 1.44TFT显示模组初始化与操作
  ******************************************************************************
  */

#ifndef __ST7735_H
#define __ST7735_H

/*Start of file ---------------------------------------------------------------*/

//size define
#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        128



//gpio define 
#define LCD_CTRL   	  	GPIOB				//定义TFT数据端口
#define LCD_RS         	GPIO_PIN_10	//PB10--->>TFT --RS/DC
#define LCD_SCL        	GPIO_PIN_13	//PB13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_PIN_15	//PB15 MOSI--->>TFT --SDA/DIN


//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//液晶控制口置1操作语句宏定义   
#define	LCD_RS_SET  	LCD_CTRL->BSRR=LCD_RS    
#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL     

//液晶控制口置0操作语句宏定义  
#define	LCD_RS_CLR  	LCD_CTRL->BRR=LCD_RS    
#define	LCD_SDA_CLR  	LCD_CTRL->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	LCD_CTRL->BRR=LCD_SCL    


#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //数据输出
#define LCD_DATAIN     LCD_DATA->IDR;   //数据输入

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 

/*Public func -----------------------------------------------------------------*/

void Lcd_Init(void);
void Lcd_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
void Lcd_SetXY(uint16_t x,uint16_t y);
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);
unsigned int Lcd_ReadPoint(uint16_t x,uint16_t y);
void Lcd_Clear(uint16_t Color);
//区块填充
void Lcd_Block(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color);

#endif /*__ST7735_H*/

