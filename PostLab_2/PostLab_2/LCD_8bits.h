/*
 * LCD_8bits.h
 *
 * Created: 20/07/2025 11:32:11
 *  Author: valen
 */ 


#ifndef LCD_8BITS_H_
#define LCD_8BITS_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// Envía byte a PD2-PD7 y PB0-PB1
void lcd_write_pins(uint8_t value);
// Funciones LCD
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_init(void);
void lcd_set_cursor(uint8_t row, uint8_t col); 
void lcd_print(char *str);





#endif /* LCD_8BITS_H_ */