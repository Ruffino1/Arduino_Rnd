
int servoPin = 9;  // сигнальный провод от серво на порт 9
int val;
const int acs712_pin = A0;
uint32_t tmr1;
int c;
int err;
int zero;  // уровень нуля, относительно которого измеряется ток, обычно VCC/2
void setup() {
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Servo is ready");
}

void loop() {
  val = Serial.read();
  val = val - '0';

  if (val == 1) {
    for (int i = 0; i <= 99; i++) {
      servoPulse(servoPin, i);
      if (err <= 3) {
        c += analogRead(acs712_pin) - zero;
        err++;
      } else {
        err = 0;
        c = 0;
      }
      if (c > 65) {
        servoPulse(servoPin, 0);
        c = 0;
        break;
      }
    }
  }
  if (val == 2) {
    for (int i = 99; i >= 0; i--) {
      servoPulse(servoPin, i);
      if (err <= 3) {
        c += analogRead(acs712_pin) - zero;
        err++;
      } else {
        err = 0;
        c = 0;
      }
      if (c > 65) {
        servoPulse(servoPin, 99);
        c = 0;
        break;
      }
    }
  }
  if (val == 0) {
    zero = 0;
    int repeats = 10;
    for (int i = 0; i < repeats; i++) {
      zero += analogRead(acs712_pin);
      Serial.print("Zero=");
      Serial.println(zero);
      delay(100);
    }
    zero /= repeats;  // берём среднее арифметическое
    Serial.print("Zero=");
    Serial.println(zero);
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