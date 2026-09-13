/*
 * sysconfig.h
 *
 * Created: 2026-03-17 오전 12:54:30
 *  Author: dl1wj
 */ 


#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "BSW/GPIO.h"
#include "BSW/UART.h"
#include "BSW/I2C.h"
#include "BSW//PWM.h"
#include "BSW/EEPROM.h"
#include "BTCOM/HC05.h"
#include "MOTER/L298N_Driver.h"	
#include "SENSOR/MPU6050.h"
#include "SENSOR/Angle_Calculation.h"
#include "SENSOR/Kalman_Filter.h"
#include "CONTROL/PID_Motor_Control.h"
#include "CONTROL/Balancing_Control.h"
#include "BTCOM/Bt_Com.h"


#endif /* SYSCONFIG_H_ */