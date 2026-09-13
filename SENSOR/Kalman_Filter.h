/*
 * Kalman_Filter.h
 *
 * Created: 2026-03-26 오후 9:35:50
 *  Author: dl1wj
 */ 


#ifndef KALMAN_FILTER_H_
#define KALMAN_FILTER_H_

/**
* @brief 칼만 필터 상태 변수를 초기화 합니다.
* 이 함수는 필터의 예측 값과 불확실성을 초기 상태로 재설정합니다.
* 새로운 필터 적용을 시작하거나 데이터를 초기화 할 때 호출해야 합니다.                                                                     
*/
void kalman_filter_reset();

/**
*@brief 칼만 필터를 사용하여 센서 데이터를 기반으로 각도를 업데이트합니다.
*이 함수는 자이로스코프와 가속도계를 이용하여 각도를 추정을 보정합니다.
*@param gyro_angle_rate 자이로스코프에서 측정항 각속도 (deg/s 단위).
*@param accel_angle 가속도계에서 계산된 각도 (degrees 단위)
*@param dt 업데이트에 사용되는 시간 간격 (초 단위)
*@return 업데이트된 각도 추정 값 (degrees 단위)
*/
float kalman_filter_update(float gyro_angle_rate, float accel_angle, float dt);



#endif /* KALMAN_FILTER_H_ */