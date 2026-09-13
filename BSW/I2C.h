/**
* @brief I2C 통신 헤더 파일
* @file I2C.h
* @author ground_rlc
* @date 2024-08-25
* @version 0.0.1
*
*/

#ifndef I2C_H_
#define I2C_H_

#include "../def.h"
#include "../sysconfig.h"

#define I2C_WRITE   0
#define I2C_READ    1

#define TW_STS      0xF8

#define TWI_SCL     0x20
#define TWI_SDA     0x10
#define TWI_RD      0x01
#define TWI_WR      0x00

#define TWI_START			0x08
#define TWI_RESTART			0x10

/* Master Transmitter */
#define TWI_MT_SLA_ACK		0x18
#define TWI_MT_SLA_NACK		0x20
#define TWI_MT_DATA_ACK		0x28
#define TWI_MT_DATA_NACK	0x30
#define TWI_MT_ARB_LOST		0x38

/* Master Receiver */
#define TWI_MR_ARB_LOST		0x38
#define TWI_MR_SLA_ACK		0x40
#define TWI_MR_SLA_NACK		0x48
#define TWI_MR_DATA_ACK		0x50
#define TWI_MR_DATA_NACK	0x58

void I2C_Init(void);
unsigned char i2c_start(unsigned char address);
unsigned char i2c_rep_start(unsigned char address);
void i2c_stop(void);
unsigned char i2c_write(unsigned char data);
unsigned char i2c_readAck(void);
unsigned char i2c_readNak(void);
uint8_t i2c_read_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data);
void i2c_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);

#endif /* TWI_H_ */