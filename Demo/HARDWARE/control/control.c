#include "control.h"	

float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
short aacx,aacy,aacz;													 //加速度传感器原始数据
short gyrox,gyroy,gyroz;											 //陀螺仪原始数据
int   Encoder_Left,Encoder_Right;         		 //左右编码器的脉冲计数
int 	Moto1=0,Moto2=0;												 //计算出来的最终赋给电机的PWM
int measure;																	 //编码器速度测量值

int Vertical_out,Velocity_out,Turn_out;		//输出变量

float Voltage;  					 //电池电压采样相关的变量
float Med_Angle = -8.5;						//机械中值

float Vertical_Kp = -345,		//直立环Kp 300
	  Vertical_Kd = -1.0;		//直立环Kd 0.9

float Velocity_Kp = 110,		//速度环Kp
	  Velocity_Ki = 0.55;			//速度环Kd

float Turn_Kp = 0,
	  Turn_Kd = 0;

void EXTI9_5_IRQHandler(void)
{    
	int PWM_out;
	if(PBin(5)==0)		
	{
		EXTI->PR=1<<5;                                          //===清除LINE5上的中断标志位
		Voltage = Get_battery_volt();
		mpu_dmp_get_data(&pitch,&roll,&yaw);										//===得到欧拉角（姿态角）的数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								//===得到陀螺仪数据
		Encoder_Left=-Read_Encoder(2);                          //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(4);                          //===读取编码器的值
		measure = (Encoder_Left + Encoder_Right);								//将编码器速度给测量值
		Vertical_out = Vertical_PD(pitch, gyroy);								//直立环计算
		Velocity_out = Velocity_PI(measure);										//速度环计算
		Turn_out = Turn(gyroz);																 	//===用来校正小车在原地转圈的现象
		
		PWM_out = Vertical_out + Velocity_out;										//PWM输出
		Moto1=PWM_out - Turn_out;                 								//===计算左轮电机最终PWM
		Moto2=PWM_out + Turn_out;                 								//===计算右轮电机最终PWM
		
	  Xianfu_Pwm();  																						//===PWM限幅
		if(Turn_Off(pitch,Voltage) == 0)													//===检查角度是否正常
			Set_Pwm(Moto1,Moto2);                                   //===赋值给PWM寄存器
	}  	
} 

//直立环PD

int Vertical_PD(float measure, float Gyro)
{
	int PWM_out;
	
	PWM_out = Vertical_Kp*(measure - Med_Angle) + Vertical_Kd*Gyro;
	return PWM_out;
}

//速度环PI
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

//转向环
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
