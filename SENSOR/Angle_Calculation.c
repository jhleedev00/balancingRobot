/*
 * Angle_Calculation.c
 *
 * Created: 2024-08-28 오후 10:29:47
 *  Author: user
 */ 

#include "Angle_Calculation.h"

#define RAD_TO_DEG 57.2957795

// 추정된 각도 값들 (롤, 피치, 요)
double phi_est = 0.0;   // 롤 각도 추정 값
double theta_est = 0.0; // 피치 각도 추정 값
double psi_est = 0.0;   // 요 각도 추정 값

//칼만필터 추정 각도값
float phi_est_kalman = 0.0f;
float theta_est_kalman = 0.0f;
float psi_est_kalman = 0.0f;

// 상보 필터의 알파 값 (0과 1 사이의 값으로 상보 필터에서 가속도 센서와 자이로 센서의 비중을 결정)
double alpha = 0.98;    // 상보 필터의 알파 값

// 캘리브레이션을 위한 변수들
int32_t sumX = 0, sumY = 0, sumZ = 0;  // 자이로 데이터를 더할 변수들 (누적합)
int16_t accel_buff[3], gyro_buff[3];    // 가속도 및 자이로 데이터를 저장할 배열
double accelX, accelY, accelZ;          // 가속도 센서에서 읽은 값 (각 축별)
double gyroX, gyroY, gyroZ;             // 자이로 센서에서 읽은 값 (각 축별)
double phi_accel, theta_accel;          // 가속도를 이용한 롤과 피치 각도
double dt;                              // 시간 간격 (샘플링 주기)

// 캘리브레이션 오프셋 값들
int ax_offset = 0, ay_offset = 0, az_offset = 0; // 가속도 센서의 각 축 오프셋
int gx_offset = 0, gy_offset = 0, gz_offset = 0; // 자이로 센서의 각 축 오프셋

// 롤, 피치, 요 각도를 계산하는 함수
void Roll_Pitch_Yaw_Calculation(void)
{
	dt = 0.01;  // 샘플링 시간 간격 (예시로 10ms)
	mpu6050_read_accel_ALL(accel_buff);  // 가속도 센서 데이터를 읽어옴
	mpu6050_read_gyro_ALL(gyro_buff);   // 자이로 센서 데이터를 읽어옴

	// 가속도 데이터를 m/s^2로 변환하고 오프셋을 보정
	accelX = (accel_buff[0] - ax_offset) * 9.8 * 2 / 32768;  // X축 가속도 보정
	accelY = (accel_buff[1] - ay_offset) * 9.8 * 2 / 32768;  // Y축 가속도 보정
	accelZ = (accel_buff[2] - az_offset) * 9.8 * 2 / 32768;  // Z축 가속도 보정

	// 자이로 데이터를 deg/s로 변환하고 오프셋을 보정
	gyroX = (gyro_buff[0] - gx_offset) / 131.0;  // X축 자이로 보정
	gyroY = (gyro_buff[1] - gy_offset) / 131.0;  // Y축 자이로 보정
	gyroZ = (gyro_buff[2] - gz_offset) / 131.0;  // Z축 자이로 보정

	// 가속도 데이터를 이용하여 롤과 피치 각도를 계산 (단위: 도)
	phi_accel = atan2(accelY, accelZ) * RAD_TO_DEG;  // 롤 계산 (Y, Z 축의 가속도 값을 이용)
	theta_accel = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * RAD_TO_DEG;  // 피치 계산 (X, Y, Z 축의 가속도 값을 이용)

	// 자이로 데이터를 이용하여 각도를 계산 (단위: 도)
	phi_est += gyroX * dt;  // X축 자이로 각도 추정
	theta_est += gyroY * dt;  // Y축 자이로 각도 추정
	psi_est += gyroZ * dt;  // Z축 자이로 각도 추정

	// 상보 필터를 적용하여 각도를 보정 (자이로와 가속도 값의 가중 평균을 구함)
	phi_est = alpha * phi_est + (1.0 - alpha) * phi_accel;  // 롤 값 보정
	theta_est = alpha * theta_est + (1.0 - alpha) * theta_accel;  // 피치 값 보정
	
	//칼만 필터를 이용한 각도 추정
	//phi_est_kalman = kalman_filter_update(gyroX, phi_accel, dt);
	theta_est_kalman = kalman_filter_update(gyroY, theta_accel, dt);
	//psi_est_kalman = kalman_filter_update(gyroZ, psi_est, dt);
}

// 센서 캘리브레이션을 시작하는 함수
void start_calibration(void)
{
	// 센서 오프셋을 0으로 초기화
	ax_offset = 0;
	ay_offset = 0;
	az_offset = 0;
	gx_offset = 0;
	gy_offset = 0;
	gz_offset = 0;

	// 샘플을 여러 번 읽어 평균을 구하여 오프셋을 계산
	long sum_ax = 0, sum_ay = 0, sum_az = 0;  // 가속도 센서의 값들을 저장할 변수들
	long sum_gx = 0, sum_gy = 0, sum_gz = 0;  // 자이로 센서의 값들을 저장할 변수들
	uint16_t num_samples = 1000;  // 샘플 횟수 (1000번 샘플링)

	// 1000번 샘플을 읽어 각 축의 평균을 구함
	for (uint16_t i = 0; i < num_samples; i++) {
		mpu6050_read_accel_ALL(accel_buff);  // 가속도 데이터 읽기
		mpu6050_read_gyro_ALL(gyro_buff);   // 자이로 데이터 읽기

		sum_ax += accel_buff[0];  // X축 가속도 값 누적
		sum_ay += accel_buff[1];  // Y축 가속도 값 누적
		sum_az += accel_buff[2];  // Z축 가속도 값 누적

		sum_gx += gyro_buff[0];   // X축 자이로 값 누적
		sum_gy += gyro_buff[1];   // Y축 자이로 값 누적
		sum_gz += gyro_buff[2];   // Z축 자이로 값 누적

		_delay_ms(1);  // 1ms 대기 (각 샘플 사이에 1ms 간격 유지)
	}

	// 각 센서 축에 대해 평균을 계산하여 오프셋을 구함
	ax_offset = sum_ax / num_samples;  // X축 가속도 센서 오프셋 계산
	ay_offset = sum_ay / num_samples;  // Y축 가속도 센서 오프셋 계산
	az_offset = sum_az / num_samples;  // Z축 가속도 센서 오프셋 계산

	gx_offset = sum_gx / num_samples;  // X축 자이로 센서 오프셋 계산
	gy_offset = sum_gy / num_samples;  // Y축 자이로 센서 오프셋 계산
	gz_offset = sum_gz / num_samples;  // Z축 자이로 센서 오프셋 계산

	// 가속도 센서의 Z축 오프셋을 보정 (Z축이 중력 방향으로 가정되므로, 보정값은 16384로 설정)
	az_offset = 16384 - az_offset;   // Z축 오프셋 보정

	// 캘리브레이션 결과를 출력 (디버깅 용도)
	printf("Calibrated Offsets:\n");
	printf("ax_offset: %d, ay_offset: %d, az_offset: %d\n", ax_offset, ay_offset, az_offset);
	printf("gx_offset: %d, gy_offset: %d, gz_offset: %d\n", gx_offset, gy_offset, gz_offset);
}

