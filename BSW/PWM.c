/*
 * PWM.c
 *
 * Created: 2026-03-17 오전 1:06:59
 *  Author: dl1wj
 */ 
#include "PWM.h"

void PWM_Init()
{
	// 1. 해당 핀을 출력(Output)으로 설정
	sbi(DDRD, PD5);
	sbi(DDRB, PB2);

	// 2. Timer0 설정 (8-bit) - PD5 사용
	// COM0B1:1, COM0B0:0 -> 비반전 모드
	// WGM01:1, WGM00:1 -> Fast PWM mode (Mode 3, TOP=0xFF)
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);
	sbi(TCCR0A, COM0B1);
	// CS01:1 -> Prescaler 64
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);
	// 4. 초기 듀티비(Duty Cycle) 설정 (0~255)
	OCR0B = 0; // OC0B 초기값 0

	// 3. Timer1 설정 (16-bit) - PB2 사용
	// COM1B1:1, COM1B0:0 -> 비반전 모드
	// WGM12:1, WGM10:1 -> Fast PWM 8-bit mode (Mode 5, TOP=0x00FF)
	sbi(TCCR1A, WGM10);
	sbi(TCCR1B, WGM12);
	sbi(TCCR1A, COM1B1);
	// Prescaler 64
	sbi(TCCR1B, CS11);
	sbi(TCCR1B, CS10);
	// 4. 초기 듀티비(Duty Cycle) 설정 (0~255)
	OCR1B = 0; // OC1B 초기값 0
}

void PWM_Set_A(uint8_t PWM)
{
	OCR0B = PWM;	//PD5(ENB)
	
	if(OCR0B >= 255)
	{
		OCR0B = 255;
	}
	
	if(OCR0B <= 0)
	{
		OCR0B = 0;
	}
}


void PWM_Set_B(uint8_t PWM)
{
	OCR1B = PWM;	//PB2(ENA)
	
	if(OCR1B >= 255)
	{
		OCR1B = 255;
	}
	
	if(OCR1B <= 0)
	{
		OCR1B = 0;
	}
}
