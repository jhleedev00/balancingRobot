/*
 * fastcampus_balancingrobot.c
 *
 * Created: 2025-11-24 오후 11:04:58
 * Author : user
 */ 
#include "def.h"
#include "sysconfig.h"

#define TARGET_ANGLE 0.0f

#define DT 0.01f // 샘플링 시간 10ms

//PID 게인(이득)
#define KP 12.0f
#define KI 10.5f
#define KD 1.5f

//YAW PID 게인(이득)
#define KP_YAW 2.5f
#define KI_YAW 0.0f
#define KD_YAW 0.2f

uint8_t mpu6050_success_ret = 0;


int main(void)
{
	UART_init(BAUD_9600);
	GPIO_Init();
	PWM_Init();
	L298N_Driver_Init();
	I2C_Init();
	
	mpu6050_success_ret = MPU6050_start();
	
	if(mpu6050_success_ret) // I2C 통신 성공시
	{
		PD3_High();
		_delay_ms(1000);
		PD4_High();
		printf("MPU6050 Enable\n");
	}
	else // I2C 통신 실패시
	{
		printf("MPU6050 Disable\n");
	}
	
	MPU6050_Init();
	printf("MPU6050 Init OK\n"); 
	
	start_calibration();
	
	PD3_Low();
	PD4_Low();
	
	balance_pid.setpoint = TARGET_ANGLE;
	rotate_pid.setpoint = TARGET_ANGLE;
	
	PID_Init(&balance_pid, KP, KI, KD);
	PID_Init(&rotate_pid, KP_YAW, KI_YAW, KD_YAW);
	
    /* Replace with your application code */
    while (1) 
    {
		Roll_Pitch_Yaw_Calculation();
		PID_Compute(&balance_pid, theta_est_kalman, DT);
		PID_Compute(&rotate_pid, psi_est, DT);
		BalancingControl(balance_pid.output, rotate_pid.output, 0);
		
		if(uart_rx_flag) // 명령어 프로토콜 해석
		{	
			parse_bluetooth_message(response);
			// 수신 완료
			uart_rx_flag = 0;
		}
		
		
    }
}

