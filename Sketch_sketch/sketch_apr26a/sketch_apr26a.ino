#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Servo.h>

#define SERVO_PIN 9 // Пин, к которому подключен сервопривод
#define SERVO_MIN_ANGLE 0 // Минимальный угол сервопривода
#define SERVO_MAX_ANGLE 180 // Максимальный угол сервопривода

Adafruit_INA219 ina219;
Servo servo;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!");

  // Инициализация INA219
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  ina219.setCalibration_32V_2A();

  // Инициализация сервопривода
  servo.attach(SERVO_PIN);
}

void loop() {
  // Вращаем сервопривод
  for (int angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle++) {
    servo.write(angle);
    delay(20); // Задержка для плавного вращения

    // Считываем силу тока
    float current_mA = ina219.getCurrent_mA();
    Serial.print("Current: "); Serial.print(current_mA); Serial.println(" mA");
  }

  // Задержка после вращения
  delay(1000);
}