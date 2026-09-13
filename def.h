/*
 * def.h
 *
 * Created: 2026-03-17 오전 12:54:11
 *  Author: dl1wj
 */ 


#ifndef DEF_H_
#define DEF_H_

#define F_CPU 16000000UL

#define sbi(PORTX, BITX) (PORTX |= (1 << BITX))
#define cbi(PORTX, BITX) (PORTX &= ~(1 << BITX))



#endif /* DEF_H_ */