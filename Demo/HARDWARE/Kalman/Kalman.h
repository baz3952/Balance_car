#ifndef __KALMAN_H
#define __KALMAN_H
	#include "mpu6050.h"
	#include <math.h>
	extern float Kalman_roll;			//解算后俯仰角
	extern float Kalman_pitch;			//解算后横滚角
	extern short temperature;			//陀螺仪温度数据

	void Angle_Calcu(void);
	void Kalman_Filter_X(float Accel,float Gyro);
	void Kalman_Filter_Y(float Accel,float Gyro);
#endif
