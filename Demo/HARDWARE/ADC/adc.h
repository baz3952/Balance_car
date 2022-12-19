#ifndef __ADC_H
#define __ADC_H
	#include "sys.h"
	#define Battery_Ch 8		//Í¨µÀ8
	void Batter_ADC_Init(void);
	u16 Get_ADC(u8 ch);
	float Get_battery_volt(void); 
#endif
