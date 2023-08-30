/*
 * LCD.h
 *
 * Created: 8/1/2023 6:41:29 PM
 *  Author: Mayank Bankoti
 */ 


#ifndef LCD_H_
#define LCD_H_
//#define F_CPU 16000000UL
#include "util/delay.h"

#define ctrl PORTB
#define RS 2
#define RW 1
#define EN 0

#define Dataout PORTA

void LCD_cmd(unsigned char Command);
void LCD_data(unsigned char Data);
void LCD_init(void);

void LCD_init(void)
{
	LCD_cmd(0x38); // 8bit mode
	LCD_cmd(0x06); // Entry mode
	LCD_cmd(0x01);  // Clear Display
	LCD_cmd(0x80); // 1st row and 1st position
	LCD_cmd(0x0E); // Display on cursor on
}

void LCD_cmd(unsigned char Command)
{
	Dataout = Command;
	ctrl = (0<<RS)|(0<<RW)|(1<<EN); // RS=0 for instruction/command 
	_delay_ms(1);
	ctrl = (0<<RS)| (0<< RW)|(0<<EN);
	_delay_ms(50);
}

void LCD_data(unsigned char Data)
{
	Dataout = Data;
	ctrl = (1<<RS)| (0<<RW)| (1<<EN);  // RS=1 for data
	_delay_ms(1);
	ctrl = (1<<RS | (0<<RW)|(0<<EN));
	_delay_ms(50);
}



#endif /* LCD_H_ */