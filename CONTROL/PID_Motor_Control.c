/*
 * PID_Motor_Control.c
 *
 * Created: 2025-11-28 오후 11:04:31
 *  Author: user
 */ 

#include "PID_Motor_control.h"

//PID 컨트롤러 구조체 변수
PID_Controller balance_pid;
PID_Controller rotate_pid;

void PID_Init(PID_Controller *pid, float kp, float ki,float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->setpoint = 0.0f;
	pid->integral = 0.0f;
	pid->previous_error = 0.0f;
	pid->output = 0.0f;
}

void PID_Compute(PID_Controller *pid, float current_value, float dt)
{
	//오차 계산
	float error = pid->setpoint - current_value;
	
	//비례 항
	float proportional = pid->kp * error;
	//적분항
	pid->integral += error *dt;
	float integralTerm = pid->ki * pid->integral;
	
	//미분항
	float derivative = (error - pid->previous_error) / dt;
	float derivativeTerm = pid->kd * derivative;
	
	pid->output = proportional + integralTerm + derivativeTerm;
	
	pid->previous_error = error;
	
	if(pid->output > 255) pid->output = 255;
	else if(pid->output < -255) pid->output = -255;
}