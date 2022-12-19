#ifndef __IIC_H
#define __IIC_H
	#include "stm32f10x.h"
	#include "delay.h"
	
	#define SDA_IN() {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=8<<12;}	//PB3输入模式
	#define SDA_OUT() {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=3<<12;}	//PB3输出模式
	
	#define IIC_SCL PBout(4)	//SCL
	#define IIC_SDA PBout(3)	//SDA
	#define READ_SDA PBin(3)	//输入SDA
	
	void IIC_Init(void);
	void IIC_Start(void);
	void IIC_Stop(void);
	u8 IIC_Wait_Ack(void);
	void IIC_Ack(void);
	void IIC_NAck(void);
	void IIC_Send_Byte(u8 txd);
	u8 IIC_Read_Byte(u8 ack);
#endif
