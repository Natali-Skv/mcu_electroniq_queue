#include <avr/io.h>
#include <util/delay.h>

#define SDA_PIN PB0
#define SCL_PIN PB1
#define I2C_PORT PORTB
#define I2C_DDR  DDRB
#define I2C_PIN  PINB

#define SDA_HIGH() (I2C_PORT |= (1 << SDA_PIN))  // SDA = 1
#define SDA_LOW()  (I2C_PORT &= ~(1 << SDA_PIN)) // SDA = 0
#define SCL_HIGH() (I2C_PORT |= (1 << SCL_PIN))  // SCL = 1
#define SCL_LOW()  (I2C_PORT &= ~(1 << SCL_PIN)) // SCL = 0

#define SDA_READ() ((I2C_PIN & (1 << SDA_PIN)) != 0)  // Читаем состояние SDA
#define SCL_READ() ((I2C_PIN & (1 << SCL_PIN)) != 0)  // Читаем состояние SCL
#define SDA_READ() (I2C_PIN & (1 << SDA_PIN))    // Чтение SDA

#define SLAVE_ADDRESS 0x33  // Адрес ведомого устройства (пример)

void i2c_wait_start();
uint8_t i2c_read_byte();
void i2c_send_ack(uint8_t ack);


volatile uint8_t received_data = 0;
volatile uint8_t global_data = 0;


int main() {
    // Инициализация портов I2C
    SDA_HIGH();  // Отпускаем SDA
    SCL_HIGH();  // Отпускаем SCL

    while (1) {
        // Ждём, пока мастер начнёт передачу
        i2c_wait_start();

        // Читаем адрес устройства
        uint8_t address = i2c_read_byte();

        // Проверяем, совпадает ли адрес
        if ((address >> 1) == SLAVE_ADDRESS && (address & 0x01) == 0) {  // Проверяем адрес и бит чтения/записи
            i2c_send_ack(0);  // Отправляем ACK (подтверждение)

            // Читаем данные, отправленные мастером
            received_data = i2c_read_byte();
            i2c_send_ack(0);  // Отправляем ACK после приёма данных
            received_data = i2c_read_byte();
            i2c_send_ack(0);  // Отправляем ACK после приёма данных
        } else {
            i2c_send_ack(1);  // Отправляем NACK, если адрес не совпал
        }

        // Здесь можно обработать полученные данные
        // Например, отправить их в порт или использовать для управления
    }
}

void i2c_wait_start() {
    while (!(SDA_READ() && SCL_READ())); // Ждём, пока оба SDA и SCL не станут HIGH одновременно
    
    while (SDA_READ() && SCL_READ()); // Ждём, пока SDA и SCL остаются HIGH

    if (SCL_READ() && !SDA_READ()) { // Проверяем условие START: SCL = 1 и SDA = 0
        while (SCL_READ()); // ждем, пока SCL не занулится
        return; // Сигнал START обнаружен
    } else {
        i2c_wait_start(); // Если не START, начинаем ожидание заново
    }
}

uint8_t i2c_read_byte() {
    uint8_t data = 0;

    // Читаем 8 бит данных
    uint8_t i = 0;
    for (i = 0; i < 8; i++) {
        while (!SCL_READ());  // Ждём, пока SCL станет HIGH (начало бита)

        if (SDA_READ()) {
           data |= (1 << (7-i));  // Если SDA = 1, записываем бит в младший разряд
        }

        while (SCL_READ());  // Ждём, пока SCL станет LOW (окончание бита)
    }

    return data;
}

void i2c_send_ack(uint8_t ack) {
    I2C_DDR |= (1 << SDA_PIN); // SDA как выход
    if (!ack) {
        SDA_LOW();  // Устанавливаем SDA в LOW (ACK)
    } else {
        SDA_HIGH();  // Отпускаем SDA (NACK)
    }
    _delay_ms(8);
    while(SCL_READ()); // ждем пока SCL не занулится
    I2C_DDR &= ~(1 << SDA_PIN); // SDA как вход
}