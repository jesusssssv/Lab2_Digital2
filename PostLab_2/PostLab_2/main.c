#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ADC.h"
#include "LCD_8bits.h"
#include "UART.h"

// ---------------------------
// FUNCIONES DE CONVERSIÓN
// ---------------------------
float adc_to_voltage(uint16_t value) {
	return (value * 5.0) / 1023.0;
}

void float_to_str(float value, char *str) {
	dtostrf(value, 4, 2, str);
}

void int_to_str(int value, char *str) {
	itoa(value, str, 10);
}

// ---------------------------
// PROGRAMA PRINCIPAL
// ---------------------------
int main(void) {
	// Desactivar UART en PD0/PD1 (modo general)
	DDRD &= ~((1 << PD0) | (1 << PD1));
	PORTD &= ~((1 << PD0) | (1 << PD1));

	lcd_init();       // Inicializa el LCD
	ADC_init();       // Inicializa el ADC
	UART_init(9600);  // UART a 9600 baudios

	char buffer1[8], buffer2[8], buffer3[8];
	int contador_S3 = 0;

	// Mostrar etiquetas estáticas
	lcd_set_cursor(0, 0);
	lcd_print(" S1:   S2:  S3:");

	UART_sendString("Sistema iniciado.\r\n");

	while (1) {
		// Leer voltajes de potenciómetros
		float v1 = adc_to_voltage(ADC_read(6)); // A6
		float v2 = adc_to_voltage(ADC_read(7)); // A7

		float_to_str(v1, buffer1);
		float_to_str(v2, buffer2);
		int_to_str(contador_S3, buffer3);

		// Mostrar en LCD
		lcd_set_cursor(1, 0);
		lcd_print(buffer1);
		lcd_print("V ");

		lcd_set_cursor(1, 6);
		lcd_print(buffer2);
		lcd_print("V ");

		lcd_set_cursor(1, 12);
		lcd_print("  ");         // Borrar anterior
		lcd_set_cursor(1, 13);
		lcd_print(buffer3);     // Mostrar contador

		// Mostrar en consola
		UART_sendString("S1: ");
		UART_sendString(buffer1);
		UART_sendString(" V | S2: ");
		UART_sendString(buffer2);
		UART_sendString(" V | S3: ");
		UART_sendString(buffer3);
		UART_sendString("\r\n");

		// Revisa si se recibió un carácter por UART
		if (UCSR0A & (1 << RXC0)) {
			char comando = UART_readChar();
			if (comando == '+') {
				contador_S3++;
				} else if (comando == '-') {
				contador_S3--;
			}
		}

		_delay_ms(300); // Actualización con retardo
	}
}


