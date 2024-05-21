#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

/*
OUT
  Сервопривод дверцы - D5
  Сервопривод L - D9
  Сервопривод С - D10
  Адресная лентав - D13 и D6
  Переключатель USB - D7 и D8
IN
  парсинг
  датчик закрытой двери
  Датчик телефона?
*/
//INA219
#define INA219_ADDRESS (0x40)  // Адрес INA219 на шине I2C

//servo
#define SERVO_PIN 5               // Пин, к которому подключен сервопривод
#define PULSE_WIDTH 900           //  длительность импульса
#define NEUTRAL_PULSE_WIDTH 1500  // длительность импульса для 90 градусов
#define ACCELERATION_TIME 3770    // время работы серво
#define speed 19                  // скорость

bool button_flag = true;  // нажатие кнопки

int ACCELERATION_STEP_braking = PULSE_WIDTH / ((ACCELERATION_TIME / 2) / speed);  // Шаг изменения скорости поворота сервопривода замеднение
int ACCELERATION_STEP_racing = PULSE_WIDTH / ((ACCELERATION_TIME / 4) / speed);   // Шаг изменения скорости поворота сервопривода ускорение


//servo usb
const uint16_t s_close = 2550;  //2000;               // Время импульса паузы сервопривода микросекунды
const uint16_t half = 1300;
const uint16_t s_open = 400;
const uint8_t out_s_l = 2;
const uint8_t out_s_c = 3;
const uint8_t out_s_s = 4;

//led
#include <Adafruit_NeoPixel.h>
#define LED_PIN 13    // Пин, к которому подключена светодиодная лента
#define LED_COUNT 30  // Количество светодиодов в ленте
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

// parsing
int step = 0;
char incomingByte = '0';
String string_convert = "";
String answer = "";

void setup() {
  Wire.begin();
  Serial.begin(9600);
  //Serial.println("Hello!");
  attachInterrupt(0, isr, FALLING);

  pinMode(SERVO_PIN, OUTPUT);
  if (getCurrent_mA() == 0) {
    Serial.println("error INA219");
  }

  // Инициализация сервопривода usb
  pinMode(out_s_l, OUTPUT);
  pinMode(out_s_c, OUTPUT);
  pinMode(out_s_s, OUTPUT);

  strip.begin();
  strip.show();  // Инициализация светодиодной ленты
}

// Если прерывание
void isr() {
  button_flag = false;  // кнопка нажата
  stopServo();          // остановить  серво
}

void loop() {
  parsing_igor();
}

void parsing_igor() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case '1':
        rotateServoWithAcceleration(false);  // открыть
        break;
      case '2':
        rotateServoWithAcceleration(true);  // закрыть
        break;
      case '3':
        fadeIn(); //вкл ленту
        break;
      case '4':
        fadeOut(); //выкл ленту
        break;
    }
  }
}
