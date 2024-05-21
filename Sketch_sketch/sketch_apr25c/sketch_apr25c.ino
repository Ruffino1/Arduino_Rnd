#define SERVO_PIN 5 // Пин, к которому подключен сервопривод
#define MAX_DURATION 1000 // Максимальное время вращения в миллисекундах
#define ACCELERATION_TIME 500 // Время ускорения и замедления в миллисекундах
#define PLATEAU_TIME (MAX_DURATION - 2 * ACCELERATION_TIME) // Продолжительность плато

unsigned long startTime;

void setup() {
  pinMode(SERVO_PIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      rotateRight();
    } else if (command == '2') {
      rotateLeft();
    }
  }
}

void rotateRight() {
  digitalWrite(SERVO_PIN, HIGH); // Включаем сервопривод
  startTime = millis();
  
  // Ускорение
  while (millis() - startTime < ACCELERATION_TIME) {
    // Ждем ускорение
  }

  // Плато
  while (millis() - startTime < ACCELERATION_TIME + PLATEAU_TIME) {
    // Ничего не делаем
  }
  
  // Замедление
  while (millis() - startTime < MAX_DURATION) {
    // Ждем замедление
  }
  
  digitalWrite(SERVO_PIN, LOW); // Отключаем сервопривод
}

void rotateLeft() {
  digitalWrite(SERVO_PIN, HIGH); // Включаем сервопривод
  startTime = millis();
  
  // Ускорение
  while (millis() - startTime < ACCELERATION_TIME) {
    // Ждем ускорение
  }

  // Плато
  while (millis() - startTime < ACCELERATION_TIME + PLATEAU_TIME) {
    // Ничего не делаем
  }
  
  // Замедление
  while (millis() - startTime < MAX_DURATION) {
    // Ждем замедление
  }
  
  digitalWrite(SERVO_PIN, LOW); // Отключаем сервопривод
}
