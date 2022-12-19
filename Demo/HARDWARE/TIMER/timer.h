#ifndef __TIMER_H
#define __TIMER_H
	#include "sys.h"
	#include "Kalman.h"
	#include "inv_mpu.h"
	#include "encoder.h"
	#include <stdio.h>
	void TIM4_Int_Init(u16 arr,u16 psc);
	extern int	Encoder_Left,Encoder_Right; 
#endif
