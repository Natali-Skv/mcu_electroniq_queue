#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "lcd.h"
#include "i2c.h"

#define BUTTON_PIN_MASK ((1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << PA4) | (1 << PA5))

#define F_CPU 16000000UL 
#define XTAL 3686400				 // Частота микроконтроллера
#define FLED 1						 // Частота мигания
#define QUEUE_CNT 6					 // Количество очередей
#define MAX_CLIENTS 9				 // Максимальное количество клиентов
#define INTERVALS {1, 2, 3, 5, 5, 7} // Интервалы времени для очередей
#define BEGIN 0
#define END 1
#define DIR 2
#define FROM_LEAST_TO_GREATEST 0
#define FROM_GREATEST_TO_LEAST 1

unsigned char queue_lengths[QUEUE_CNT] = {0};
const unsigned char intervals[QUEUE_CNT] = INTERVALS;

unsigned char last_call_time[QUEUE_CNT] = {0};
// char queues[QUEUE_CNT][3] = {
// 	{1, 8, FROM_LEAST_TO_GREATEST}, // от 1 до 8 вкл
// 	{1, 8, FROM_LEAST_TO_GREATEST},
// 	{1, 8, FROM_LEAST_TO_GREATEST},
// 	{1, 8, FROM_LEAST_TO_GREATEST},
// 	{1, 8, FROM_LEAST_TO_GREATEST},
// 	{1, 8, FROM_LEAST_TO_GREATEST},
// };

char queues[QUEUE_CNT][3] = {
	{0, 0, FROM_LEAST_TO_GREATEST}, // от 1 до 8 вкл
	{0, 0, FROM_LEAST_TO_GREATEST},
	{0, 0, FROM_LEAST_TO_GREATEST},
	{0, 0, FROM_LEAST_TO_GREATEST},
	{0, 0, FROM_LEAST_TO_GREATEST},
	{0, 0, FROM_LEAST_TO_GREATEST},
};

LCD_t lcd_queue = {&PORTB, &DDRB, PB0, PB1};
LCD_t lcd_new_client = {&PORTC, &DDRC, PC0, PC1};
// LCD_t lcd_queue = {};

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 0x10000 - (XTAL / 1024 / FLED); // Сброс таймера

	unsigned char i = 0;
	for (i = 0; i < QUEUE_CNT; i++)
	{
		last_call_time[i]++;
		if (last_call_time[i] < intervals[i])
			continue;

		if (queues[i][DIR] == FROM_LEAST_TO_GREATEST && queues[i][END] > queues[i][BEGIN] ||
			queues[i][DIR] == FROM_GREATEST_TO_LEAST && queues[i][END] < queues[i][BEGIN])
		{
			last_call_time[i] = 0;
			queues[i][BEGIN] = queues[i][BEGIN] % MAX_CLIENTS + 1;
			if (queues[i][BEGIN] == 1)
			{
				queues[i][DIR] = FROM_LEAST_TO_GREATEST;
			}

		i2c_start();            // Генерация стартового сигнала
		if (!i2c_write(0x33 << 1)) {
			if (!i2c_write(i)) {
        		i2c_write(queues[i][BEGIN]);      // Передача числа
			}      // Передача числа
		};   // Адрес ведомого устройства (пример: 0x20, бит записи = 0)
        i2c_stop();             // Генерация стоп-сигнала

		}
		else // TODO мб не нужно? мб оно само
		{
			queues[i][BEGIN] = 0;
			queues[i][END] = 0; // никого нет в очереди
			queues[i][DIR] = FROM_LEAST_TO_GREATEST;
		}
	}

	char queue_display[32] = "";
	snprintf(queue_display, sizeof(queue_display), "| %2d | %2d | %2d | %2d | %2d | %2d |", queues[0][BEGIN], queues[1][BEGIN], queues[2][BEGIN], queues[3][BEGIN], queues[4][BEGIN], queues[5][BEGIN]);

	LCD_Command(lcd_queue, 0xC0);
	LCD_String(lcd_queue, queue_display);
}

int main(void)
{
	LCD_Init(lcd_queue);
	LCD_Init(lcd_new_client);

	LCD_String(lcd_queue, "|  1 |  2 |  3 |  4 |  5 |  6  |");

    i2c_init(); // Инициализация I2C

	// Настройка Timer1
	TCCR1A = 0;
	TCCR1B = 5; // Предделитель 1024
	TCNT1 = 0x10000 - (XTAL / 1024 / FLED);
	TIFR = 0;
	TIMSK = 0x80; // Разрешение прерывания
	GICR = 0;

	// Настройка кнопок как входов с подтяжкой
	DDRA &= ~BUTTON_PIN_MASK; // PA0-PA5 как входы
	PORTA |= BUTTON_PIN_MASK; // Включить подтягивающие резисторы

	sei(); // Глобальное разрешение прерываний

	uint8_t last_state = PINA & BUTTON_PIN_MASK; // Сохраняем начальное состояние кнопок

	while (1)
	{
		uint8_t current_state = PINA & BUTTON_PIN_MASK;

		uint8_t i = 0;
		for (i = 0; i < 6; i++)
		{
			uint8_t mask = (1 << i);
			if (!(last_state & mask) || (current_state & mask))
				continue;
			LCD_Clear(lcd_new_client);
			if (abs(queues[i][END] - queues[i][BEGIN]) >= 2 || queues[i][DIR] == FROM_LEAST_TO_GREATEST)
			{
				queues[i][END] = queues[i][END] % MAX_CLIENTS + 1; // TODO сделать зацикливание из 99 в 0, но не переходя за begin
				if (queues[i][END] == 1 && queues[i][BEGIN] != 0)
				{
					queues[i][DIR] = FROM_GREATEST_TO_LEAST;
				}

				char new_client[21] = "";
				snprintf(new_client, sizeof(new_client), "queue %1d: %2d", i + 1, queues[i][END]);
				LCD_String(lcd_new_client, new_client);
			}
			else
			{
				LCD_String(lcd_new_client, "no places");
			}
		}

		last_state = current_state; // Обновляем состояние кнопок

		_delay_ms(50); // Задержка для устранения дребезга кнопки
	}
}

// TODO при переходе за границу зацикливания очередь обнуляется ??? надо исправить
// при отсутствии мест 1 до 3   1 2 3 1-9