#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 1000000UL
#define LCD_DATA_PORT PORTC
#define LCD_COMMAND_PORT PORTD
#define RS_PIN 0
#define RW_PIN 1
#define E_PIN 2
typedef unsigned char U8;

void LCD_init();
void LCD_COMMAND(U8);
void LCD_DATA(U8);

// LCD initialize function
void LCD_init()
{
	DDRC = 0xFF;		// LCD data from port C
	DDRD = 0x07;		// LCD RS, RW, E from port D (PD0 --> PD2)
	_delay_ms(20);
	LCD_COMMAND(0x38);	// 8-bit 2 lines
	LCD_COMMAND(0x0E);  // Display on, cursor on 
	LCD_COMMAND(0x01);	// Clear display
	_delay_ms(2);
}

// LCD command function
void LCD_COMMAND(U8 x)
{
	LCD_COMMAND_PORT &= ~(1 << RS_PIN);
	LCD_COMMAND_PORT &= ~(1 << RW_PIN);
	LCD_DATA_PORT = x;
	LCD_COMMAND_PORT |= (1 << E_PIN);
	_delay_ms(1);
	LCD_COMMAND_PORT &= ~(1 << E_PIN);
}

// LCD data function
void LCD_DATA(U8 x)
{
	LCD_COMMAND_PORT |= (1 << RS_PIN);
	LCD_COMMAND_PORT &= ~(1 << RW_PIN);
	LCD_DATA_PORT = x;
	LCD_COMMAND_PORT |= (1 << E_PIN);
	_delay_ms(1);
	LCD_COMMAND_PORT &= ~(1 << E_PIN);
}

// LCD string input function
void LCD_DATA_STRING(const U8* x)
{
	for(char i = 0; x[i] != '\0'; i++)
	{
		LCD_DATA(x[i]);
		_delay_ms(100);
	}
}

#endif /* LCD_H_ */