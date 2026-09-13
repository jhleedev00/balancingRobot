/*
 * UART.c
 *
 * Created: 2026-03-17 오전 9:55:07
 *  Author: dl1wj
 */ 

#include "UART.h"

// FILE 스트림 생성 및 연결
static FILE uart_stdout = FDEV_SETUP_STREAM(UART_putchar, NULL, _FDEV_SETUP_WRITE);

volatile char received_char;
char response[100];
uint8_t idx = 0;

uint8_t uart_rx_flag = 0;

void UART_init(uint32_t baud) {
	uint16_t ubrr = (F_CPU / (16 * baud)) - 1;
	
	if(USE_2X)
	{
		sbi(UCSR0A, U2X0);
		ubrr = (F_CPU/ 8 / baud) - 1;
	}
	else
	{
		cbi(UCSR0A, U2X0);
	}

	// 보드레이트 설정 (UBRR0H, UBRR0L)
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	// 송수신 가능 설정 (RX, TX Enable)
	sbi(UCSR0B,RXEN0);
	sbi(UCSR0B,TXEN0);
	//수신 인터럽트 활성화
	sbi(UCSR0B,RXCIE0);
	

	// 프레임 포맷 설정: 비동기, 패리티 없음, 1스탑 비트, 8비트 데이터
	sbi(UCSR0C, UCSZ01);
	sbi(UCSR0C, UCSZ00);
	
	stdout = &uart_stdout; //printf로 사용설정
	sei(); //전역 인터럽트 활성화
}

// 문자 하나를 UART로 보내는 함수
int UART_putchar(char c, FILE *stream) {
	if (c == '\n') { // 줄바꿈 문자 처리 (Optional)
		UART_putchar('\r', stream);
	}
	// 송신 버퍼가 비어있을 때까지 대기
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
	return 0;
}

ISR(USART_RX_vect)
{
	received_char = UDR0; // 수신 받은 데이터 가져오기
	response[idx++] = received_char; 
	if(received_char == '\n')
	{
		//parse_bluetooth_message(response);
		uart_rx_flag = 1;
		idx = 0;
	}
}

void UART_transmit(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0))); //수신 버퍼 비였음 확인
	UDR0 = data;
}

unsigned char UART_Receive()
{
	while(!(UCSR0A & (1 << UDRE0))); //송신 버퍼 비였음 확인
	return UDR0;
}

void UART_send_string(const char *str){
	while(*str){
		UART_transmit(*str++);
	}
}