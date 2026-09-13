/**
 * @file MPU6050.c
 * @brief MPU6050 센서 제어 함수 구현
 * @author ground_rlc
 * @date 2024-08-26
 * @version 0.0.1
 */

#include "MPU6050.h"

/**
 * @brief MPU6050을 I2C로 시작하는 함수
 * @details MPU6050 장치와 통신을 시작하고, WHO_AM_I 레지스터를 읽어 장치 주소를 확인합니다. 
 *          성공 시 0x68을 반환하며, 실패 시 0을 반환합니다.
 * @return uint8_t - 장치 주소 (0x68) 또는 오류 시 0
 */
uint8_t MPU6050_start(void) {
    uint8_t res = 0;

    // I2C 시작 조건과 장치 주소 전송
    if (i2c_start((MPU6050_ADDRESS << 1) | I2C_WRITE) == 0) {
        // WHO_AM_I 레지스터 읽기
        if (i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I, &res) == 0) {
            // 읽은 값이 0x68이면 장치 연결 성공
            if (res == 0x68) {
                return res;
            }
        }
    }

    // 실패 시 0 반환
    return 0;
}

/**
 * @brief MPU6050 초기화 함수
 * @details MPU6050의 다양한 레지스터를 설정하여 기본 동작 모드를 구성합니다. 
 *          슬립 모드를 해제하고, 저역 필터와 센서 범위를 설정합니다.
 */
void MPU6050_Init(void) {
    // 슬립 모드 해제
    i2c_write_byte(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0x00);

    // 저역 필터 설정 (가속도: 184Hz, 자이로: 188Hz)
    i2c_write_byte(MPU6050_ADDRESS, MPU6050_RA_CONFIG, 0x01);

    // 자이로 범위 설정 (1000 deg/s)
    i2c_write_byte(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 1 << 4);

    // 가속도 범위 설정 (2g)
    i2c_write_byte(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0x00);

    // 데이터 준비 인터럽트 비활성화
    i2c_write_byte(MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 0x00);

    // 신호 경로 리셋 비활성화
    i2c_write_byte(MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET, 0x00);
}

/**
 * @brief 모든 축의 자이로 데이터를 읽어오는 함수
 * @details MPU6050의 X, Y, Z 축 자이로 데이터를 읽어와 배열에 저장합니다.
 * @param buff int16_t 배열 포인터 (최소 3개의 요소를 가져야 함)
 */
void mpu6050_read_gyro_ALL(int16_t *buff) {
    uint8_t tmp[2];

    // X 축 자이로 데이터 읽기
    mpu6050_read_gyro_X(tmp);
    buff[0] = (int16_t)((tmp[0] << 8) | tmp[1]);

    // Y 축 자이로 데이터 읽기
    mpu6050_read_gyro_Y(tmp);
    buff[1] = (int16_t)((tmp[0] << 8) | tmp[1]);

    // Z 축 자이로 데이터 읽기
    mpu6050_read_gyro_Z(tmp);
    buff[2] = (int16_t)((tmp[0] << 8) | tmp[1]);
}

/**
 * @brief 모든 축의 가속도 데이터를 읽어오는 함수
 * @details MPU6050의 X, Y, Z 축 가속도 데이터를 읽어와 배열에 저장합니다.
 * @param buff int16_t 배열 포인터 (최소 3개의 요소를 가져야 함)
 */
void mpu6050_read_accel_ALL(int16_t *buff) {
    uint8_t tmp[2];

    // X 축 가속도 데이터 읽기
    mpu6050_read_accel_X(tmp);
    buff[0] = (int16_t)((tmp[0] << 8) | tmp[1]);

    // Y 축 가속도 데이터 읽기
    mpu6050_read_accel_Y(tmp);
    buff[1] = (int16_t)((tmp[0] << 8) | tmp[1]);

    // Z 축 가속도 데이터 읽기
    mpu6050_read_accel_Z(tmp);
    buff[2] = (int16_t)((tmp[0] << 8) | tmp[1]);
}

/**
 * @brief X 축 자이로 데이터를 읽어오는 함수
 * @details MPU6050의 X 축 자이로 데이터를 두 바이트 읽어와 버퍼에 저장합니다.
 * @param buff uint8_t 배열 포인터 (최소 2개의 요소를 가져야 함)
 */
void mpu6050_read_gyro_X(uint8_t *buff) {
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, buff);
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L, buff + 1);
}

/**
 * @brief Y 축 자이로 데이터를 읽어오는 함수
 * @details MPU6050의 Y 축 자이로 데이터를 두 바이트 읽어와 버퍼에 저장합니다.
 * @param buff uint8_t 배열 포인터 (최소 2개의 요소를 가져야 함)
 */
void mpu6050_read_gyro_Y(uint8_t *buff) {
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H, buff);
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L, buff + 1);
}

/**
 * @brief Z 축 자이로 데이터를 읽어오는 함수
 * @details MPU6050의 Z 축 자이로 데이터를 두 바이트 읽어와 버퍼에 저장합니다.
 * @param buff uint8_t 배열 포인터 (최소 2개의 요소를 가져야 함)
 */
void mpu6050_read_gyro_Z(uint8_t *buff) {
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H, buff);
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L, buff + 1);
}

/**
 * @brief X 축 가속도 데이터를 읽어오는 함수
 * @details MPU6050의 X 축 가속도 데이터를 두 바이트 읽어와 버퍼에 저장합니다.
 * @param buff uint8_t 배열 포인터 (최소 2개의 요소를 가져야 함)
 */
void mpu6050_read_accel_X(uint8_t *buff) {
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, buff);
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_L, buff + 1);
}

/**
 * @brief Y 축 가속도 데이터를 읽어오는 함수
 * @details MPU6050의 Y 축 가속도 데이터를 두 바이트 읽어와 버퍼에 저장합니다.
 * @param buff uint8_t 배열 포인터 (최소 2개의 요소를 가져야 함)
 */
void mpu6050_read_accel_Y(uint8_t *buff) {
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_H, buff);
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_L, buff + 1);
}

/**
 * @brief Z 축 가속도 데이터를 읽어오는 함수
 * @details MPU6050의 Z 축 가속도 데이터를 두 바이트 읽어와 버퍼에 저장합니다.
 * @param buff uint8_t 배열 포인터 (최소 2개의 요소를 가져야 함)
 */
void mpu6050_read_accel_Z(uint8_t *buff) {
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_H, buff);
    i2c_read_byte(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_L, buff + 1);
}
