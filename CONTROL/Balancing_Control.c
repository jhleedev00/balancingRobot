/*
 * Balancing_Control.c
 *
 * Created: 2024-12-30 오후 5:36:58
 *  Author: user
 */ 
#include "Balancing_Control.h"

// constrain 함수 (표준 C 라이브러리에 있을 가능성이 높음)
int constrain(int x, int a, int b)
{
	if (x < a) return a;
	if (x > b) return b;
	return x;
}


void BalancingControl(float balPwm, float compRotate, float drivePwm)
{
	const int pwmMax = 255; // PWM 최대값
	int left_pwm, right_pwm;

	// 균형 PWM과 구동 PWM 합산 후 범위 제한
	float basePwm = constrain(balPwm + drivePwm, -pwmMax, pwmMax);

	// 좌우 모터 PWM 계산 (회전 보정 적용)
	float right = basePwm + compRotate;
	float left = basePwm - compRotate;
	
	/*
	basePwm > 0: 전진 또는 제자리 회전
		left > 0 && right > 0: 전진
		left < 0 && right < 0: (발생하지 않음, 로직 오류)
		left > 0 && right < 0: 좌회전
		left < 0 && right > 0: 우회전
	basePwm < 0: 후진 또는 제자리 회전
		left < 0 && right < 0: 후진
		left > 0 && right > 0: (발생하지 않음, 로직 오류)
		left < 0 && right > 0: 좌회전
		left > 0 && right < 0: 우회전
		basePwm == 0: 정지 또는 제자리 회전
	compRotate > 0: 좌회전
	compRotate < 0: 우회전
	compRotate == 0: 정지
	*/
	
	// 모터 방향 및 PWM 값 설정
	if(basePwm > 0)// 전진 또는 제자리 회전
	{
		if(left > 0)// 전진 또는 좌회전
		{
			Left_Motor_Forward();
			left_pwm = (int)left;
		}
		else { // 우회전
			Left_Motor_Backward();
			left_pwm = (int)(-left);
		}

		if(right > 0)// 전진 또는 우회전
		{ 
			Right_Motor_Forward();
			right_pwm = (int)right;
		}
		else// 좌회전
		{ 
			Right_Motor_Backward();
			right_pwm = (int)(-right);
		}
	}
	else if(basePwm < 0)// 후진 또는 제자리 회전
	{
		if(left < 0)// 후진 또는 우회전
		{ 
			Left_Motor_Backward();
			left_pwm = (int)(-left);
		}
		else// 좌회전
		{ 
			Left_Motor_Forward();
			left_pwm = (int)left;
		}

		if(right < 0)// 후진 또는 좌회전
		{ 
			Right_Motor_Backward();
			right_pwm = (int)(-right);
		}
		else{// 우회전
			Right_Motor_Forward();
			right_pwm = (int)right;
		}
	}
	else// 정지 또는 제자리 회전
	{ 
		if(compRotate > 0)// 좌회전
		{ 
			Left_Motor_Forward();
			Right_Motor_Backward();
			left_pwm = (int)compRotate;
			right_pwm = (int)compRotate;
		}
		else if(compRotate < 0)// 우회전
		{ 
			Left_Motor_Backward();
			Right_Motor_Forward();
			left_pwm = (int)(-compRotate);
			right_pwm = (int)(-compRotate);
		}
		else// 정지
		{
			left_pwm = 0;
			right_pwm = 0;
		}
	}	

	// PWM 값 범위 제한 (혹시 모를 오류 방지)
	left_pwm = constrain(left_pwm, 0, pwmMax);
	right_pwm = constrain(right_pwm, 0, pwmMax);
	
	Left_Motor_PWM(left_pwm);
	Right_Motor_PWM(right_pwm);
}

void BalancingTest(PID_Controller balance_pid)
{
	if(balance_pid.output >= 0.0f)
	{
		Left_Motor_Forward();
		Right_Motor_Forward();
		Left_Motor_PWM((uint8_t)balance_pid.output);
		Right_Motor_PWM((uint8_t)balance_pid.output);
	}
	else
	{
		Left_Motor_Backward();
		Right_Motor_Backward();
		Left_Motor_PWM((uint8_t)(-balance_pid.output));
		Right_Motor_PWM((uint8_t)(-balance_pid.output));
	}
	
	printf("phi_est (Roll) = %0.2f, theta_est (Pitch) = %0.2f, psi_est (Yaw) = %0.2f\n", psi_est, psi_est_kalman, rotate_pid.output);
}