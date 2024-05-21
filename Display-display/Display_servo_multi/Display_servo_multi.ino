#include <TM1637Display.h>
#include <Servo.h>

char incomingByte;

bool Multi_flag = false;
bool servo_flag = false;

// SERVO
bool ready = false;
const uint8_t servo_ctrl = 6;  // Пин управления сервоприводом
const uint16_t home = 360;     //1100;                  // Время импульса паузы сервопривода микросекунды
const uint16_t getit = 2450;
uint32_t time_double_Click;
int pushButton = 4;

int multi = 10;
int servo = 0;
// Сервоприводы
int servo_light_blue = 3;
int servo_black = 4;
int servo_green = 5;
int servo_pink = 6;
int servo_blue = 7;
int servo_white = 8;
int servo_red = 9;
int servo_yellow = 10;
int servo_brown = 11;
int servo_purple = 12;

//дисплей
#define CLK 13
//#define DIO A0

//мульти
#define S0 15
#define S1 16
#define S2 17
#define S3 18
#define SIG A0

TM1637Display display = TM1637Display(CLK, SIG);

int number = 0;

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  //display.clear();
  display.setBrightness(7);
  Serial.begin(9600);
}


void loop() {
  parsing();
}

void getit_servo() {  // Возврат всех сервоприводов в домашнее положени
  for (int g = 3; g <= 12; g++) {
    for (int j = 0; j <= 10; j++) {
      digitalWrite(g, HIGH);
      delayMicroseconds(getit);
      digitalWrite(g, LOW);
      delay(20 - (getit / 1000));
    }
    delay(1000);
  }
}

void home_servo() {  // Возврат всех сервоприводов в домашнее положение
  for (int g = 3; g <= 12; g++) {
    for (int j = 0; j <= 10; j++) {
      digitalWrite(g, HIGH);
      delayMicroseconds(home);
      digitalWrite(g, LOW);
      delay(20 - (home / 1000));
    }
    delay(1000);
  }
}

void parsing() {  // Парсинг входящих сообщений с линии RS485
  if (Serial.available() > 0) {

    incomingByte = Serial.read();
    if (incomingByte == 'S' || Multi_flag == true) {
      Serial.println("324");
      getit_servo();
    }
    if (incomingByte == 'P' || servo_flag == true) {
      home_servo();
    }
  }
}






void main_logic() {
  switch (multi) {
    case 0:  // поворот на 0 градусов
      readMux(0);
      rotate_servo();
      Serial.println("you");
      display.showNumberDec(666);
      delay(2000);
      display.showNumberDec(1);
      break;
    case 1:
      readMux(1);
      rotate_servo();
      Serial.println("u");
      display.showNumberDec(666);
      delay(2000);
      display.showNumberDec(2);
      break;
    case 2:
      readMux(2);
      rotate_servo();
      display.showNumberDec(666);
      delay(2000);
      display.showNumberDec(3);
      break;
    case 3:

      readMux(3);
      display.showNumberDec(666);
      delay(2000);

      display.showNumberDec(4);
      break;
    case 4:

      display.showNumberDec(666);
      delay(2000);

      display.showNumberDec(5);
      break;
    case 5:

      readMux(5);
      display.showNumberDec(666);
      delay(2000);

      display.showNumberDec(6);
      break;
    case 6:

      readMux(6);
      display.showNumberDec(666);
      delay(2000);

      display.showNumberDec(7);
      break;
    case 7:

      readMux(7);
      display.showNumberDec(666);
      delay(2000);

      display.showNumberDec(8);
      break;
    case 8:

      readMux(8);
      display.showNumberDec(666);
      delay(2000);

      display.showNumberDec(9);
      break;
    case 9:

      readMux(9);
      display.showNumberDec(666);
      delay(2000);

      display.showNumberDec(10);
      break;
  }
}
int readMux(int channel) {
  int controlPin[] = { S0, S1, S2, S3 };

  int muxChannel[16][4] = {
    { 0, 0, 0, 0 },  //channel 0
    { 1, 0, 0, 0 },  //channel 1
    { 0, 1, 0, 0 },  //channel 2
    { 1, 1, 0, 0 },  //channel 3
    { 0, 0, 1, 0 },  //channel 4
    { 1, 0, 1, 0 },  //channel 5
    { 0, 1, 1, 0 },  //channel 6
    { 1, 1, 1, 0 },  //channel 7
    { 0, 0, 0, 1 },  //channel 8
    { 1, 0, 0, 1 },  //channel 9
    { 0, 1, 0, 1 },  //channel 10
    { 1, 1, 0, 1 },  //channel 11
    { 0, 0, 1, 1 },  //channel 12
    { 1, 0, 1, 1 },  //channel 13
    { 0, 1, 1, 1 },  //channel 14
    { 1, 1, 1, 1 }   //channel 15
  };

  //loop through the 4 sig
  for (int i = 0; i < 4; i++) {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG);

  //return the value
  return val;
}

void rotate_servo() {
  for (byte l = 0; l <= 20; l++) {
    digitalWrite(servo, HIGH);
    delayMicroseconds(getit);
    digitalWrite(servo, LOW);
    delay(20 - (getit / 1000));
  }
  for (byte c = 0; c <= 20; c++) {
    digitalWrite(servo, HIGH);
    delayMicroseconds(home);
    digitalWrite(servo, LOW);
    delay(20 - (home / 1000));
  }
  ready = true;
}