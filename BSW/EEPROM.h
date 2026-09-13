/*
 * EEPROM.h
 *
 * Created: 2026-03-17 오후 1:55:02
 *  Author: dl1wj
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include "../def.h"
#include "../sysconfig.h"

#define BP_ADDRESS 0x00 //Balancing PID
#define BI_ADDRESS 0x04
#define BD_ADDRESS 0x08
#define RP_ADDRESS 0x0C //Rotation PID
#define RI_ADDRESS 0x10
#define RD_ADDRESS 0x14
#define VP_ADDRESS 0x18 //Velocity PID
#define VI_ADDRESS 0x1C
#define VD_ADDRESS 0x20

enum EEPROM
{
	EEPROM_NONE = 0,
	EEPROM_SAVE = 1
};

extern void EEPROM_Write(unsigned int uiAddress, unsigned char ucData);
extern unsigned char EEPROM_Read(unsigned int uiAdderss);


#endif /* EEPROM_H_ */