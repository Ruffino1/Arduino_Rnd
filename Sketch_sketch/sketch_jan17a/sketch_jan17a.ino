
#define BUT 2   // кнопка
#define SET 4   // SET

byte but[2];

void setup() {
  pinMode(BUT, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SET, OUTPUT);
  digitalWrite(SET, HIGH);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    byte a = Serial.read();
    if (a == '1') {           // если пришла команда на включение, зажигаем светодиод
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (a == '5') {    // если на выключение - выключаем
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  but_read();                 // отслеживание нажатий кнопки
}

void but_read() {
  static unsigned long timerT;
  if ((timerT + 100) > millis()) return;
  but[0] = but[1];
  but[1] = digitalRead(BUT);
  if (but[0] && !but[1]) Serial.write("1"); // кнопка нажата, отправляем 1
  if (!but[0] && but[1]) Serial.write("5"); // кнопка отжата, отправляем 5
  timerT = millis();
}