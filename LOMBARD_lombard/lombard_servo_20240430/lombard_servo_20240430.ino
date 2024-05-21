#include <Wire.h>
#include <Adafruit_INA219.h>
#define INA219_ADDRESS (0x40)  // Адрес INA219 на шине I2C
// основные переменные
#define SERVO_PIN 5               // Пин, к которому подключен сервопривод
#define MIN_PULSE_WIDTH 600       // Минимальная длительность импульса для 0 градусов
#define MAX_PULSE_WIDTH 2400      // Максимальная длительность импульса для 180 градусов
#define NEUTRAL_PULSE_WIDTH 1500  // длительность импульса для 90 градусов
#define ACCELERATION_TIME 3665    // время работы серво
#define MAX_AM 750                // максимальная нагрузка
#define MIN_AM 63                 // минимальная нагрузка
#define speed 20                  // скорость


//таймер
int count = 0;
unsigned long startTime;  // начало основного цикла
unsigned long time;       // начало основного цикла
bool flag_racing = true;
bool button_flag = true;  // нажатие кнопки
float current_mA_old = 0;
float current_mA = 0;                                                     // нагрузка
int ACCELERATION_STEP_braking = 900 / ((ACCELERATION_TIME / 2) / speed);  // Шаг изменения скорости поворота сервопривода замеднение
int ACCELERATION_STEP_racing = 900 / ((ACCELERATION_TIME / 4) / speed);   // Шаг изменения скорости поворота сервопривода ускорение
int AM_STEP_braking = MAX_AM / ((ACCELERATION_TIME / 2) / speed);         // Шаг изменения максимальной нагрузки замедление
int AM_STEP_racing = MAX_AM / ((ACCELERATION_TIME / 4) / speed);          // Шаг изменения максимальной нагрузки ускорение
int step_speed = 0;                                                       // шаг ускорения и замедления

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
  if (getCurrent_mA() == 0) {
    Serial.println("error INA219");
  }
}

// Если прерывание
void isr() {
  button_flag = false;  // кнопка нажата
  stopServo();          // остановить  серво
}

void loop() {
  parsing();
}

void parsing() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case '1':
        rotateServoWithAcceleration(false);  // открыть
        break;
      case '2':
        rotateServoWithAcceleration(true);  // закрыть
        break;
    }
  }
}

void rotateServo_speed() {

  if (millis() - startTime < ACCELERATION_TIME / 4) {  // если не прошло больше 1/4 тогда ускоряемся
    if (step_speed < 900) {                            // и если скороть не максимальная
      step_speed += ACCELERATION_STEP_racing;          // ускорение
      flag_racing = false;
    }
  } else if (millis() - startTime > (ACCELERATION_TIME / 2) && millis() - startTime < (ACCELERATION_TIME - 1000)) {  // если не прошло больше 1/2 тогда замедляемся 
    if (step_speed > 20) {                                                                                           //и если скороть не минимальная
      step_speed -= ACCELERATION_STEP_braking;                                                                       // замедление
    }
  } else if (millis() - startTime > (ACCELERATION_TIME - 1000)) { // очень плавное закрытие или открытие 
    step_speed = 100;
  } else {
    flag_racing = true;
  }
}


// основной цикл работы серво
void rotateServoWithAcceleration(int inc) {
  
  button_flag = true;    // кнопка не нажата
  Serial.println();      // очистить буфер
  startTime = millis();  // начало основного таймера для серво
  step_speed = 0;
  current_mA = 0;
  current_mA_old = 100;  // обнулить значение нагрузки
  while (millis() - startTime < ACCELERATION_TIME) {
    rotateServo_speed();
    /*
    // график нагрузки
    Serial.print(step_am);
    Serial.print(',');
    Serial.print(current_mA);
    Serial.println();
    */

    if (inc) {  // закрыть
      rotateServo(1500 + step_speed); 
      if (!button_flag) {
        stopServo();
        break;
      }
      if (current_mA > current_mA_old + 100) { //если больше заданого значение остановить и поднять шатер 
        stopServo();
        delay(50);
        open((millis() - startTime) * 4.4);
        break;
      }
    } else {  // открыть
      if (current_mA > current_mA_old + 120) { //если больше заданого значение остановить
        stopServo();
        break;
      }
      rotateServo(1500 - step_speed);
    }
  }
}


void open(unsigned long Time) { // плавно поднимает шатер с учетом пройденого времени 
  startTime = millis();  
  while (millis() - startTime < Time) {
    rotateServo(1400);  // открыть
  }
}


void rotateServo(int degrees) {  // севро
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(degrees);
  digitalWrite(SERVO_PIN, LOW);
  delay(speed - 1);  // Задержка для плавного вращения
  if (current_mA > current_mA_old || !flag_racing) {
    current_mA_old = current_mA;
  }
  current_mA = getCurrent_mA();
}

void stopServo() {               // стоп севро
  digitalWrite(SERVO_PIN, LOW);  // Отключаем питание сервопривода
}
