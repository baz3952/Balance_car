#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define PWMA   TIM1->CCR1  //PA8

#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(13)
#define BIN2   PBout(12)

#define PWMB   TIM1->CCR4  //PA11

void Motor_Init(void);
void Set_Pwm(int moto1,int moto2);
int myabs(int a);
void Xianfu_Pwm(void);
u8 Turn_Off(float angle, float voltage);
#endif
