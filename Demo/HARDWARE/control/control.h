#ifndef __CONTROL_H
#define __CONTROL_H
	#include "sys.h"
	#include "mpu6050.h"
	#include "inv_mpu.h"
	#include "encoder.h"
	#include "motor.h"
	#include "adc.h"
	
	#define PI 3.14159265

	void EXTI9_5_IRQHandler(void);
	int Vertical_PD(float measure, float Gyro);
	int Velocity_PI(int Speed_measure);
	int Turn(int gyro_Z);
#endif
