#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define PIN_STRIP_1 5  // Pin for LED strip 1
#define PIN_STRIP_2 6  // Pin for LED strip 2
#define PIN_STRIP_3 7  // Pin for LED strip 3

#define NUM_LEDS_STRIP_1 32  // Number of LEDs in strip 1
#define NUM_LEDS_STRIP_2 32  // Number of LEDs in strip 2
#define NUM_LEDS_STRIP_3 32  // Number of LEDs in strip 3

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_STRIP_1, PIN_STRIP_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS_STRIP_2, PIN_STRIP_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS_STRIP_3, PIN_STRIP_3, NEO_GRB + NEO_KHZ800);

bool light_flag = false;    // true после парсинга
bool parsing_flag = false;  // true после команды на парсинг
char incomingByte = '0';    // переменная для Serial.read
char parsing_mass[6];       // массив для парсинга
char side_led = "";         // для выбора подсветки
int led_i = 0;              // счетчик для массива
int brightness_led = 0;     // для выбора яркости

String string_convert = "";

int step = 0;  // шаг

bool light = false;
bool shater = false;
bool ready = true;

bool left = false;
bool right = false;
bool front = false;

int power = 0;

const int button2Pin = 2;
const int acs712Pin = A0;  // ACS712 analog input pin

const float thresholdCurrent = 0.80;
const int servoSpeed = 30;

Servo myServo;
bool clockwise = true;
bool state = true;
bool state_open = true;

void setup() {
  pinMode(button2Pin, INPUT_PULLUP);
  myServo.attach(9);
  Serial.begin(115200);
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip1.show();
  strip2.show();
  strip3.show();
}

void loop() {
  parsing();
}

void parsing() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    // Начало команды
    if (incomingByte == 'S') shater = true;
    if (incomingByte == 'L') light = true;
    if (incomingByte == 'R') ready = true;
    if (incomingByte == ',') { string_convert = ""; }
    // переходим к аргуметам света
    if (incomingByte == 'l' && light) left = true;
    if (incomingByte == 'r' && light) right = true;
    if (incomingByte == 'f' && light) front = true;
    // устанавливаем яркость
    if (incomingByte != ',' && incomingByte != ';' && light == true && (left == true || right == true || front == true)) {
      string_convert += incomingByte;
      // Выключаем
      if (incomingByte == '0') {
        if (front) turnOffStrip(strip2, NUM_LEDS_STRIP_2);
        if (right) turnOffStrip(strip1, NUM_LEDS_STRIP_1);
        if (left) turnOffStrip(strip3, NUM_LEDS_STRIP_3);
        light = false;
        right = false;
        left = false;
        front = false;
        string_convert = "";
        Serial.println("L_off");
      }
      // Включаем
      if (string_convert.toInt() > 0 && incomingByte != ',' && incomingByte != ';') {
        if (front) turnOnStrip(strip2, NUM_LEDS_STRIP_2, string_convert.toInt() * 25);
        if (right) turnOnStrip(strip1, NUM_LEDS_STRIP_1, string_convert.toInt() * 25);
        if (left) turnOnStrip(strip3, NUM_LEDS_STRIP_3, string_convert.toInt() * 25);
        light = false;
        right = false;
        left = false;
        front = false;
        string_convert = "";
        Serial.println("L_on");
      }
    }
    // аргументы шатера
    if (incomingByte != ',' && incomingByte != ';' && shater == true) {
      if (incomingByte == '1') {
        move_shater(1);
        shater = false;
      }
      if (incomingByte == '0') {
        move_shater(0);
        shater = false;
      }
    }
    if (incomingByte == ';' || incomingByte == '\n') {
      light = false;
      right = false;
      left = false;
      front = false;
      string_convert = "";
      shater = false;
    }
  }
}


void move_shater(bool open) {
  state = false;

  if (open) {
    myServo.write(0);  // команда открытия шатера
    while (!state) {
      if (getCurrent() > thresholdCurrent) {  // если ток скакнул
        myServo.write(90);                    // остановить
        Serial.println("Open");               // сообщить
        state = true;
      }
    }
  }

  if (!open) {
    myServo.write(180);  // команда закрытия шатера
    while (!state) {
      if (digitalRead(button2Pin) == LOW && getCurrent() > thresholdCurrent) {
        myServo.write(90);
        Serial.println("Close");
        state = true;
      } else if (digitalRead(button2Pin) == HIGH && getCurrent() > thresholdCurrent) {  
        myServo.write(90);
        Serial.println("S error");
        myServo.write(0);
        state = true;
      }
    }
  }
}

float getCurrent() {
  int sensorValue = analogRead(acs712Pin);
  float voltage = sensorValue * (5.0 / 1023.0);  // Convert analog reading to voltage
  float current = (voltage - 2.5) / 0.185;       // ACS712 sensitivity is 185mV/A
  //Serial.println(current);
  return abs(current);
}
// включить ленту
void turnOnStrip(Adafruit_NeoPixel& strip, int numLEDs, int brightness) {
  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, strip.Color(brightness, brightness, brightness, 255));  // Set color to white (you can change this)
  }
  strip.show();
}
// выключить ленту
void turnOffStrip(Adafruit_NeoPixel& strip, int numLEDs) {
  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Set color to off
  }
  strip.show();
}
