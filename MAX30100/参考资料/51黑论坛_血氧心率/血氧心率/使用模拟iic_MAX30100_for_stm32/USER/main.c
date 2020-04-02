#include "stm32f10x.h"
#include "usart.h"
#include "ultrasonic.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "delay.h"	//延时函数 1
#include "delayl.h"	//延时函数 2
#include <stdio.h>
#include <math.h>
#include "bsp_i2c_gpio.h"
#include "oled.h"

#define SAMPLE_50   //如果定义此宏就是50采样率   否则是100

 
/*************************************************
函数: fputc(int ch, FILE *f)
功能: 重定向c库函数printf到USART1
参数: 无
返回: 无
**************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}


#define USR_I2C_USED I2C1

void I2C1_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    

	/* PB6,7 SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;//100K速度
    
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);

    printf("I2C1_Configuration----\n\r");
}

void I2C2_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    

	/* PB10,11 SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    I2C_DeInit(I2C2);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;//100K速度
    
	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C2, ENABLE);

    printf("I2C2_Configuration----\n\r");
}

void I2C1_GPIO_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO结构体定义
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能I2C的IO口 

		/* 使能与 I2C1 有关的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//PIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 开漏输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化结构体配置

}

/* I2C 工作模式配置 */

void I2C1_Mode_config(void)
{
		/*定义I2C结构体*/
	I2C_InitTypeDef  I2C_InitStructure;



		
	/*配置为I2C模式*/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/*该参数只有在I2C 工作在快速模式（时钟工作频率高于 100KHz）下才有意义。*/
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
	/*设置第一个设备自身地址*/
	I2C_InitStructure.I2C_OwnAddress1 =0x30;
	
	/*使能应答*/
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/*AT24C02地址为7位所以设置7位就行了*/
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; /*时钟速率，以HZ为单位的，最高为400khz*/
	
	I2C_InitStructure.I2C_ClockSpeed = 20000;

	/* 使能 I2C1 */
	I2C_Cmd(I2C1, ENABLE);
	
	/* I2C1 初始化 */
	I2C_Init(I2C1, &I2C_InitStructure);
	
	
}

void I2C2_GPIO_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; //GPIO结构体定义
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能I2C的IO口 

		/* 使能与 I2C1 有关的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	
	/* PB10-I2C2_SCL、PB11-I2C2_SDA*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;

	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//PIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 开漏输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化结构体配置

}

/* I2C 工作模式配置 */

void I2C2_Mode_config(void)
{
		/*定义I2C结构体*/
	I2C_InitTypeDef  I2C_InitStructure;

	/*配置为I2C模式*/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/*该参数只有在I2C 工作在快速模式（时钟工作频率高于 100KHz）下才有意义。*/
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
	/*设置第一个设备自身地址*/
	I2C_InitStructure.I2C_OwnAddress1 =0x30;
	
	/*使能应答*/
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/*AT24C02地址为7位所以设置7位就行了*/
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; /*时钟速率，以HZ为单位的，最高为400khz*/
	
	I2C_InitStructure.I2C_ClockSpeed = 100000;

	/* 使能 I2C1 */
	I2C_Cmd(I2C2, ENABLE);
	
	/* I2C1 初始化 */
	I2C_Init(I2C2, &I2C_InitStructure);
	
	
}


/*************************************************
函数: void main_init(void)
功能: main初始化
参数: 无
返回: 无
**************************************************/
void main_init(void)
{
	Usart_Init();
	//I2C1_GPIO_Config();
	//I2C1_Mode_config();
	//I2C1_Configuration();
	bsp_InitI2C();
	delay_init(72);            //延时初始化
}

extern void test_max30100_fun(void);
extern u8 max10300_Bus_Read(u8 Register_Address);
extern void max10300_init(void);

