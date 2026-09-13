/*
 * Bt_Com.h
 *
 * Created: 2026-03-27 오후 8:20:48
 *  Author: dl1wj
 */ 


#ifndef BT_COM_H_
#define BT_COM_H_

#include "../def.h"
#include "../sysconfig.h"

extern char response[100];

void parse_bluetooth_message(const char *btMessage);

#endif /* BT_COM_H_ */