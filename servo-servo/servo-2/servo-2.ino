
int servoPin = 9;  // сигнальный провод от серво на порт 9
int val;
const int acs712_pin = A0;
uint32_t tmr1;
int c;
int err;
const int SCALE_5A = 37.88;     // чувствительность 5-амперного модуля — 185 мВ/А
const int SCALE_20A = 20.48;    // чувствительность 20-амперного модуля — 100 мВ/А
const int SCALE_30A = 13.5168;  // чувствительность 30-амперного модуля — 66 мВ/А

int zero;  // уровень нуля, относительно которого измеряется ток, обычно VCC/2

float scale = SCALE_20A;  // выбираем для работы подходящее значение

void setup() {
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Servo is ready");
  zero = 0;
  int repeats = 10;
  for (int i = 0; i < repeats; i++) {
    zero += analogRead(acs712_pin);
    delay(100);
  } 
  zero /= repeats;  // берём среднее арифметическое
  Serial.print("Zero=");
  Serial.println(zero);
}

void loop() {

  val = Serial.read();
  val = val - '0';
  if (val == 1) {
    for (int i = 0; i <= 99; i++) {
      servoPulse(servoPin, i);
      if (err <= 5) {
        int sensorValue = analogRead(acs712_pin);
        c += getCurrent(sensorValue);
        err++;
      } else {
        err = 0;
        Serial.println(c);
        c = 0;
      }
      if (c >= 152) {
        servoPulse(servoPin, 0);
        break;
      }
    }
  }
  if (val == 0) {
    for (int i = 99; i >= 0; i--) {
      servoPulse(servoPin, i);
      if (err <= 5) {
        int sensorValue = analogRead(acs712_pin);
        c += getCurrent(sensorValue);
      } else {
        err = 0;
      }
      if (c >= 308) {
        servoPulse(servoPin, 99);
      }
    }
  }
  if (val == 2) {
    for (int i = 0; i <= 99; i++) {
      servoPulse(servoPin, i);
      int sensorValue = analogRead(acs712_pin) ;
      c += getCurrent(sensorValue);
    }
    Serial.println(c);
    c = 0;
  }
}
  void servoPulse(int pin, int angle) {
    for (int i = 0; i <= 3; i++) {
      // convert angle to 500-2480 pulse width
      int pulseWidth = 20 * angle + 500;
      if (millis() - tmr1 >= 5 - pulseWidth / 1000) {
        tmr1 = millis();
        digitalWrite(pin, HIGH);        // set the level of servo pin as high
        delayMicroseconds(pulseWidth);  // delay microsecond of pulse width
        digitalWrite(pin, LOW);
      } else {
        i--;
      }
    }
  }

  int getCurrent(int adc) {
    int delta = adc - zero;                   // отклонение от нуля шкалы
    int current = (int)delta * 1000 / scale;  // считаем ток в мА и округляем до целых
    return current;
  }