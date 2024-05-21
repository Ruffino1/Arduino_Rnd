#include <Servo.h>

Servo myServo;
int servoPin = 9;  // сигнальный провод от серво на порт 9
int button_touch = 3;
uint32_t tmr1;
int buttonState = 0;
bool cl_flag = false;
bool gh_flag = false;


void setup() {
  // pinMode(servoPin, OUTPUT);
  pinMode(button_touch, INPUT);
  myServo.attach(9);
  Serial.begin(9600);
  Serial.println("Servo is ready");
}

void loop() {
   myServo.write(0);
  //logic();
}


void logic() {
  buttonState = digitalRead(button_touch);
  if (buttonState == HIGH && !cl_flag) {
    cl_flag = true;
    myServo.write(175);
    tmr1 = millis();
  } else if (cl_flag) {
    if (millis() - tmr1 >= 1270) {
      myServo.write(0);
      if (buttonState == LOW)
      {
        cl_flag = false;
      }
    }
  }
}
