#ifndef L298N_DRIVER_H_
#define L298N_DRIVER_H_

#include "../def.h"
#include "../sysconfig.h"

#define IN1		PB1
#define IN2		PB0
#define ENA		PB2

#define IN3		PD7
#define IN4		PD6
#define ENB		PD5

enum MOTOR{
	MOTOR_OFF = 0, // Motor OFF 상태
	MOTOR_ON = 1   // Motor ON 상태
};

extern enum MOTOR motorState;

void Left_Motor_PWM(uint8_t pwm);
extern void Right_Motor_PWM(uint8_t pwm);
extern void L298N_Driver_Init(void);
extern void Left_Motor_Forward(void);
extern void Left_Motor_Backward(void);
extern void Left_Motor_Stop(void);
extern void Right_Motor_Forward(void);
extern void Right_Motor_Backward(void);
extern void Right_Motor_Stop(void);

#endif /* L298N_DRIVER_H_ */