/*************************************************
函数: int main(void)
功能: main主函数
参数: 无
返回: 无
**************************************************/
int main(void)
{
	u8 temp_num=0; 
	 
	main_init();
	
	max10300_init();
	printf("\r\n stm32 init runing \r\n");
	
	delayl_init();	    	 //延时函数初始化	  
	 
	OLED_Init();			//初始化OLED     
 
	OLED_ShowString(0,0, "SpO2:",16);  
 	 	
	OLED_ShowString(0,30,"Heart Rate:",16);  
 		  
	OLED_Refresh_Gram();//更新显示到OLED
	
	
	
	
	
	/*
while(1)
{
	delay_ms(1000);            
	max10300_init();
	temp_num = max10300_Bus_Read(0x16);
	printf("当前温度 = %d\r\n",temp_num);
}*/
	while(1)
	{	
		test_max30100_fun();
		
		
		 	
		
		
		
	}
}




#define max10300_WR_address 0xAE

u8 max10300_Bus_Write(u8 Register_Address, u8 Word_Data)
{

	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max10300_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址 */
	i2c_SendByte(Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第5步：开始写入数据 */
	i2c_SendByte(Word_Data);

	/* 第6步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}



u8 max10300_Bus_Read(u8 Register_Address)
{
	u8  data;


	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max10300_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max10300_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	{
		data = i2c_ReadByte();	/* 读1个字节 */

		i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return data;	/* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

static void i2c_Delay(void)
{
	uint8_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 30; i++);
}
#if 1
void max10300_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max10300_WR_address | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(max10300_WR_address | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	while (no)
	{
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		i2c_Ack();
		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //

		
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		if(1==no)
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			i2c_Ack();
		Word_Data[i][1] = (((u16)data1 << 8) | data2); 

		no--;	
		i++;
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
}
#else

void max10300_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	

	while(I2C_GetFlagStatus(USR_I2C_USED, I2C_FLAG_BUSY))
		; //调用库函数检测I2C器件是否处于BUSY状态
		

	I2C_AcknowledgeConfig(USR_I2C_USED, ENABLE);   /*允许1字节1应答模式*/


	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //清除EV5
	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(USR_I2C_USED,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		; //ADDR=1，清除EV6 
		
	I2C_SendData(USR_I2C_USED, Register_Address);
	 while(! I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	 	;//移位寄存器非空，数据寄存器已经空，产生EV8，发送数据到DR既可清除该事件

	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //清除EV5
	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


 	while (no)
    {	
	
		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7 
	    data1 = I2C_ReceiveData(USR_I2C_USED);

		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
	    data2 = I2C_ReceiveData(USR_I2C_USED);

	    Word_Data[i][0] = (((u16)data1 << 8) | data2);  //



       while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
		data1 = I2C_ReceiveData(USR_I2C_USED);

	    if(no==1)
		{
     		I2C_AcknowledgeConfig(I2C1, DISABLE);	//最后一位后要关闭应答的
    		I2C_GenerateSTOP(I2C1, ENABLE);			//发送停止位
		
		}

		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
		data2 = I2C_ReceiveData(USR_I2C_USED);

		Word_Data[i][1] = (((u16)data1 << 8) | data2); 
		i++;

		
	    /* Decrement the read bytes counter */
	    no--;
    }
	
	I2C_AcknowledgeConfig(USR_I2C_USED, ENABLE);//将应答位使能回去，等待下次通信
	I2C_GenerateSTOP(I2C1, ENABLE);			//发送停止位

}

#endif
#define INTERRUPT_REG  					0X00
#define INTERRUPT_REG_A_FULL  			(0X01<<7)
#define INTERRUPT_REG_TEMP_RDY  		(0X01<<6)
#define INTERRUPT_REG_HR_RDY  			(0X01<<5)
#define INTERRUPT_REG_SPO2_RDY  		(0X01<<4)
#define INTERRUPT_REG_PWR_RDY  			(0X01<<0)



void max10300_init()
{
	max10300_Bus_Write(0x06, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
	//max10300_Bus_Write(0x06, 0x0a);  //MODE[2:0]=010 HR only enabled     when used is mode ,the red led is not used.
	max10300_Bus_Write(0x01, 0xF0); //open all of interrupt
	max10300_Bus_Write(INTERRUPT_REG, 0x00); //all interrupt clear
	max10300_Bus_Write(0x09, 0x33); //r_pa=3,ir_pa=3

#ifdef SAMPLE_50
	max10300_Bus_Write(0x07, 0x43); //SPO2_SR[4:2]=000   50 per second    LED_PW[1:0]=11  16BITS
#else
	max10300_Bus_Write(0x07, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS
#endif
	
	max10300_Bus_Write(0x02, 0x00);   //set FIFO write Pointer reg = 0x00 for clear it
	max10300_Bus_Write(0x03, 0x00);	//set Over Flow Counter  reg = 0x00 for clear it
	max10300_Bus_Write(0x04, 0x0f);	//set FIFO Read Pointer  reg = 0x0f for   
											//waitting  write pointer eq read pointer   to   interrupts  INTERRUPT_REG_A_FULL
}

double my_floor(double x)
{
   double y=x;
    if( (*( ( (int *) &y)+1) & 0x80000000)  != 0) //或者if(x<0)
        return (float)((int)x)-1;
    else
        return (float)((int)x);
}

double my_fmod(double x, double y)
{
   double temp, ret;
  
   if (y == 0.0)
      return 0.0;
   temp = my_floor(x/y);
   ret = x - temp * y;
   if ((x < 0.0) != (y < 0.0))
      ret = ret - y;
   return ret;
}


#define XPI            (3.1415926535897932384626433832795)
#define XENTRY        (100)
#define XINCL        (XPI/2/XENTRY)

  static const double XSinTbl[] = {
        0.00000000000000000  , 0.015707317311820675 , 0.031410759078128292 , 0.047106450709642665 , 0.062790519529313374 ,
        0.078459095727844944 , 0.094108313318514325 , 0.10973431109104528  , 0.12533323356430426  , 0.14090123193758267  ,
        0.15643446504023087  , 0.17192910027940955  , 0.18738131458572463  , 0.20278729535651249  , 0.21814324139654256  ,
        0.23344536385590542  , 0.24868988716485479  , 0.26387304996537292  , 0.27899110603922928  , 0.29404032523230400  ,
        0.30901699437494740  , 0.32391741819814940  , 0.33873792024529142  , 0.35347484377925714  , 0.36812455268467797  ,
        0.38268343236508978  , 0.39714789063478062  , 0.41151435860510882  , 0.42577929156507272  , 0.43993916985591514  ,
        0.45399049973954680  , 0.46792981426057340  , 0.48175367410171532  , 0.49545866843240760  , 0.50904141575037132  ,
        0.52249856471594880  , 0.53582679497899666  , 0.54902281799813180  , 0.56208337785213058  , 0.57500525204327857  ,
        0.58778525229247314  , 0.60042022532588402  , 0.61290705365297649  , 0.62524265633570519  , 0.63742398974868975  ,
        0.64944804833018377  , 0.66131186532365183  , 0.67301251350977331  , 0.68454710592868873  , 0.69591279659231442  ,
        0.70710678118654757  , 0.71812629776318881  , 0.72896862742141155  , 0.73963109497860968  , 0.75011106963045959  ,
        0.76040596560003104  , 0.77051324277578925  , 0.78043040733832969  , 0.79015501237569041  , 0.79968465848709058  ,
        0.80901699437494745  , 0.81814971742502351  , 0.82708057427456183  , 0.83580736136827027  , 0.84432792550201508  ,
        0.85264016435409218  , 0.86074202700394364  , 0.86863151443819120  , 0.87630668004386369  , 0.88376563008869347  ,
        0.89100652418836779  , 0.89802757576061565  , 0.90482705246601958  , 0.91140327663544529  , 0.91775462568398114  ,
        0.92387953251128674  , 0.92977648588825146  , 0.93544403082986738  , 0.94088076895422557  , 0.94608535882754530  ,
        0.95105651629515353  , 0.95579301479833012  , 0.96029368567694307  , 0.96455741845779808  , 0.96858316112863108  ,
        0.97236992039767667  , 0.97591676193874743  , 0.97922281062176575  , 0.98228725072868872  , 0.98510932615477398  ,
        0.98768834059513777  , 0.99002365771655754  , 0.99211470131447788  , 0.99396095545517971  , 0.99556196460308000  ,
        0.99691733373312796  , 0.99802672842827156  , 0.99888987496197001  , 0.99950656036573160  , 0.99987663248166059  ,
        1.00000000000000000  };

double XSin( double x )
{
    int s = 0 , n;
    double dx , sx , cx;
    if( x < 0 )
        s = 1 , x = -x;
    x = my_fmod( x , 2 * XPI );
    if( x > XPI )
        s = !s , x -= XPI;
    if( x > XPI / 2 )
        x = XPI - x;
    n = (int)( x / XINCL );
    dx = x - n * XINCL;
    if( dx > XINCL / 2 )
        ++n , dx -= XINCL;
    sx = XSinTbl[n];
    cx = XSinTbl[XENTRY-n];
    x = sx + dx*cx - (dx*dx)*sx/2
        - (dx*dx*dx)*cx/6 
        + (dx*dx*dx*dx)*sx/24
        ;
     
    return s ? -x : x;
}
 
double XCos( double x )
{
    return XSin( x + XPI/2 );
}


/*********************************FFT*************************************
                         快速傅里叶变换C函数
函数简介：此函数是通用的快速傅里叶变换C语言函数，移植性强，以下部分不依
          赖硬件。此函数采用联合体的形式表示一个复数，输入为自然顺序的复
          数（输入实数是可令复数虚部为0），输出为经过FFT变换的自然顺序的
          复数
使用说明：使用此函数只需更改宏定义FFT_N的值即可实现点数的改变，FFT_N的
          应该为2的N次方，不满足此条件时应在后面补0
函数调用：FFT(s);
时    间：2010-2-20
版    本：Ver1.0
参考文献：     
**********************************************************************/

#define PI 3.1415926535897932384626433832795028841971               //定义圆周率值
#define FFT_N 1024                                                  //定义福利叶变换的点数

struct compx     //定义一个复数结构
	{
		float real;
		float imag;
	};                  

struct compx s1[FFT_N+16];           //FFT输入和输出：从S[1]开始存放，根据大小自己定义
struct compx s2[FFT_N+16];           //FFT输入和输出：从S[1]开始存放，根据大小自己定义


/*******************************************************************
函数原型：struct compx EE(struct compx b1,struct compx b2)  
函数功能：对两个复数进行乘法运算
输入参数：两个以联合体定义的复数a,b
输出参数：a和b的乘积，以联合体的形式输出
*******************************************************************/
struct compx EE(struct compx a,struct compx b)      
{
	 struct compx c;
	 c.real=a.real*b.real-a.imag*b.imag;
	 c.imag=a.real*b.imag+a.imag*b.real;
	 return(c);
}

/*****************************************************************
函数原型：void FFT(struct compx *xin,int N)
函数功能：对输入的复数组进行快速傅里叶变换（FFT）
输入参数：*xin复数结构体组的首地址指针，struct型
*****************************************************************/
void FFT(struct compx *xin)
{
	int f,m,nv2,nm1,i,k,l,j=0;
	struct compx u,w,t;

	nv2=FFT_N/2;                  //变址运算，即把自然顺序变成倒位序，采用雷德算法
	nm1=FFT_N-1;  
	for(i=0;i<nm1;i++)        
	{
		if(i<j)                    //如果i<j,即进行变址
		{
			t=xin[j];           
			xin[j]=xin[i];
			xin[i]=t;
		}
		k=nv2;                    //求j的下一个倒位序
		
		while(k<=j)               //如果k<=j,表示j的最高位为1   
		{           
			j=j-k;                 //把最高位变成0
			k=k/2;                 //k/2，比较次高位，依次类推，逐个比较，直到某个位为0
		}
		
		j=j+k;                   //把0改为1
	}
	 
	{  //FFT运算核，使用蝶形运算完成FFT运算
		int le,lei,ip;                           
		f=FFT_N;
		for(l=1;(f=f/2)!=1;l++)                  //计算l的值，即计算蝶形级数
			;
		for(m=1;m<=l;m++)                           // 控制蝶形结级数
		{                                           //m表示第m级蝶形，l为蝶形级总数l=log（2）N
			le=2<<(m-1);                            //le蝶形结距离，即第m级蝶形的蝶形结相距le点
			lei=le/2;                               //同一蝶形结中参加运算的两点的距离
			u.real=1.0;                             //u为蝶形结运算系数，初始值为1
			u.imag=0.0;
			w.real=XCos(PI/lei);                     //w为系数商，即当前系数与前一个系数的商
			w.imag=-XSin(PI/lei);
			for(j=0;j<=lei-1;j++)                   //控制计算不同种蝶形结，即计算系数不同的蝶形结
			{
				for(i=j;i<=FFT_N-1;i=i+le)            //控制同一蝶形结运算，即计算系数相同蝶形结
				{
					ip=i+lei;                           //i，ip分别表示参加蝶形运算的两个节点
					t=EE(xin[ip],u);                    //蝶形运算，详见公式
					xin[ip].real=xin[i].real-t.real;
					xin[ip].imag=xin[i].imag-t.imag;
					xin[i].real=xin[i].real+t.real;
					xin[i].imag=xin[i].imag+t.imag;
				}
				u=EE(u,w);                           //改变系数，进行下一个蝶形运算
			}
		}
	}

}

u16 g_fft_index=0;

u16 qsqrt(u32 a)
{
  u32 rem = 0, root = 0, divisor = 0;
  u16 i;
  for(i=0; i<16; i++)
  {
    root <<= 1;
    rem = ((rem << 2) + (a>>30));
    a <<= 2;
    divisor = (root << 1) + 1;
    if(divisor <= rem)
    {
      rem -= divisor;
      root++;
    }
  }
  return root;
}

#define START_INDEX    10   //滤出低频干扰
u16 find_max_num_index(struct compx *data,u16 count)
{
	u16 i=START_INDEX;
	u16 max_num_index = i;
	//struct compx temp=data[i];
	float temp = data[i].real;
	for(i=START_INDEX;i<count;i++)
	{
		if(temp < data[i].real)
		{
			temp = data[i].real;
			max_num_index = i;
		}
	}
	printf("max_num_index=%d\r\n",max_num_index);
	return max_num_index; 
}

#define CORRECTED_VALUE	50   //粗略标定血液氧气含量   ，精准数据需要大量测量

void sp02_treated_fun(u16 max_index)
{
	float sp02_num=0;
	 
	delayl_init();	    	 //延时函数初始化	   
	OLED_Init();			//初始化OLED     
   
 	printf("\r\n zhiliu s1=%f,s2=%f \r\n",s1[0].real,s2[0].real);
	printf("\r\n s1=%f,s2=%f \r\n",s1[max_index].real,s2[max_index].real);
	if((s1[max_index].real*s2[0].real)>(s2[max_index].real*s1[0].real))  //if   ir>red      sp02>75%
	{
		sp02_num = (s2[max_index].real*s1[0].real)/(s1[max_index].real*s2[0].real);
		printf("\r\nsp02_num  : %f\r\n",sp02_num*100);
		printf("\r\n血氧含量为: %f\r\n",(1-sp02_num)*100+CORRECTED_VALUE);
		
		OLED_ShowString(0,0, "SpO2:",16);
		if((1-sp02_num)*100+CORRECTED_VALUE>99)
			OLED_ShowString(40,0, "99",16);
		else
			OLED_ShowNum(40,0,(1-sp02_num)*100+CORRECTED_VALUE,4,16);

		OLED_ShowString(80,0,"%",16); 
		OLED_ShowString(0,30,"Heart Rate:",12);   
		OLED_Refresh_Gram();//更新显示到OLED 	 
		
	}
	else   // sp02<75%
	{
		printf("\r\n 严重缺氧! \r\n");
		
		OLED_ShowString(0,0, "SpO2:",16);
		OLED_ShowString(40,0,"ANOXIA!",16);
		OLED_ShowString(0,30,"Heart Rate:",12);   
		OLED_Refresh_Gram();//更新显示到OLED 
	}	
}

void test_max30100_fun(void)
{
	u16 temp_num=0;
	u16 fifo_word_buff[15][2];
	u16 Heart_Rate=0;
	u16 s1_max_index=0;
	u16 s2_max_index=0;
	
	delayl_init();	    	 //延时函数初始化	   
	OLED_Init();			//初始化OLED     
	
	while(1)
	{
		temp_num = max10300_Bus_Read(INTERRUPT_REG);
		//if( (INTERRUPT_REG_HR_RDY&temp_num) && (INTERRUPT_REG_SPO2_RDY&temp_num) )
		if( INTERRUPT_REG_A_FULL&temp_num )
		{
			max10300_FIFO_Read(0x05,fifo_word_buff,15); //read the hr and spo2 data form fifo in reg=0x05
			{
				u16 i=0;
				for(i=0;i<15;i++)
				{ 
					if(g_fft_index < FFT_N)
					{
						s1[g_fft_index].real = fifo_word_buff[i][0];
						s1[g_fft_index].imag= 0;
						s2[g_fft_index].real = fifo_word_buff[i][1];
						s2[g_fft_index].imag= 0;
						g_fft_index++;
					}
				}
				if(g_fft_index>=FFT_N)
				{
						{ //printf()  fft data
							u16 index=0;
							for(index=0;index<40;index++)
							{					
								//printf("s1[%3d]= %f\r\n",index,s1[index].real);
								//printf("s2[%3d]= %f\r\n",index,s2[index].real);
							}
						}
					 FFT(s1);
					 FFT(s2);
					 for(i=0;i<FFT_N;i++) 
					 {
							s1[i].real=sqrtf(s1[i].real*s1[i].real+s1[i].imag*s1[i].imag);
							s2[i].real=sqrtf(s2[i].real*s2[i].real+s2[i].imag*s2[i].imag);
					 }

					{ //printf()  fft data
						u16 index=START_INDEX;
						for( ;index<60;index++)
						{	
						#ifdef SAMPLE_50
								printf("f=%3.3f HZ,s1[%3d] = %f \r\n",50.0/FFT_N*index,index,s1[index].real);
								printf("f=%3.3f HZ,s2[%3d] = %f \r\n",50.0/FFT_N*index,index,s2[index].real);
						#else
								printf("f=%3.3f HZ,s1[%3d] = %f \r\n",100.0/FFT_N*index,index,s1[index].real);
								printf("f=%3.3f HZ,s2[%3d] = %f \r\n",100.0/FFT_N*index,index,s2[index].real);
						#endif
						}
						
					}
					s1_max_index = find_max_num_index(s1, 60);
					s2_max_index = find_max_num_index(s2, 60);
					if(s1_max_index == s2_max_index)	
					{
				#ifdef SAMPLE_50	 
						Heart_Rate =  60*50*((s1_max_index+s2_max_index )/2)/FFT_N;
				#else
						Heart_Rate =  60*100*((s1_max_index+s2_max_index )/2)/FFT_N;
				#endif
						printf("\r\n心率为: %d\r\n",Heart_Rate);
						
						sp02_treated_fun(s1_max_index);	
						
						OLED_ShowString(0,0, "SpO2:",16);
						OLED_ShowString(0,30,"Heart_Rate:",12); 
						OLED_ShowNum(70,30,Heart_Rate-10,4,16);
						OLED_Refresh_Gram();//更新显示到OLED 
											
					}
					else
					{
						printf("\r\n 测量失败，  请压好重新测量!\r\n");
						
						OLED_ShowString(0,0, "SpO2:",16);
						OLED_ShowString(0,30,"Heart_Rate:",12); 
						OLED_ShowString(70,30,"ERROR",16); 
						OLED_Refresh_Gram();//更新显示到OLED 
					}
					g_fft_index = 0;
				}
			}
		/*	ic 自动管理FIFO指针 不用参与
			max10300_Bus_Write(0x02, 0x00);   //set FIFO write Pointer reg = 0x00 for clear it
			max10300_Bus_Write(0x03, 0x00);	//set Over Flow Counter  reg = 0x00 for clear it
			max10300_Bus_Write(0x04, 0x0f);	//set FIFO Read Pointer  reg = 0x0F 
			max10300_Bus_Write(INTERRUPT_REG, 0x00); // all interrupt clear
		*/
		}
	}
}

