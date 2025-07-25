/*
 * UART.h
 *
 * Created: 20/07/2025 12:03:30
 *  Author: valen
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void UART_init(unsigned int baud);
void UART_sendChar(char data);
void UART_sendString(const char* str);
char UART_readChar(void);




#endif /* UART_H_ */