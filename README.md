# Balancing Robot & Android Controller

[![MCU]()](https://www.microchip.com/) [![IDE]()](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) [![Platform]()](https://developer.android.com/) ![Language]() ![License]()

ATmega328P 마이크로컨트롤러 기반의 **2륜 역진자 자율 균형 로봇(Self-Balancing Robot)** 펌웨어 및 **안드로이드 블루투스 무선 조종/PID 튜닝 애플리케이션** 통합 프로젝트입니다.

6축 IMU(MPU6050) 센서 데이터에 \*\*1차원 칼만 필터(1D Kalman Filter)\*\*를 적용하여 정밀한 피치(Pitch) 각도를 추정하고, \*\*이중 루프 PID 제어 알고리즘(Balancing & Yaw Steering)\*\*과 L298N 모터 드라이버를 통해 실시간 자립 주행을 구현합니다. 전용 안드로이드 앱을 통해 무선 조이스틱 주행 제어 및 온더플라이(On-the-fly) PID 게인 튜닝을 지원합니다.

---

## 📑 목차 (Table of Contents)

1. [시스템 아키텍처](#-시스템-아키텍처-system-architecture)  
2. [하드웨어 사양 및 핀맵](#-하드웨어-사양-및-핀맵-hardware-specification)  
3. [펌웨어 소프트웨어 구조](#-펌웨어-소프트웨어-구조-firmware-architecture)  
4. [핵심 제어 알고리즘](#-핵심-제어-알고리즘-core-algorithms)  
5. [블루투스 통신 프로토콜](#-블루투스-통신-프로토콜-bluetooth-communication)  
6. [안드로이드 원격 제어 앱](#-안드로이드-원격-제어-앱-android-companion-app)  
7. [빌드 및 설치 방법](#-빌드-및-설치-방법-getting-started)

---

## 🏛 시스템 아키텍처 (System Architecture)

**Tier 1: Android Host Application (원격 조종 & 텔레메트리)**

| Virtual Joystick (JoystickView) 360° 터치 변위 연산 주행 명령(F, B, L, R, Z) | PID Tuning Dashboard 런타임 Kp, Ki, Kd 게인 조정 모터 ON/OFF 토글 | ConnectedThread RFCOMM 소켓 워커 스레드 논블로킹 스트리밍 |
| :---: | :---: | :---: |

↓ \[ Bluetooth SPP 무선 링크 (HC-05 모듈 / UART 9600 Baud) \] ↓

**Tier 2: ATmega328P 100Hz Real-Time Firmware (임베디드 제어 펌웨어)**

| Sub-block A: 명령 파서 & 저장소 Bt\_Com.c (시리얼 명령 파서) ⇄ EEPROM.c (튜닝된 PID 게인 영구 저장) |
| :---- |
| **Sub-block B: 센서 수집 및 1D 칼만 필터 상태 추정 파이프라인** MPU6050.c (I2C 100kHz 원시값) → Angle\_Calculation.c (1,000회 오프셋 보정) → Kalman\_Filter.c (자이로 각속도 적분 \+ 가속도 융합) → 추정 피치각(θ\_kalman) |
| **Sub-block C: 이중 루프 PID 제어 및 모터 믹싱 알고리즘** • Balance PID (Kp=12.0, Ki=10.5, Kd=1.5) & Rotate PID (Kp=2.5, Kd=0.2) • Balancing\_Control.c (모터 믹싱: BasePWM \= constrain(BalPwm \+ DrivePwm, ±255), 좌/우 모터 PWM 및 방향 판별) |
| **Sub-block D: 하드웨어 추상화 계층 (BSW)** • Timer 0 (PD5/OC0B) & Timer 1 (PB2/OC1B) Fast PWM (\~976Hz) • Direction GPIOs (좌: PB0/PB1, 우: PD7/PD6) |

↓ \[ 속도 PWM 듀티 신호 & H-Bridge 방향 논리 신호 \] ↓

**Tier 3: Hardware Actuation & Physical Plant (구동부 및 역진자 기구부)**

| L298N Dual H-Bridge 드라이버 모듈 → 좌/우 DC 기어드 모터 → 2륜 역진자 로봇 차체 (무게중심 및 관성 모멘트) |
| :---: |

↺ \[ 물리적 피드백: 로봇 차체의 실시간 기울기 각도(θ)와 각속도(ω)가 MPU6050 센서로 전달되는 폐루프 제어 \]

&nbsp;

---

## 🔌 하드웨어 사양 및 핀맵 (Hardware Specification)

### 1\. 주요 부품 사양

* **MCU**: Microchip ATmega328P (8-bit AVR RISC, 16MHz External Crystal, 5V)  
* **IMU 센서**: InvenSense MPU6050 (3축 가속도계 \+ 3축 자이로스코프, I2C)  
* **모터 드라이버**: L298N Dual H-Bridge Motor Driver Module  
* **구동 모터**: 2x 기어드 DC 모터 (차동 구동)  
* **무선 통신**: HC-05 Bluetooth Module (UART 9600 Baud)  
* **전원 구성**: 2S/3S Li-ion 배터리 \+ LM2596 강압 레귤레이터 (MCU 5V 공급)

### 2\. MCU 핀 할당표 (Pinout Mapping)

| 구분 | 핀 번호 (ATmega328P) | 기능 / 역할 | 연결 대상 |
| :---- | :---- | :---- | :---- |
| **I2C / TWI** | `PC4 (SDA)` | TWI 데이터 라인 (External Pull-up) | MPU6050 SDA |
|  | `PC5 (SCL)` | TWI 클록 라인 (External Pull-up) | MPU6050 SCL |
| **모터 제어 (좌)** | `PB0` | Left Motor Direction 1 (`IN1`) | L298N IN1 |
|  | `PB1` | Left Motor Direction 2 (`IN2`) | L298N IN2 |
|  | `PB2 (OC1B)` | Left Motor Speed PWM (Timer 1 Fast PWM) | L298N ENA |
| **모터 제어 (우)** | `PD7` | Right Motor Direction 1 (`IN3`) | L298N IN3 |
|  | `PD6` | Right Motor Direction 2 (`IN4`) | L298N IN4 |
|  | `PD5 (OC0B)` | Right Motor Speed PWM (Timer 0 Fast PWM) | L298N ENB |
| **USART** | `PD0 (RXD)` | Serial Data Receive (인터럽트 구동) | HC-05 TXD |
|  | `PD1 (TXD)` | Serial Data Transmit | HC-05 RXD |
| **상태 표시** | `PD3`, `PD4` | 시스템 초기화 및 센서 캘리브레이션 상태 LED | Status LEDs |

---

## 💻 펌웨어 소프트웨어 구조 (Firmware Architecture)

계층형 아키텍처(Layered Architecture)를 준수하여 하드웨어 제어, 신호 처리, 제어 로직을 모듈화했습니다.

BalancingRobot/

├── main.c                          \# 100Hz 메인 제어 루프 및 시스템 오케스트레이션

├── sysconfig.h                     \# 전체 모듈 인클루드 및 시스템 매크로 정의

├── def.h                           \# 기본 타입 및 비트 연산자 매크로 (sbi, cbi)

├── BSW/                            \# Board Support Package (하드웨어 레지스터 추상화)

│   ├── GPIO.c / GPIO.h             \# 디지털 입출력 제어 함수

│   ├── I2C.c / I2C.h               \# 하드웨어 TWI 마스터 모드 통신 (100kHz)

│   ├── PWM.c / PWM.h               \# Timer0/Timer1 Fast PWM 초기화 및 듀티 제어

│   ├── UART.c / UART.h             \# 9600 Baud UART 및 링버퍼 수신 인터럽트

│   └── EEPROM.c / EEPROM.h         \# PID 파라미터 비휘발성 저장/로드

├── SENSOR/                         \# 센서 데이터 수집 및 상태 추정

│   ├── MPU6050.c / MPU6050.h       \# MPU6050 레지스터 세팅 및 가속도/자이로 원시값 리딩

│   ├── Angle\_Calculation.c / .h    \# 삼각함수 기반 각도 연산 및 1,000회 오프셋 캘리브레이션

│   └── Kalman\_Filter.c / .h        \# 1차원 이산 칼만 필터 (Gyro \+ Accel 데이터 융합)

├── CONTROL/                        \# 제어 알고리즘

│   ├── PID\_Motor\_Control.c / .h    \# 균형(Balance) 및 회전(Rotate) PID 연산기

│   └── Balancing\_Control.c / .h    \# 차동 모터 믹싱, 데드밴드 보정 및 방향 판별

├── MOTER/                          \# 구동기 드라이버

│   └── L298N\_Driver.c / .h         \# L298N H-Bridge 핀 제어 (전진/후진/정지/PWM)

└── BTCOM/                          \# 통신 프로토콜 핸들러

    ├── HC05.c / HC05.h             \# 블루투스 하드웨어 인터페이스

    └── Bt\_Com.c / Bt\_Com.h         \# 명령 파서 (PID 게인 실시간 변경, 조이스틱 패킷 처리)

---

## ⚙️ 핵심 제어 알고리즘 (Core Algorithms)

### 1\. 센서 캘리브레이션 (Initial Calibration)

* 전원 인가 직후 로봇이 정지 상태일 때 가속도 및 자이로 센서 데이터를 1,000회 샘플링하여 평균 오프셋(`ax_offset`, `ay_offset`, `az_offset`, `gx_offset`, `gy_offset`, `gz_offset`)을 산출합니다.  
* Z축은 중력가속도(1g \= 16,384 LSB at ±2g)를 보정하여 정확한 수평 기준점을 확보합니다.

### 2\. 1D 칼만 필터 기반 피치 각도 추정 (Kalman Filtering)

가속도계의 진동 노이즈와 자이로스코프의 시간에 따른 적분 드리프트(Drift) 문제를 해결하기 위해 1차원 칼만 필터를 적용했습니다.

* **\[예측 단계 (Prediction)\]**  
* • 각도 예측: θ̂\_(k|k-1) \= θ̂\_(k-1|k-1) \+ Δt · θ̇\_gyro  
* • 오차 공분산 예측: P\_(k|k-1) \= P\_(k-1|k-1) \+ Δt · Q  
* \[업데이트 단계 (Update)\]  
* • 칼만 이득 계산: K\_k \= P\_(k|k-1) / (P\_(k|k-1) \+ R)  
* • 상태 추정치 갱신: θ̂\_(k|k) \= θ̂\_(k|k-1) \+ K\_k · (θ\_accel \- θ̂\_(k|k-1))  
* • 오차 공분산 갱신: P\_(k|k) \= (1 \- K\_k) · P\_(k|k-1)  
* Parameters:  
* • 자이로 프로세스 노이즈 공분산 (Q): 0.005  
* • 가속도 측정 노이즈 공분산 (R): 0.1  
* • 제어 및 샘플링 주기 (Δt): 10ms (100Hz)

### 3\. 이중 루프 PID 제어 및 모터 믹싱 (Dual PID & Motor Mixing)

* • **Balance PID Loop**:  
  * \- 목표 피치 각도 (Setpoint \= 0.0°)와 칼만 필터 추정 각도 (θ\_est\_kalman) 간의 오차를 입력받아 균형 유지 토크 출력 계산  
  * \- 기본 게인: Kp \= 12.0, Ki \= 10.5, Kd \= 1.5  
* • Rotate PID Loop:  
  * \- 회전 목표 각도 (rotate\_pid.setpoint)와 요(Yaw) 추정치 간의 오차를 기반으로 조향 보정값 (CompRotate) 산출  
  * \- 기본 게인: Kp \= 2.5, Ki \= 0.0, Kd \= 0.2  
* • Motor Mixing 연산:  
  * BasePWM \= constrain(Pwm\_bal \+ DrivePWM, \-255, 255\)  
  * PWM\_Right \= BasePWM \+ CompRotate  
  * PWM\_Left \= BasePWM \- CompRotate

---

## 📡 블루투스 통신 프로토콜 (Bluetooth Communication)

안드로이드 앱과 로봇 간 시리얼 통신(9600 Baud) 명령 규격입니다.

| 명령 코드 | 포맷 예시 | 설명 및 처리 로직 |
| :---: | :---: | :---- |
| `C` | `C120` | Balance PID Kp 설정 (value / 10.0 \-\> Kp \= 12.0) |
| `D` | `D105` | Balance PID Ki 설정 (value / 10.0 \-\> Ki \= 10.5) |
| `G` | `G15` | Balance PID Kd 설정 (value / 10.0 \-\> Kd \= 1.5) |
| `H` / `I` / `K` | `H25` | Rotate (Yaw) PID Kp, Ki, Kd 설정 |
| `F` | `F` | 전진 목표치 가산 (balance\_pid.setpoint \+= 1.0f) |
| `B` | `B` | 후진 목표치 감산 (balance\_pid.setpoint \-= 1.0f) |
| `L` | `L` | 좌회전 조향 가산 (rotate\_pid.setpoint \+= 5.0f) |
| `R` | `R` | 우회전 조향 감산 (rotate\_pid.setpoint \-= 5.0f) |
| `Z` | `Z` | 비상 정지 / 중립 복귀 (setpoint \= 0.0f) |
| `O` / `X` | `O` / `X` | 모터 출력 활성화(ON) / 비활성화(OFF) 토글 |
| `E` | `E` | 현재 설정된 PID 게인을 내장 EEPROM에 영구 저장 |
| `J` | `J+50-30` | 조이스틱 X/Y 좌표 기반 속도 및 조향 패킷 수신 |

---

## 📱 안드로이드 원격 제어 앱 (Android Companion App)

프로젝트 레포지토리: [balancingRobot\_app](https://github.com/jhleedev00/balancingRobot_app)

* **Bluetooth Classic SPP 연결 관리**:  
  * `ConnectedThread` 백그라운드 워커 스레드를 통해 UI 블로킹 없는 양방향 논블로킹 스트리밍 통신 구현  
* **Custom JoystickView 구현**:  
  * 터치 이벤트를 감지하여 중심점 기준 각도와 변위를 실시간 계산, 로봇의 전/후진 속도 및 좌/우 회전량을 직관적으로 조작  
* **실시간 무선 PID 튜닝 대시보드**:  
  * 로봇을 케이블에 연결하거나 펌웨어를 재컴파일할 필요 없이, 주행 중 흔들림을 관찰하며 앱 화면에서 즉각 게인 변경 및 튜닝 가능

---

## 🚀 빌드 및 설치 방법 (Getting Started)

### 1\. 펌웨어 (Microchip Studio 7.0)

1. **Microchip Studio 7.0 (구 Atmel Studio)** 설치  
2. `Balancingrobot.atsln` 솔루션 파일 열기  
3. 컴파일러 설정: AVR-GCC (Optimization Level: `-O2` 권장)  
4. 솔루션 빌드: `Build Solution (F7)`  
5. ISP 프로그래머(STK500, USBasp 등) 또는 Arduino Bootloader를 통해 `ATmega328P`에 헥사(`.hex`) 파일 플래시

### 2\. 안드로이드 애플리케이션 (Android Studio)

1. **Android Studio** 최신 버전 실행  
2. `BalancingRobot_app` 폴더 열기 (Gradle Sync 완료 대기)  
3. 스마트폰의 블루투스 설정에서 `HC-05` 모듈 페어링 (기본 PIN: `1234` 또는 `0000`)  
4. 앱 빌드 및 설치 (`Run 'app'`)  
5. 앱 상단 `Scan/Connect` 버튼을 눌러 페어링된 HC-05 선택 후 조종 시작

&nbsp;