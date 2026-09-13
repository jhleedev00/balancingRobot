/*
 * HC05.h
 *
 * Created: 2026-03-19 오전 12:17:42
 *  Author: dl1wj
 */ 


#ifndef HC05_H_
#define HC05_H_

#include "../def.h"
#include "../sysconfig.h"

extern void hc05_init();
extern void hc05_enter_at_mode();
extern void hc05_exit_at_mode();
extern int hc05_send_at_command(const char *cmd, char *response, uint8_t max_length);

extern int hc05_change_baud_rate(unsigned int baud);
extern int hc05_check_connection();
extern int hc05_get_version_info(char *response, uint8_t max_length);
int hc05_factory_reset();

extern void AT_Cmd();




#endif /* HC05_H_ */