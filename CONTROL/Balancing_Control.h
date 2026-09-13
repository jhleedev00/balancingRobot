/*
 * Balancing_Control.h
 *
 * Created: 2026-03-31 오후 9:36:45
 *  Author: dl1wj
 */ 


#ifndef BALANCING_CONTROL_H_
#define BALANCING_CONTROL_H_

#include "../def.h"
#include "../sysconfig.h"

int contrain(int x, int a,  int b);
void BalancingControl(float balPwm, float compRotate, float drivePwm);

#endif /* BALANCING_CONTROL_H_ */