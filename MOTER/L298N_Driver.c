/*
 * L298N_Driver.c
 *
 * Created: 2024-12-18 오후 9:56:12
 *  Author: user
 */ 

/*
 * L298N_Driver.c
 *
 * Created: 2024-09-03 오후 11:31:11
 *  Author: user
 */ 

#include "L298N_Driver.h"

enum MOTOR motorState = MOTOR_OFF;

void L298N_Driver_Init(void){
	// PB0, PB1, PB2, PD7, PD6, PD5 핀을 출력으로 설정하는 함수

	// 왼쪽 모터 제어 핀 설정 (PB0 - IN1, PB1 - IN2, PB2 - ENA)
	sbi(DDRB, IN1);  // IN1 핀을 출력으로 설정
	sbi(DDRB, IN2);  // IN2 핀을 출력으로 설정
	sbi(DDRB, ENA);  // ENA 핀을 출력으로 설정
	
	// 오른쪽 모터 제어 핀 설정 (PD5 - IN3, PD6 - IN4, PD7 - ENB)
	sbi(DDRD, IN3);  // IN3 핀을 출력으로 설정
	sbi(DDRD, IN4);  // IN4 핀을 출력으로 설정
	sbi(DDRD, ENB);  // ENB 핀을 출력으로 설정
	

	// 타이머1 설정 - Fast PWM 모드, 비반전 모드, 분주율 64 (PB2)
	sbi(TCCR1A, WGM10); // Fast PWM 모드
	sbi(TCCR1A, WGM12);
	sbi(TCCR1A, COM1B1); // 비반전 모드 (PB2)
	sbi(TCCR1B, CS11); // 분주율 64
	sbi(TCCR1B, CS10);
	OCR1B = 0;  // 초기 출력 비교 값 설정 (모터 속도 0)

	// 타이머0 설정 - Fast PWM 모드, 비반전 모드, 분주율 64 (PD5)
	sbi(TCCR0A, WGM00); // Fast PWM 모드
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, COM0B1); // 비반전 모드 (PD5)
	sbi(TCCR0B, CS01); // 분주율 64
	sbi(TCCR0B, CS00);
	OCR0B = 0;  // 초기 출력 비교 값 설정 (모터 속도 0)
}

void Left_Motor_PWM(uint8_t pwm) {
	OCR1B = pwm;  // PB2 (ENA)
}

/*
* @brief	구동 모터 PWM 출력 함수
* @details  Sets the speed of rear motor
* @param	speed The speed value (0-255) to set for rear motor.
* @return	void
*/
void Right_Motor_PWM(uint8_t pwm) {
	OCR0B = pwm;  // PD5 (ENB)
}

/*
* @brief	왼쪽 모터 전진 함수
* @details  왼쪽 모터를 전진 방향으로 제어하기 위해 IN1 핀을 High, IN2 핀을 Low로 설정.
* @param	없음
* @return	void
*/
void Left_Motor_Forward(void)
{
	sbi(PORTB, IN1); // IN1 핀을 High로 설정하여 전진 방향
	cbi(PORTB, IN2); // IN2 핀을 Low로 설정
}

/*
* @brief	왼쪽 모터 후진 함수
* @details  왼쪽 모터를 후진 방향으로 제어하기 위해 IN1 핀을 Low, IN2 핀을 High로 설정.
* @param	없음
* @return	void
*/
void Left_Motor_Backward(void)
{
	cbi(PORTB, IN1); // IN1 핀을 Low로 설정
	sbi(PORTB, IN2); // IN2 핀을 High로 설정하여 후진 방향
}

/*
* @brief	왼쪽 모터 정지 함수
* @details  왼쪽 모터를 정지시키기 위해 IN1과 IN2 핀을 모두 High로 설정.
* @param	없음
* @return	void
*/
void Left_Motor_Stop(void)
{
	sbi(PORTB, IN1); // IN1 핀을 High로 설정하여 정지
	sbi(PORTB, IN2); // IN2 핀을 High로 설정
}

/*
* @brief	오른쪽 모터 전진 함수
* @details  오른쪽 모터를 전진 방향으로 제어하기 위해 IN3 핀을 Low, IN4 핀을 High로 설정.
* @param	없음
* @return	void
*/
void Right_Motor_Forward(void)
{
	sbi(PORTD, IN3); // IN3 핀을 High로 설정
	cbi(PORTD, IN4); // IN4 핀을 Low로 설정하여 후진 방향
}
/*
* @brief	오른쪽 모터 후진 함수
* @details  오른쪽 모터를 후진 방향으로 제어하기 위해 IN3 핀을 High, IN4 핀을 Low로 설정.
* @param	없음
* @return	void
*/
void Right_Motor_Backward(void)
{
	cbi(PORTD, IN3); // IN3 핀을 Low로 설정
	sbi(PORTD, IN4); // IN4 핀을 High로 설정하여 전진 방향
}

/*
* @brief	오른쪽 모터 정지 함수
* @details  오른쪽 모터를 정지시키기 위해 IN3과 IN4 핀을 모두 High로 설정.
* @param	없음
* @return	void
*/
void Right_Motor_Stop(void)
{
	sbi(PORTD, IN3); // IN3 핀을 High로 설정하여 정지
	sbi(PORTD, IN4); // IN4 핀을 High로 설정
}
