/*
 * HC05.c
 *
 * Created: 2026-03-19 오전 12:17:06
 *  Author: dl1wj
 */

#include "HC05.h"

void hc05_init(){
	DDRB |= (1 << DDB4); //EN으로 사용
	PORTB &= ~(1 << PORTB4);// 기본값 0
}

void hc05_enter_at_mode(){
	PORTB |= (1 << PORTB4);
	_delay_ms(100);
}

void hc05_exit_at_mode(){
	PORTB &= ~(1 << PORTB4);
	_delay_ms(100);
}

int hc05_send_at_command(const char *cmd, char *response, uint8_t max_length)
{
	UART_send_string(cmd);
	UART_transmit('\r');
	UART_transmit('\n');
	
	uint8_t i = 0;
	uint16_t timeout = 10000;
	
	while(i < max_length - 1 && timeout > 0)
	{
		if(UCSR0A & (1 << RXC0))
		{
			response[i] = UART_Receive();
			if(response[i] == '\r') continue;
			if(response[i] == '\n') break;
			i++;
		}
		_delay_ms(10);
		timeout--;
	}
	
	response[i] = '\0';
	return i > 0 ? 1 : 0; // i가 0보다 크면 1 true 그렇지 않으면 0 false
}

int hc05_change_baud_rate(unsigned int baud)
{
	char command[32];
	char response[128];
	
	snprintf(command, sizeof(command), "AT+UART=%u,0,0", baud);
	return hc05_send_at_command(command,response, sizeof(response));
}

int hc05_check_connection(){
	char response[128];
	return hc05_send_at_command("AT", response, sizeof(response));
}

int hc05_get_version_info(char *response, uint8_t max_length)
{
	return hc05_send_at_command("AT+VERSION", response, sizeof(response));
}

void AT_Cmd()
{
	hc05_init();
	hc05_enter_at_mode();
	_delay_ms(500);
	
	char response[256];
	
	if(hc05_check_connection())
	{
		UART_send_string("AT command Successful\n");
	}
	else
	{
		UART_send_string("AT command Failed\n");
	}
	
	if(hc05_get_version_info(response, (uint8_t)sizeof(response)))
	{
		UART_send_string("Version Info: ");
		UART_send_string(response);
		UART_send_string("\n");
	}
	else
	{
		UART_send_string("Failed to retrieve version info\n");
	}
	
	if(hc05_change_baud_rate(57600))
	{
		UART_send_string("Baud rate changed to 57600\n");
		UART_init(57600);
	}
	else
	{
		UART_send_string("Failed to change baud rate\n");
	}
	
	hc05_exit_at_mode();
}

int hc05_factory_reset()
{
	char response[128];
	
	// 1. AT 모드 진입 확인 (응답이 없으면 시작 불가)
	if (!hc05_send_at_command("AT", response, sizeof(response))) {
		return 0;
	}

	// 2. 공장 초기화 명령 (Original 상태로 복구)
	// 성공 시 이름: HC-05, 비번: 1234, 속도: 9600으로 변경됨
	if (hc05_send_at_command("AT+ORGL", response, sizeof(response))) {
		return 1;
	}

	return 0;
}