/**
 * @brief MPU6050 센서 헤더 파일
 * @file MPU6050.h
 * @author ground_rlc
 * @date 2024-08-26
 * @version 0.0.1
 *
 * @details 이 헤더 파일은 MPU6050 센서를 제어하기 위한 상수, 함수 선언 및 필요한 포함 파일을 정의합니다.
 */

#ifndef MPU6050_H_
#define MPU6050_H_

// 필수 헤더 파일 포함
#include "../def.h"        // 공통 정의 파일
#include "../sysconfig.h"  // 시스템 설정 관련 파일
#include "../BSW/I2C.h"    // I2C 통신을 위한 헤더 파일

// MPU6050 I2C 주소 (기본값: 0x68)
#define MPU6050_ADDRESS 0x68 // 읽기/쓰기 비트를 포함하지 않은 주소
#define MPU6050_RA_WHO_AM_I 0x75
#define MPU6050_RA_SIGNAL_PATH_RESET 0x68
#define MPU6050_RA_PWR_MGMT_1 0x6B
#define MPU6050_RA_INT_ENABLE 0x38

// MPU6050 레지스터 주소 정의
#define MPU6050_RA_XG_OFFS_TC 0x00 // X축 자이로 오프셋 및 OTP 유효성 비트
#define MPU6050_RA_YG_OFFS_TC 0x01 // Y축 자이로 오프셋 및 OTP 유효성 비트
#define MPU6050_RA_ZG_OFFS_TC 0x02 // Z축 자이로 오프셋 및 OTP 유효성 비트
#define MPU6050_RA_X_FINE_GAIN 0x03 // X축 자이로 미세 조정
#define MPU6050_RA_Y_FINE_GAIN 0x04 // Y축 자이로 미세 조정
#define MPU6050_RA_Z_FINE_GAIN 0x05 // Z축 자이로 미세 조정
#define MPU6050_RA_XA_OFFS_H 0x06   // X축 가속도계 오프셋 상위 바이트
#define MPU6050_RA_XA_OFFS_L_TC 0x07 // X축 가속도계 오프셋 하위 바이트
#define MPU6050_RA_YA_OFFS_H 0x08   // Y축 가속도계 오프셋 상위 바이트
#define MPU6050_RA_YA_OFFS_L_TC 0x09 // Y축 가속도계 오프셋 하위 바이트
#define MPU6050_RA_ZA_OFFS_H 0x0A   // Z축 가속도계 오프셋 상위 바이트
#define MPU6050_RA_ZA_OFFS_L_TC 0x0B // Z축 가속도계 오프셋 하위 바이트

// 기타 필요한 레지스터 주소 정의 (샘플 레지스터로 나열)
// 전체 목록을 제공하기보다는 필요한 경우 추가 정의 가능
#define MPU6050_RA_SMPLRT_DIV 0x19  // 샘플 속도 분배기
#define MPU6050_RA_CONFIG 0x1A      // 설정 레지스터 (DLPF 설정 포함)
#define MPU6050_RA_GYRO_CONFIG 0x1B // 자이로 설정 레지스터
#define MPU6050_RA_ACCEL_CONFIG 0x1C // 가속도계 설정 레지스터

// 센서 데이터 출력 레지스터
#define MPU6050_RA_ACCEL_XOUT_H 0x3B // X축 가속도 데이터 상위 바이트
#define MPU6050_RA_ACCEL_XOUT_L 0x3C // X축 가속도 데이터 하위 바이트
#define MPU6050_RA_ACCEL_YOUT_H 0x3D // Y축 가속도 데이터 상위 바이트
#define MPU6050_RA_ACCEL_YOUT_L 0x3E // Y축 가속도 데이터 하위 바이트
#define MPU6050_RA_ACCEL_ZOUT_H 0x3F // Z축 가속도 데이터 상위 바이트
#define MPU6050_RA_ACCEL_ZOUT_L 0x40 // Z축 가속도 데이터 하위 바이트
#define MPU6050_RA_GYRO_XOUT_H 0x43  // X축 자이로 데이터 상위 바이트
#define MPU6050_RA_GYRO_XOUT_L 0x44  // X축 자이로 데이터 하위 바이트
#define MPU6050_RA_GYRO_YOUT_H 0x45  // Y축 자이로 데이터 상위 바이트
#define MPU6050_RA_GYRO_YOUT_L 0x46  // Y축 자이로 데이터 하위 바이트
#define MPU6050_RA_GYRO_ZOUT_H 0x47  // Z축 자이로 데이터 상위 바이트
#define MPU6050_RA_GYRO_ZOUT_L 0x48  // Z축 자이로 데이터 하위 바이트

// 초기화 함수 및 센서 데이터 읽기 함수 선언
extern uint8_t MPU6050_start(void);           // MPU6050 시작 및 연결 확인
void MPU6050_Init(void);               // MPU6050 초기화 함수
void mpu6050_read_gyro_X(uint8_t *buff); // X축 자이로 데이터 읽기
void mpu6050_read_gyro_Y(uint8_t *buff); // Y축 자이로 데이터 읽기
void mpu6050_read_gyro_Z(uint8_t *buff); // Z축 자이로 데이터 읽기
void mpu6050_read_accel_X(uint8_t *buff); // X축 가속도 데이터 읽기
void mpu6050_read_accel_Y(uint8_t *buff); // Y축 가속도 데이터 읽기
void mpu6050_read_accel_Z(uint8_t *buff); // Z축 가속도 데이터 읽기
void mpu6050_read_gyro_ALL(int16_t *buff); // 모든 자이로 축 데이터 읽기
void mpu6050_read_accel_ALL(int16_t *buff); // 모든 가속도 축 데이터 읽기
void mpu6050_accel_test(int16_t* buf); //가속도 테스트
void mpu6050_gyro_test(int16_t* buf);	//자이로 테스트

#endif /* MPU6050_H_ */
