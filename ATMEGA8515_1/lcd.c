#include "lcd.h"

void LCD_Command(LCD_t lcd, unsigned char cmnd)
{
	*lcd.port = (*lcd.port & 0x0F) | (cmnd & 0xF0);
	*lcd.port &= ~(1 << lcd.rs);
	*lcd.port |= (1 << lcd.en);
	_delay_us(1);
	*lcd.port &= ~(1 << lcd.en);

	_delay_us(200);

	*lcd.port = (*lcd.port & 0x0F) | (cmnd << 4);
	*lcd.port |= (1 << lcd.en);
	_delay_us(1);
	*lcd.port &= ~(1 << lcd.en);
	_delay_ms(2);
}

void LCD_Char(LCD_t lcd, unsigned char data)
{
	*lcd.port = (*lcd.port & 0x0F) | (data & 0xF0);
	*lcd.port |= (1 << lcd.rs);
	*lcd.port |= (1 << lcd.en);
	_delay_us(1);
	*lcd.port &= ~(1 << lcd.en);

	_delay_us(200);

	*lcd.port = (*lcd.port & 0x0F) | (data << 4);
	*lcd.port |= (1 << lcd.en);
	_delay_us(1);
	*lcd.port &= ~(1 << lcd.en);
	_delay_ms(2);
}

void LCD_Init(LCD_t lcd)
{
	*lcd.dir = 0xFF;
	_delay_ms(20);

	LCD_Command(lcd, 0x02);
	LCD_Command(lcd, 0x28);
	LCD_Command(lcd, 0x0c);
	LCD_Command(lcd, 0x06);
	LCD_Command(lcd, 0x01);
	_delay_ms(3);
}

void LCD_String(LCD_t lcd, char *str)
{
	int i;
	for (i = 0; str[i] != 0; i++)
	{
		LCD_Char(lcd, str[i]);
	}
}

void LCD_String_xy(LCD_t lcd, char row, char pos, char *str)
{
	if (row == 0 && pos < 16)
		LCD_Command(lcd, (pos & 0x0F) | 0x80);
	else if (row == 1 && pos < 16)
		LCD_Command(lcd, (pos & 0x0F) | 0xC0);
	LCD_String(lcd, str);
}

void LCD_Clear(LCD_t lcd)
{
	LCD_Command(lcd, 0x01);
	_delay_ms(2);
	LCD_Command(lcd, 0x80);
}