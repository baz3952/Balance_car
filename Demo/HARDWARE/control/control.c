#include "control.h"	

float pitch,roll,yaw; 								  			 //ŷ����(��̬��)
short aacx,aacy,aacz;													 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;											 //������ԭʼ����
int   Encoder_Left,Encoder_Right;         		 //���ұ��������������
int 	Moto1=0,Moto2=0;												 //������������ո��������PWM
int measure;																	 //�������ٶȲ���ֵ

int Vertical_out,Velocity_out,Turn_out;		//�������

float Voltage;  					 //��ص�ѹ������صı���
float Med_Angle = -8.5;						//��е��ֵ

float Vertical_Kp = -345,		//ֱ����Kp 300
	  Vertical_Kd = -1.0;		//ֱ����Kd 0.9

float Velocity_Kp = 110,		//�ٶȻ�Kp
	  Velocity_Ki = 0.55;			//�ٶȻ�Kd

float Turn_Kp = 0,
	  Turn_Kd = 0;

void EXTI9_5_IRQHandler(void)
{    
	int PWM_out;
	if(PBin(5)==0)		
	{
		EXTI->PR=1<<5;                                          //===���LINE5�ϵ��жϱ�־λ
		Voltage = Get_battery_volt();
		mpu_dmp_get_data(&pitch,&roll,&yaw);										//===�õ�ŷ���ǣ���̬�ǣ�������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								//===�õ�����������
		Encoder_Left=-Read_Encoder(2);                          //===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
		Encoder_Right=Read_Encoder(4);                          //===��ȡ��������ֵ
		measure = (Encoder_Left + Encoder_Right);								//���������ٶȸ�����ֵ
		Vertical_out = Vertical_PD(pitch, gyroy);								//ֱ��������
		Velocity_out = Velocity_PI(measure);										//�ٶȻ�����
		Turn_out = Turn(gyroz);																 	//===����У��С����ԭ��תȦ������
		
		PWM_out = Vertical_out + Velocity_out;										//PWM���
		Moto1=PWM_out - Turn_out;                 								//===�������ֵ������PWM
		Moto2=PWM_out + Turn_out;                 								//===�������ֵ������PWM
		
	  Xianfu_Pwm();  																						//===PWM�޷�
		if(Turn_Off(pitch,Voltage) == 0)													//===���Ƕ��Ƿ�����
			Set_Pwm(Moto1,Moto2);                                   //===��ֵ��PWM�Ĵ���
	}  	
} 

//ֱ����PD

int Vertical_PD(float measure, float Gyro)
{
	int PWM_out;
	
	PWM_out = Vertical_Kp*(measure - Med_Angle) + Vertical_Kd*Gyro;
	return PWM_out;
}

//�ٶȻ�PI
int Velocity_PI(int Speed_measure)
{
	static int Encoder_err, Encoder_err_low, Encoder_err_low_last, Encoder_sum, Movement;
	static int PWM_out;
	static float Target_Velocity = 200;
	
//	if(Flag_front == 1)			Movement = Target_Velocity / 2;
//	else if(Flag_back == 1)    	Movement = -Target_Velocity / 2;
//	else if(Flag_jingzhi == 1)	Movement = 0;
//	else if(1 == Flag_Quick)	Movement = Target_Velocity;

//	else Movement = 0;
	Movement = 0;
	
	Encoder_err = 0 - Speed_measure;
	Encoder_err_low = 0.3 * Encoder_err + 0.7 * Encoder_err_low_last;
	Encoder_err_low_last = Encoder_err_low;
	Encoder_sum += Encoder_err_low;
	Encoder_sum = Encoder_sum + Movement;
	if(Encoder_sum > 10000) Encoder_sum = 10000;
	if(Encoder_sum < -10000) Encoder_sum = -10000;
	
	PWM_out = Velocity_Kp*Encoder_err + Velocity_Ki*Encoder_sum;
	
	if(Turn_Off(pitch,Voltage) == 1) Encoder_sum = 0;
		
	return PWM_out;
}

//ת��
int Turn(int gyro_Z)
{
	int PWM_out;
	static float Turn_Target, Turn_Amplitude=40;
	float Kp = Turn_Kp,Kd;			
	
//	if(1 == Flag_Left)	        Turn_Target = -Turn_Amplitude/2;
//	else if(1 == Flag_Right)	  Turn_Target = Turn_Amplitude/2;
//	else if(1 == Flag_Turn_jingzhi)	 Turn_Target = 0;
//	else Turn_Target = 0;
	Turn_Target = 0;
//	
//	if(1 == Flag_front || 1 == Flag_back)  Kd = Turn_Kd;        
//	else Kd = 0;   
	Kd = 0;
	
	PWM_out = Turn_Target * Kp + gyro_Z * Kd;
	
	return PWM_out;
}
