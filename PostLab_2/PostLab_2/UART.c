/*
 * UART.c
 *
 * Created: 20/07/2025 12:01:45
 *  Author: valen
 */ 

// ---------------------------
// UART CONFIGURACIÓN
// ---------------------------
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void UART_init(unsigned int baud) {
	uint16_t ubrr = F_CPU / 16 / baud - 1;
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Habilita Rx y Tx
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos, sin paridad
}

void UART_sendChar(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Espera a que el buffer esté vacío
	UDR0 = data;
}

void UART_sendString(const char* str) {
	while (*str) {
		UART_sendChar(*str++);
	}
}

char UART_readChar(void) {
	while (!(UCSR0A & (1 << RXC0))); // Espera a que haya datos
	return UDR0;
}