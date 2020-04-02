#include "OLED_IIC.h"
/********************************************
// fill_Picture
********************************************/
void fill_picture(uint8_t fill_Data)
{
	uint8_t m,n;
	for(m=0;m<8;m++)
	{
		IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xb0+m);		//rowe0-rowe1
		IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x00);		//low column start address
		IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			IIC_Write_Byte(OLED_Device_address,OLED_Device_Data,fill_Data);
		}
	}
}

void OLED_Printf_EN(uint8_t row,uint8_t column,const char *Str,...)//页、列、汉字
{
	uint8_t r,n,ChaNum=0;
	
	char string[100];
	va_list ap;
  va_start(ap,Str);
	vsprintf(string,Str,ap);
	va_end(ap); 
	char *ptr=string;	
	while(*ptr!='\0')
	{		
		for(r=row;r<row+2;r++)//控制页一页有8行
		{
			IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xb0+r);//行
			IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,column&0x0f);//列的低四位	
			IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x10+(column>>4));//列的高四位
			if(r==row)
			{
				for(n=0;n<8;n++)
					IIC_Write_Byte(OLED_Device_address,OLED_Device_Data,ASCII8x16_Table[*ptr-' '][n]);		
			}
			if(r==row+1)
			{
				for(n=8;n<16;n++)
					IIC_Write_Byte(OLED_Device_address,OLED_Device_Data,ASCII8x16_Table[*ptr-' '][n]);	
			}
		}
		ChaNum++;
		column+=8;	
		if(ChaNum%16==0)
			row+=2;
		if(ChaNum==64)
			break;
		ptr++;
	}
}
/******************************************
// picture用来显示一个图片
******************************************/
void OLED_Picture(uint8_t *image)
{
  uint8_t x,y;
  for(y=0;y<8;y++)
    {
      IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xb0+y);
      IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x0);
      IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x10);
      for(x=0;x<128;x++)
        {
          IIC_Write_Byte(OLED_Device_address,OLED_Device_Data,*image++);
        }
    }
}

void OLED_Init(void)
{
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xAE);   //display off
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x20);	//Set Memory Addressing Mode	
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x00);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,rowe Addressing Mode (RESET);11,Invalid
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xb0);	//Set rowe Start Address for rowe Addressing Mode,0-7
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xc8);	//Set COM Output Scan Direction
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x00);//---set low column address
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x10);//---set high column address
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x40);//--set start line address
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x81);//--set contrast control register
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xdf);
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xa1);//--set segment re-map 0 to 127
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xa6);//--set normal display
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xa8);//--set multiplex ratio(1 to 64)
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x3F);//
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xd3);//-set display offset
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x00);//-not offset
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xd5);//--set display clock divide ratio/oscillator frequency
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xf0);//--set divide ratio
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xd9);//--set pre-charge period
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x22); //
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xda);//--set com pins hardware configuration
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x12);
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xdb);//--set vcomh
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x20);//0x20,0.77xVcc
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x8d);//--set DC-DC enable
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0x14);//
	IIC_Write_Byte(OLED_Device_address,OLED_Device_Command,0xaf);//--turn on oled panel 
}


