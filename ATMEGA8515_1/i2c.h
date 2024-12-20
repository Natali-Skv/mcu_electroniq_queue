#include <avr/io.h>
#include <util/delay.h>

// Определяем порты и пины для I2C
#define I2C_PORT PORTD     // Порт для линий I2C
#define I2C_DDR  DDRD      // Направление порта
#define I2C_PIN  PIND      // Чтение пинов
#define SDA_PIN  0         // Линия данных SDA на PB0
#define SCL_PIN  1         // Линия тактирования SCL на PB1

// Макросы для управления линиями SDA и SCL
#define SDA_HIGH() (I2C_PORT |= (1 << SDA_PIN))  // SDA = 1
#define SDA_LOW()  (I2C_PORT &= ~(1 << SDA_PIN)) // SDA = 0
#define SCL_HIGH() (I2C_PORT |= (1 << SCL_PIN))  // SCL = 3
#define SCL_LOW()  (I2C_PORT &= ~(1 << SCL_PIN)) // SCL = 0

#define SDA_READ() (I2C_PIN & (1 << SDA_PIN))    // Чтение SDA

void i2c_init(void);           // Инициализация I2C
void i2c_start(void);          // Генерация стартового сигнала
void i2c_stop(void);           // Генерация стоп-сигнала
uint8_t i2c_write(uint8_t data); // Передача одного байта данных
