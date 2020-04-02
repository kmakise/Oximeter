#include "mpu6050.h"
MPU6050_Angle dat;
MPU6050_Ang_velocity dat1;
/****************************************************************************** 
* 函数介绍： MPU6050 初始化函数 
* 输入参数： 无 
* 输出参数： 无 
* 返回值 ： 无 
* 备 注： 配置 MPU6050 测量范围：± 2000 °/s ± 4g 
******************************************************************************/
void MPU6050_Init(void)
{
    IIC_Write_Byte(SlaveAddress,PWR_MGMT_1, 	0x00);    //解除休眠状态     
    IIC_Write_Byte(SlaveAddress,SMPLRT_DIV, 	0x07);    //陀螺仪采样率，典型值：0x07(125Hz)     
    IIC_Write_Byte(SlaveAddress,CONFIG_6050, 	0x06);    //低通滤波频率，典型值：0x06(5Hz)     
    IIC_Write_Byte(SlaveAddress,GYRO_CONFIG, 	0x18);  	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)     
    IIC_Write_Byte(SlaveAddress,ACCEL_CONFIG, 0x08);  	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，4G，5Hz) 
}

/****************************************************************************** 
* 函数介绍： 连续读两个寄存器并合成 16 位数据 
* 输入参数： regAddr：数据低位寄存器地址 
* 输出参数： 无 
* 返回值 ： data：2 个寄存器合成的 16 位数据 
******************************************************************************/
int16_t MPU6050_Get_Data(uint8_t regAddr)
{
    uint8_t Data_H, Data_L;
    uint16_t dat;   
    Data_H = IIC_Read_Byte(SlaveAddress,regAddr);
    Data_L = IIC_Read_Byte(SlaveAddress,regAddr+1);
    dat = (Data_H << 8) | Data_L;  // 合成数据 
    return dat;
}

/****************************************************************************** 
* 函数介绍： 串口打印 6050 传感器读取的三轴加速度、陀螺仪及温度数据 
* 输入参数： 无 
* 输出参数： 无 
* 返回值 ： 无 
******************************************************************************/
void MPU6050_Display(void)
{
    /* 打印 x, y, z 轴加速度 */
    printf("ACCEL_X: %d\t", MPU6050_Get_Data(ACCEL_XOUT_H) + X_ACCEL_OFFSET);
    printf("ACCEL_Y: %d\t", MPU6050_Get_Data(ACCEL_YOUT_H) + Y_ACCEL_OFFSET);
    printf("ACCEL_Z: %d\t", MPU6050_Get_Data(ACCEL_ZOUT_H) + Z_ACCEL_OFFSET);
    
    /* 打印温度 */
    printf("TEMP: %0.2f\t", MPU6050_Get_Data(TEMP_OUT_H) / 340.0 + 36.53);
    
    /* 打印 x, y, z 轴角速度 */
    printf("GYRO_X: %d\t", MPU6050_Get_Data(GYRO_XOUT_H) + X_GYRO_OFFSET);
    printf("GYRO_Y: %d\t", MPU6050_Get_Data(GYRO_YOUT_H) + Y_GYRO_OFFSET);
    printf("GYRO_Z: %d\t", MPU6050_Get_Data(GYRO_ZOUT_H) + Z_GYRO_OFFSET);
    
    printf("\r\n");
}


void MPU6050_Get_Angle(void)
{   
	/* 计算x, y, z 轴倾角，返回弧度值*/
	dat.X_Angle = acos((MPU6050_Get_Data(ACCEL_XOUT_H) + X_ACCEL_OFFSET) / 8192.0);
	dat.Y_Angle = acos((MPU6050_Get_Data(ACCEL_YOUT_H) + Y_ACCEL_OFFSET) / 8192.0);
	dat.Z_Angle = acos((MPU6050_Get_Data(ACCEL_ZOUT_H) + Z_ACCEL_OFFSET) / 8192.0);

	/* 弧度值转换为角度值 */
	dat.X_Angle 	= dat.X_Angle * 57.29577;
	dat.Y_Angle 	= dat.Y_Angle * 57.29577;
	dat.Z_Angle 	= dat.Z_Angle * 57.29577;

	dat1.X_Ang_speed += (MPU6050_Get_Data(GYRO_XOUT_H) + X_GYRO_OFFSET)/ 65.5/4;
	dat1.Y_Ang_speed += (MPU6050_Get_Data(GYRO_YOUT_H) + Y_GYRO_OFFSET)/ 65.5/4;
	dat1.Z_Ang_speed += (MPU6050_Get_Data(GYRO_ZOUT_H) + Z_GYRO_OFFSET)/ 65.5/4;
	
	printf("X_Angle = %3d° ", (int)dat.X_Angle);
	printf("Y_Angle = %3d° ", (int)dat.Y_Angle);
	printf("Z_Angle = %3d° ", (int)dat.Z_Angle);

	printf("TEMP: %0.2f\t", MPU6050_Get_Data(TEMP_OUT_H) / 340.0 + 36.53);

	/* 打印 x, y, z 轴角速度 */
	printf("GYRO_X: %d\t", (int)dat1.X_Ang_speed);
	printf("GYRO_Y: %d\t", (int)dat1.Y_Ang_speed);
	printf("GYRO_Z: %d\t", (int)dat1.Z_Ang_speed);
	printf("\r\n");	
} 

