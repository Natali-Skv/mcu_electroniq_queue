#include <avr/io.h>
#include <util/delay.h>

#ifndef LCD1602_H_
#define LCD1602_H_
#define LCD_Dir DDRB
#define LCD_PORT PORTB
#define RS PB0
#define EN PB1

typedef typeof(&PORTB) port_t;
typedef typeof(PB0) pin_t;

typedef struct
{
	port_t port;
	port_t dir;
	pin_t rs;
	pin_t en;
} LCD_t;

void LCD_Command(LCD_t lcd, unsigned char cmnd);
void LCD_Char(LCD_t lcd, unsigned char data);
void LCD_Init(LCD_t lcd);
void LCD_String(LCD_t lcd, char *str);
void LCD_String_xy(LCD_t lcd, char row, char pos, char *str);
void LCD_Clear(LCD_t lcd);

#endif /* LCD1602_H_ */