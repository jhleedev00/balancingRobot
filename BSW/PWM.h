/*
 * PWM.h
 *
 * Created: 2026-03-17 오전 1:06:43
 *  Author: dl1wj
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "../def.h"
#include "../sysconfig.h"

extern void PWM_Init();
extern void PWM_Set_A(uint8_t PWM);
extern void PWM_Set_B(uint8_t PWM);

#endif /* PWM_H_ */