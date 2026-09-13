/*
 * PID_Motor_Control.h
 *
 * Created: 2026-03-26 오후 11:51:04
 *  Author: dl1wj
 */ 


#ifndef PID_MOTOR_CONTROL_H_
#define PID_MOTOR_CONTROL_H_

#include "../def.h"
#include "../sysconfig.h"

typedef struct{
	float kp;
	float ki;
	float kd;
	float setpoint;
	float integral;
	float previous_error;
	float output;
}PID_Controller;

extern PID_Controller balance_pid;
extern PID_Controller rotate_pid;

void PID_Init(PID_Controller *balance_pid, float kp, float ki, float kd);
void PID_Compute(PID_Controller *pid, float current_value, float dt);
#endif /* PID_MOTOR_CONTROL_H_ */