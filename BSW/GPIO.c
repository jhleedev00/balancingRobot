/*
 * GPIO.c
 *
 * Created: 2026-03-17 오전 12:40:51
 *  Author: dl1wj
 */ 

#include "GPIO.h"

void GPIO_Init()
{
	sbi(DDRD, PD3); // 출력핀으로 사용
	sbi(DDRD, PD4);	
}

void PD3_High()
{
	sbi(PORTD, PD3); // PD3 high
}

void PD3_Low()
{
	cbi(PORTD, PD3); // PD3 low
}

void PD4_High()
{
	sbi(PORTD, PD4); // PD3 high
}

void PD4_Low()
{
	cbi(PORTD, PD4); // PD3 low
}