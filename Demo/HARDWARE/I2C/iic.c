#include "iic.h"

//PB6/7
//��ʼ��I2C
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	IIC_SCL = 1;
	IIC_SDA = 1;
}

//��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA = 1;IIC_SCL = 1;
	delay_us(2);
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 0;
}

//ֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;IIC_SDA = 1;
	delay_us(2);
}

//�ȴ�Ӧ���ź�
//return 1��ʧ��
//			 0���ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();						//SDA����Ϊ���
	IIC_SDA = 1;delay_us(2);
	IIC_SCL = 1;delay_us(2);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;	//ʱ�����0
	return 0;
}

//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL = 0;SDA_OUT();
	IIC_SDA = 0;delay_us(2);
	IIC_SCL = 1;delay_us(2);
	IIC_SCL = 0;
}

//������ACKӦ��
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;delay_us(2);
	IIC_SCL = 1;delay_us(2);
	IIC_SCL = 0;
}

//I2C����һ���ֽ�
//return 1:��Ӧ��
//			 0:��Ӧ��
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0;
	for(t = 0;t < 8;t++)
	{
		IIC_SDA = (txd&0x80) >> 7;
		txd <<= 1;
		delay_us(2);
		IIC_SCL = 1;delay_us(2);
		IIC_SCL = 0;delay_us(2);
	}
}

//��һ�ֽڣ�ack=1ʱ������ACK,ack=0ʱ������nACK
u8 IIC_Read_Byte(u8 ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA ����Ϊ����
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;
		delay_us(2);
	}
	if (!ack) IIC_NAck();//���� nACK
	else IIC_Ack(); //���� ACK
	return receive;
}
