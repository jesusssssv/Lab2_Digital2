/*
 * LCD_8bits.c
 *
 * Created: 20/07/2025 11:29:51
 *  Author: valen
 */ 

// Define la frecuencia del CPU en Hz (16MHz) para los delays
#define F_CPU 16000000UL

// Incluye las librerías necesarias para el manejo de I/O y delays
#include <avr/io.h>        // Para el manejo de registros de entrada/salida
#include <util/delay.h>    // Para las funciones de delay (_delay_ms, _delay_us)
#include <stdlib.h>        // Librería estándar de C

// Define los pines de control del LCD
#define RS PB2  // Pin de selección de registro (Register Select) en PB2
#define EN PB3  // Pin de habilitación (Enable) en PB3

// Macros para controlar el pin RS del LCD
#define LCD_RS_HIGH() PORTB |= (1 << RS)   // Pone RS en alto (modo datos)
#define LCD_RS_LOW()  PORTB &= ~(1 << RS)  // Pone RS en bajo (modo comandos)

// Macros para controlar el pin EN del LCD
#define LCD_EN_HIGH() PORTB |= (1 << EN)   // Pone EN en alto (habilita LCD)
#define LCD_EN_LOW()  PORTB &= ~(1 << EN)  // Pone EN en bajo (deshabilita LCD)

// Función para enviar un byte completo al LCD usando 8 pines
// Distribuye el byte en PD2-PD7 (6 bits) y PB0-PB1 (2 bits)
void lcd_write_pins(uint8_t value) {
	// Envía los bits 0-5 del valor a los pines PD2-PD7
	PORTD = (PORTD & 0x03) | (value << 2); // Mantiene PD0 y PD1 intactos, desplaza valor 2 posiciones
	
	// Envía los bits 6-7 del valor a los pines PB0-PB1  
	PORTB = (PORTB & 0xFC) | (value >> 6); // Mantiene PB2-PB7 intactos, toma bits superiores
}

// Función para enviar un comando al LCD
void lcd_command(uint8_t cmd) {
	LCD_RS_LOW();        // Selecciona modo comando (RS = 0)
	lcd_write_pins(cmd); // Envía el comando a los pines de datos
	LCD_EN_HIGH();       // Activa el pulso de habilitación
	_delay_us(1);        // Espera 1 microsegundo (tiempo mínimo de pulso)
	LCD_EN_LOW();        // Desactiva el pulso de habilitación
	_delay_ms(2);        // Espera 2ms para que el LCD procese el comando
}

// Función para enviar un dato (carácter) al LCD
void lcd_data(uint8_t data) {
	LCD_RS_HIGH();        // Selecciona modo datos (RS = 1)
	lcd_write_pins(data); // Envía el dato a los pines de datos
	LCD_EN_HIGH();        // Activa el pulso de habilitación
	_delay_us(1);         // Espera 1 microsegundo (tiempo mínimo de pulso)
	LCD_EN_LOW();         // Desactiva el pulso de habilitación
	_delay_ms(2);         // Espera 2ms para que el LCD procese el dato
}

// Función de inicialización del LCD
void lcd_init(void) {
	// Configura los pines PD2-PD7 como salidas para datos del LCD
	DDRD |= 0b11111100; // Bits 7-2 como salida, bits 1-0 como entrada
	
	// Configura los pines PB0-PB3 como salidas
	// PB0-PB1 para datos, PB2 para RS, PB3 para EN
	DDRB |= 0b00001111; // Bits 3-0 como salida, bits 7-4 permanecen sin cambio
	
	_delay_ms(20);       // Espera inicial de 20ms después del encendido
	
	lcd_command(0x38);   // Configura LCD: modo 8 bits, 2 líneas, matriz 5x8
	lcd_command(0x0C);   // Enciende display, cursor OFF, parpadeo OFF
	lcd_command(0x01);   // Limpia la pantalla y regresa cursor a inicio
	_delay_ms(2);        // Espera adicional después del clear
	lcd_command(0x06);   // Modo de entrada: cursor se mueve a la derecha, no shift
}

// Función para posicionar el cursor en una fila y columna específica
void lcd_set_cursor(uint8_t row, uint8_t col) {
	// Calcula la dirección de memoria según la fila
	uint8_t addr = (row == 0) ? 0x00 : 0x40; // Fila 0 inicia en 0x00, fila 1 en 0x40
	
	// Envía comando de posicionamiento (bit 7 = 1, bits 6-0 = dirección)
	lcd_command(0x80 | (addr + col)); // 0x80 es el comando Set DDRAM Address
}

// Función para imprimir una cadena de caracteres en el LCD
void lcd_print(char *str) {
	// Itera a través de cada carácter de la cadena hasta encontrar el terminador '\0'
	while (*str) {
		lcd_data(*str++); // Envía el carácter actual y avanza al siguiente
	}
}