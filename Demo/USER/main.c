#include "led.h"
#include "delay.h"
#include "usart.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "Kalman.h"
#include "timer.h"
#include "encoder.h"
#include "adc.h"
#include "pwm.h"
#include "motor.h"
#include "exti.h"

int main(void)
{
	LED_Init();
	delay_init();
	uart_init(115200);
	MPU_Init();
	mpu_dmp_init();
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	TIM1_PWM_Init(7199,0);
	Batter_ADC_Init();
	Motor_Init();
	USART1->CR2 &= ~(1<<8);
	USART1->CR3 &= ~(1<<9);
	EXTI_MPU6050_Init();
	while (1)
	{
		printf("%f/%f\r\n",pitch,roll);
		delay_ms(10);
	}
}
