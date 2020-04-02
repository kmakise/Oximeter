#ifndef _MPU6050_H
#define _MPU6050_H
#include "iic.h"
#include "usart.h"
#include <math.H>
#include "stm32f10x.h"
#define        SMPLRT_DIV          0x19        //陀螺仪采样率，典型值：0x07(125Hz)
#define        CONFIG_6050         0x1A        //低通滤波频率，典型值：0x06(5Hz)
#define        GYRO_CONFIG         0x1B        //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define        ACCEL_CONFIG        0x1C        //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define        ACCEL_XOUT_H        0x3B
#define        ACCEL_XOUT_L        0x3C
#define        ACCEL_YOUT_H        0x3D
#define        ACCEL_YOUT_L        0x3E
#define        ACCEL_ZOUT_H        0x3F
#define        ACCEL_ZOUT_L        0x40
#define        TEMP_OUT_H          0x41
#define        TEMP_OUT_L          0x42
#define        GYRO_XOUT_H         0x43
#define        GYRO_XOUT_L         0x44
#define        GYRO_YOUT_H         0x45
#define        GYRO_YOUT_L         0x46
#define        GYRO_ZOUT_H         0x47
#define        GYRO_ZOUT_L         0x48
#define        PWR_MGMT_1          0x6B        //电源管理，典型值：0x00(正常启用)
#define        WHO_AM_I            0x75        //IIC地址寄存器(默认数值0x68，只读)
#define        SlaveAddress        0xD0        //IIC写入时的地址字节数据，+1为读取

#define X_ACCEL_OFFSET 	-270
#define Y_ACCEL_OFFSET 	-360 
#define Z_ACCEL_OFFSET 	700 
#define X_GYRO_OFFSET 	34 
#define Y_GYRO_OFFSET		-6 
#define Z_GYRO_OFFSET 	21 

void MPU6050_Init(void);
void MPU6050_Display(void);
void MPU6050_Get_Angle(void);

typedef struct Angle
{
    double X_Angle;
    double Y_Angle;
    double Z_Angle; 
} MPU6050_Angle;
typedef struct Ang_velocity
{
		double X_Ang_speed;
    double Y_Ang_speed;
    double Z_Ang_speed;
}MPU6050_Ang_velocity;
#endif

