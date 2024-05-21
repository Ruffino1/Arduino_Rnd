#include <Wire.h>

#define INA219_ADDRESS (0x40)  // Адрес INA219 на шине I2C
// основные переменные
#define SERVO_PIN 5               // Пин, к которому подключен сервопривод
#define MIN_PULSE_WIDTH 600       // Минимальная длительность импульса для 0 градусов
#define MAX_PULSE_WIDTH 2400      // Максимальная длительность импульса для 180 градусов
#define NEUTRAL_PULSE_WIDTH 1500  // длительность импульса для 90 градусов
#define ACCELERATION_TIME 3770    // время работы серво
#define MAX_AM 750                // максимальная нагрузка
#define MIN_AM 63                 // минимальная нагрузка
#define speed 20                  // скорость

//таймер
unsigned long startTime;  // начало основного цикла

bool button_flag = true;  // нажатие кнопки

float current_mA = 0;
float current_old_mA = 0;                                                 // нагрузка
int ACCELERATION_STEP_braking = 900 / ((ACCELERATION_TIME / 2) / speed);  // Шаг изменения скорости поворота сервопривода замеднение
int ACCELERATION_STEP_racing = 900 / ((ACCELERATION_TIME / 4) / speed);   // Шаг изменения скорости поворота сервопривода ускорение
int AM_STEP_braking = MAX_AM / ((ACCELERATION_TIME / 2) / speed);         // Шаг изменения максимальной нагрузки замедление
int AM_STEP_racing = MAX_AM / ((ACCELERATION_TIME / 4) / speed);          // Шаг изменения максимальной нагрузки ускорение



// Данные с INA219
int16_t readRegister(uint8_t reg) {
  Wire.beginTransmission(INA219_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom((int)INA219_ADDRESS, (int)2);
  return (Wire.read() << 8) | Wire.read();
}

// Функция для получения силы тока в мА
float getCurrent_mA() {
  int16_t raw_current = readRegister(0x04);  // Регистр сырых данных о токе
  return raw_current * 0.1;                  // Коэффициент преобразования для получения значения в мА
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  //Serial.println("Hello!");
  attachInterrupt(0, isr, FALLING);
  // Инициализация сервопривода
  pinMode(SERVO_PIN, OUTPUT);
}

// Если прерывание
void isr() {
  button_flag = false;  // кнопка нажата
  stopServo();          // остановить  серво
}

void loop() {

  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case '1':
        rotateServoWithAcceleration(MAX_PULSE_WIDTH);  // закрыть
        break;
      case '2':
        rotateServoWithAcceleration(MIN_PULSE_WIDTH);  // открыть
        break;
    }
  }
}

// основной цикл работы серво
void rotateServoWithAcceleration(int inc) {
  button_flag = true;                                                  // кнопка не нажата
  Serial.println();                                                    // очистить буфер
  startTime = millis();                                                // начало основного таймера для серво
  int step_speed = 1500;                                               // шаг ускорения и замедления
  int step_am = -AM_STEP_racing + MIN_AM;                              // шаг максимальной и минимальной нагрузки
  current_mA = 0;                                                      // обнулить значение нагрузки
  if (inc >= 1500) {                                                   // закрыть
    while (millis() - startTime < ACCELERATION_TIME && button_flag) {  // пока таймер и кнопка не нажата

      if (millis() - startTime < ACCELERATION_TIME / 4) {  // если не прошло больше 1/4 тогда ускоряемся
        if (step_speed < MAX_PULSE_WIDTH) {                // и если скорость не максимальная
          step_speed += ACCELERATION_STEP_racing;          // ускорение
        }
        if (step_am < MAX_AM) {       // максимальная нагрузка пропорциональна ускорению
          step_am += AM_STEP_racing;  // увеличиваем нагрузку
          //Serial.println(AM_STEP_racing);
        }
      } else if (millis() - startTime > ACCELERATION_TIME / 2) {  // если не прошло больше 1/2 тогда замедляемся
        if (step_speed > NEUTRAL_PULSE_WIDTH) {                   //и если скороть не минимальная
          step_speed -= ACCELERATION_STEP_braking;                // замедление
        }
        if (step_am > MIN_AM) {        // минимальная нагрузка пропорциональна ускорению
          step_am -= AM_STEP_braking;  // уменьшаем нагрузку
        }
      }



      /*
      // график нагрузки 
      Serial.print(step_am);
      Serial.print(',');
      Serial.print(current_mA);
      Serial.println();
      */

      if (current_mA > step_am) {  // если нагрузка выше заданной
        Serial.println(current_mA);
        stopServo();  // остановить серво
        break;
      }
      rotateServo(step_speed);  // севро
    }
  } else {  // открыть

    while (millis() - startTime < ACCELERATION_TIME) {
      // Serial.println(step_speed);
      if (millis() - startTime < ACCELERATION_TIME / 4) {  // если не прошло больше 1/4 тогда ускоряемся
        if (step_speed > MIN_PULSE_WIDTH) {                // и если скороть не максимальная
          step_speed -= ACCELERATION_STEP_racing;          // ускорение
        }
        if (step_am < MAX_AM) {       // максимальная нагрузка пропорциональна ускорению
          step_am += AM_STEP_racing;  // увеличиваем нагрузку
        }
      } else if (millis() - startTime > ACCELERATION_TIME / 2) {  // если не прошло больше 1/2 тогда замедляемся
        if (step_speed < NEUTRAL_PULSE_WIDTH) {                   //и если скороть не минимальная
          step_speed += ACCELERATION_STEP_braking;                // замедление
        }
        if (step_am > MIN_AM) {        // минимальная нагрузка пропорциональна ускорению
          step_am -= AM_STEP_braking;  // уменьшаем нагрузку
        }
      }

      
       // график нагрузки 
      Serial.print(step_am);
      Serial.print(',');
      Serial.print(current_mA);
      Serial.println();
      
      if (current_mA > step_am) {  // если нагрузка выше заданной
        Serial.println(current_mA);
        stopServo();  // остановить серво
        break;
      }
      rotateServo(step_speed);  // севро
    }
  }
}

void rotateServo(int degrees) {  // севро
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(degrees);
  digitalWrite(SERVO_PIN, LOW);
  delay(speed - 1);  // Задержка для плавного вращения
  current_mA = getCurrent_mA();
}

void stopServo() {               // стоп севро
  digitalWrite(SERVO_PIN, LOW);  // Отключаем питание сервопривода
}
