/*
 * @file kalman_filter.c
 * @brief 각도 추정을 위한 칼만 필터 구현
 */

#include "Kalman_Filter.h"
#include <math.h>

// 칼만 필터 상태 변수
static float k_predict;      // 예측된 각도 값
static float k_uncertainty;  // 시스템 불확실성

// 자이로 및 가속도계의 표준 편차
#define ANGLE_STD_DEV_OF_GYRO   0.005  // 자이로 센서의 표준 편차 (deg/s)
#define ANGLE_STD_DEV_OF_ACCEL  0.1   // 가속도 센서의 표준 편차 (degrees)
#define IMPACT_THRESHOLD         5.0  // Threshold for detecting sudden impacts (degrees)

/**
 * @brief 칼만 필터 상태를 초기화합니다.
 *
 * k_predict와 k_uncertainty 변수를 초기값으로 설정합니다.
 */
void kalman_filter_reset(void) {
    k_predict = 0.0f;         // 초기 각도 추정 값 설정
    k_uncertainty = 1.0f;     // 초기 불확실성 값 설정
}

/**
 * @brief 새로운 센서 데이터를 사용하여 칼만 필터를 업데이트합니다.
 *
 * @param gyro_angle_rate 자이로 센서로부터의 각속도 변화 값 (deg/s)
 * @param accel_angle 가속도 센서로부터 계산된 각도 값 (degrees)
 * @param dt 시간 간격 (초 단위)
 * @return 업데이트된 각도 추정 값 (degrees)
 */
float kalman_filter_update(float gyro_angle_rate, float accel_angle, float dt) {
    // 1. 현재 시스템 상태를 예측합니다.
    k_predict += dt * gyro_angle_rate;  // 자이로 센서 데이터를 이용하여 상태 예측

    // 2. 시스템의 불확실성을 계산합니다.
    k_uncertainty += dt * ANGLE_STD_DEV_OF_GYRO * ANGLE_STD_DEV_OF_GYRO;

    // Detect sudden deviations
//    float measurement_residual = fabs(accel_angle - k_predict);
//    if (measurement_residual > IMPACT_THRESHOLD) {
	    // Temporarily increase uncertainty during impacts
//	    k_uncertainty *= 2.0f;
//    }

    // 3. 칼만 게인을 계산합니다.
    float k_gain = k_uncertainty / (k_uncertainty + ANGLE_STD_DEV_OF_ACCEL * ANGLE_STD_DEV_OF_ACCEL);

    // 4. 측정값을 사용하여 예측 상태를 업데이트합니다.
    k_predict += k_gain * (accel_angle - k_predict);

    // 5. 업데이트된 상태의 불확실성을 계산합니다.
    k_uncertainty *= (1.0f - k_gain);

    return k_predict;  // 업데이트된 각도 반환
}