/*
 * Angle_Calculation.h
 *
 * Created: 2026-03-25 오후 11:12:37
 *  Author: dl1wj
 */ 


#ifndef ANGLE_CALCULATION_H_
#define ANGLE_CALCULATION_H_

#include "MPU6050.h"
#include "../sysconfig.h"

// 추정된 각도 및 바이어스 변수 선언
extern double phi_est;   // 추정된 롤 각도
extern double theta_est; // 추정된 피치 각도
extern double psi_est;   // 추정된 요 각도

extern double biasX;     // X축 자이로 바이어스
extern double biasY;     // Y축 자이로 바이어스
extern double biasZ;     // Z축 자이로 바이어스

extern double alpha;    // 상보 필터의 알파 값

// 센서 데이터 및 계산 변수 선언
extern int32_t sumX;
extern int32_t sumY;
extern int32_t sumZ;

extern int16_t accel_buff[3];
extern int16_t gyro_buff[3];

extern double accelX, accelY, accelZ;
extern double gyroX, gyroY, gyroZ;

extern double phi_accel, theta_accel;
extern double dt;

//칼만 필터 적용
extern float phi_est_kalman;
extern float theta_est_kalman;
extern float psi_est_kalman;

// 함수 선언
void start_calibration(void);
void Roll_Pitch_Yaw_Calculation(void);

#endif /* ANGLE_CALCULATION_H_ */