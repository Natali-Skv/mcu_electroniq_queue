#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

 
// int main(void) {
//     uint8_t number = 0x42; // Число для отправки

//     i2c_init(); // Инициализация I2C

//     while (1) {
//         i2c_start();            // Генерация стартового сигнала
//         ack = i2c_write(0x33 << 1);   // Адрес ведомого устройства (пример: 0x20, бит записи = 0)
//         ack = i2c_write(number);      // Передача числа
//         ack = i2c_write(0x44);      // Передача числа
//         ack = i2c_write(0x45);      // Передача числа
//         i2c_stop();             // Генерация стоп-сигнала

//         _delay_ms(1000);        // Задержка 1 секунда
//     }
// }

void i2c_init(void) {
    // Настройка линий SDA и SCL как выходы
    I2C_DDR |= (1 << SDA_PIN) | (1 << SCL_PIN);
    SDA_HIGH(); // SDA = 1
    SCL_HIGH(); // SCL = 1
}

void i2c_start(void) {
    // Генерация стартового сигнала: SDA = 0 при SCL = 1
    SDA_HIGH();
    SCL_HIGH();
    _delay_ms(5);  // Небольшая задержка
    SDA_LOW();
    _delay_ms(5);
    SCL_LOW();
}

void i2c_stop(void) {
    // Генерация стоп-сигнала: SDA = 1 при SCL = 1
    SDA_LOW();
    SCL_HIGH();
    _delay_ms(5);
    SDA_HIGH();
    _delay_ms(5);
}

uint8_t i2c_write(uint8_t data) {
    // Передача одного байта данных
    uint8_t i = 0;
    for (i = 0; i < 8; i++) {
        if (data & 0x80) { // Проверка старшего бита
            SDA_HIGH();
        } else {
            SDA_LOW();
        }
        _delay_ms(5); // Задержка
        SCL_HIGH();   // Импульс тактирования
        _delay_ms(5);
        SCL_LOW();
        data <<= 1;   // Сдвиг данных влево
    }



    // Чтение подтверждения ACK от ведомого устройства
    SDA_HIGH();           // Освобождаем SDA для приёма ACK

    I2C_DDR &= ~(1 << SDA_PIN); // SDA как вход
    _delay_ms(5);
    SCL_HIGH();           // Импульс тактирования для ACK
    _delay_ms(5);
    uint8_t ack = SDA_READ(); // Чтение ACK если 0 -- было подтверждение
    SCL_LOW();
    I2C_DDR |= (1 << SDA_PIN); // SDA обратно как выход
    return ack; // Возвращаем ACK (0 = подтверждение, 1 = нет подтверждения)
}
