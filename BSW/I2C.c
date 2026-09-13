/**
* @brief I2C 통신 Init
* @file I2C.c
* @author ground_rlc
* @date 2024-08-25
* @version 0.0.1
*
*/

#include "I2C.h"

void I2C_Init(void)
{
	/* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
	TWSR = 0x00;                         /* no prescaler */
	TWBR = 12;  /* ((F_CPU / SCL_CLOCK) - 16) / 2 = TWBR ,  F_CPU = 4Mhz, SCL_CLOCK = 100K  --> 12*/ 
}/* i2c_init */

unsigned char i2c_start(unsigned char address)
{
    uint8_t   twst;

    // send START condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    // wait until transmission completed
    while(!(TWCR & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TWSR & 0xF8;
    if ( (twst != TWI_START) && (twst != TWI_RESTART)) return 1;

    // send device address
    TWDR = address;
    TWCR = (1<<TWINT) | (1<<TWEN);

    // wail until transmission completed and ACK/NACK has been received
    while(!(TWCR & (1<<TWINT)));

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TWSR & 0xF8;
    if ( (twst != TWI_MT_SLA_ACK) && (twst != TWI_MR_SLA_NACK) ) return 1;

    return 0;	
}

unsigned char i2c_rep_start(unsigned char address)
{
	return i2c_start( address );
}/* i2c_rep_start */

void i2c_stop(void)
{
	/* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}/* i2c_stop */

unsigned char i2c_write( unsigned char data )
{
	uint8_t   twst;
	
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TWSR & 0xF8;
	if( twst != TWI_MT_DATA_ACK) return 1;
	return 0;

}/* i2c_write */

/*************************************************************************
 Read one byte from the I2C device, request more data from device 
 
 Return:  byte read from I2C device
*************************************************************************/

unsigned char i2c_readAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    

    return TWDR;

}/* i2c_readAck */

/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;

}/* i2c_readNak */

// read one byte from dev, stored in value, return 1 for error
uint8_t i2c_read_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t* data)
{
	i2c_start((dev_addr<<1) | I2C_WRITE); 	//start i2c to write register address
	i2c_write(reg_addr);			//write address of register to read
	i2c_rep_start((dev_addr<<1) | I2C_READ);	//restart i2c to start reading
	*data = i2c_readNak();
	i2c_stop();
	return 0;
}

void i2c_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
	i2c_start((dev_addr<<1) | I2C_WRITE);
	i2c_write(reg_addr);
	i2c_write(data);
	i2c_stop();
}