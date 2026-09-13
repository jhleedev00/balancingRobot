/*
 * UART.h
 *
 * Created: 2026-03-17 오전 9:55:24
 *  Author: dl1wj
 */ 


#ifndef UART_H_
#define UART_H_

#include "../def.h"
#include "../sysconfig.h"

#define BAUD_9600 9600
#define BAUD_38400 38400
#define BAUD_57600 57600

#define USE_2X 1

#if USE_2X
#define UBRR_VALUE ((F_CPU/ (BAUD * 8UL)) - 1)
#else
#define UBRR_VALUE ((F_CPU/ (BAUD * 16UL)) - 1)
#endif

extern char response[100];
extern uint8_t uart_rx_flag;

void UART_init(unsigned long baud);
int UART_putchar(char c, FILE *stream);
void UART_transmit(unsigned char data);
unsigned char UART_Receive();
void UART_send_string(const char *str);

#endif /* UART_H_ */