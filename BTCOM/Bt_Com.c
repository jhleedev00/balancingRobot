/*
 * Bt_Com.c
 *
 * Created: 2026-03-27 오후 8:20:33
 *  Author: dl1wj
 */ 

#include "Bt_Com.h"

void parse_bluetooth_message(const char *btMessage)
{
	char command = btMessage[0];
	float value = 0.0f;
	char buffer[10];
	//Joystick control logic here
	int x;
	int y;
	
	switch (command)
	{	//PID 제어
		case 'C':
			value = atof(btMessage + 1);
			balance_pid.kp = value/10.0f;
			printf("BP = %0.2f, BI = %0.2f, BD = %0.2f\n",balance_pid.kp, balance_pid.ki, balance_pid.kd);
			break;
		case 'D':
			value = atof(btMessage + 1);
			balance_pid.ki = value/10.0f;
			printf("BP = %0.2f, BI = %0.2f, BD = %0.2f\n",balance_pid.kp, balance_pid.ki, balance_pid.kd);
			break;
		case 'G':
			value = atof(btMessage + 1);
			balance_pid.kd = value/10.0f;
			printf("BP = %0.2f, BI = %0.2f, BD = %0.2f\n",balance_pid.kp, balance_pid.ki, balance_pid.kd);
			break;
		//YAW PID 제어
		case 'H':
			value = atof(btMessage + 1);
			balance_pid.kp = value/10.0f;
			break;
		case 'I':
			value = atof(btMessage + 1);
			balance_pid.kp = value/10.0f;
			break;
		case 'K':
			value = atof(btMessage + 1);
			balance_pid.kp = value/10.0f;
			break;
		//주행 및 목표값 제어
		case 'F':
			balance_pid.setpoint += 1.0f;
			break;
		case 'B':
			balance_pid.setpoint -= 1.0f;
			break;
		case 'L':
			rotate_pid.setpoint += 5.0f;
			break;
		case 'R':
			rotate_pid.setpoint -= 5.0f;
			break;
		case 'Z':
			balance_pid.setpoint = 0.0f;
			rotate_pid.setpoint = 0.0f;
			break;
		//시스템 상태 및 조이스틱
		case 'O':
			motorState = MOTOR_ON;
			printf("motor = %d\n", motorState);
			break;
		case 'X':
			motorState == MOTOR_OFF;
			printf("motor = %d\n", motorState);
			break;
		case 'E':
			printf("EEPROM 저장\n");
			break;
		case 'J':
			x = atoi(btMessage + 1);
			y = atoi(btMessage + 3);
			break;
		default:
			UART_send_string("Error: Unknown Command\n");
			break;
	}	
}