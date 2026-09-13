/*
 * EEPROM.c
 *
 * Created: 2026-03-17 오후 1:54:43
 *  Author: dl1wj
 */ 
#include "EEPROM.h"

void EEPROM_Write(unsigned int uiAddress, unsigned char ucData)
{
	while(EECR & (1 << EEPE)) //EEPROM 쓰기완료 대기
	
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= (1 << EEMPE); //마스터의 EEPE 쓰기 허용
	EECR |= (1 << EEPE); //EEDR에 데이터 EEPROM에 쓰기 시작
	
}

unsigned char EEPROM_Read(unsigned int uiAdderss)
{
	while(EECR & (1 << EEPE)) //EEPROM 쓰기완료 대기
	EEAR = uiAdderss;
	EECR |= (1 << EERE); // EEPROM 읽기 시작 (EEPROM 데이터를 EEDR에 복사)
	
	return EEDR;
}