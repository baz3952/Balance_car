#ifndef __KALMAN_H
#define __KALMAN_H
	#include "mpu6050.h"
	#include <math.h>
	extern float Kalman_roll;			//���������
	extern float Kalman_pitch;			//���������
	extern short temperature;			//�������¶�����

	void Angle_Calcu(void);
	void Kalman_Filter_X(float Accel,float Gyro);
	void Kalman_Filter_Y(float Accel,float Gyro);
#endif
