#include <Arduino.h>
#include <GyverHX711.h>

bool ready = true;
bool home = false;
bool dir = 0;
bool alarm = false;
bool b1 = true;
bool b2 = true;

// IO

// RGB
/*
#include <FastLED_NeoPixel.h>
#define DATA_PIN 19
#define NUM_LEDS 8
#define BRIGHTNESS 50
FastLED_NeoPixel<NUM_LEDS, DATA_PIN, NEO_GRB> strip;
*/
// RGBW
#include <Adafruit_NeoPixel.h>
#define LED_PIN 19
#define LED_COUNT 32
#define BRIGHTNESS 50  // Set BRIGHTNESS to about 1/5 (max = 255)
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

// мультиплексор
//const uint8_t in_p = 3;   // датчики на мультиплексоре
int multi = 20;           // Номер текущего пина
const uint8_t m0_p = 12;  // Пин выбора для мультплексора
const uint8_t m1_p = 11;  // Пин выбора для мультплексора
const uint8_t m2_p = 10;  // Пин выбора для мультплексора
const uint8_t m3_p = 9;   // Пин выбора для мультплексора
bool adr_space[][4] = {
  // Массив адресов аналогового мультиплексора
  { 0, 0, 0, 0 },  //0, 0-3
  { 1, 0, 0, 0 },  //1, 4-7
  { 0, 1, 0, 0 },  //2, 8-11
  { 1, 1, 0, 0 },  //3, 12-15
  { 0, 0, 1, 0 },  //4, 16-19
  { 1, 0, 1, 0 },  //5, 20-23
  { 0, 1, 1, 0 },  //6, 24-27
  { 1, 1, 1, 0 },  //7, 28-31
  { 0, 0, 0, 1 },  //8, 32-35
  { 1, 0, 0, 1 },  //9, 36-39
  { 0, 1, 0, 1 },  //10, 40-43
  { 1, 1, 0, 1 },  //11, 44-47
  { 0, 0, 1, 1 },  //12, 48-51
  { 1, 0, 1, 1 },  //13, 52-55
  { 0, 1, 1, 1 },  //14, 56-59
  { 1, 1, 1, 1 },  //15, 60-63
};

// энкодер
const uint8_t enc_p = 2;  // Пин энкодера
int k_enc = 10;           // Множитель для двигателя
int l_enc = 0;            // Старый показатель энкодера
int enc = 0;
bool enc_on = false;

// нулевая позиция
const uint8_t zero_p = 13;
long last_time = 0;

// всякие датчики
bool input_b = false;
const uint8_t in_p = 3;   // Датчик на входе
const uint8_t m_p = 6;    // Индукционные датчики
const uint8_t up_p = 15;  // Датчик на барабане
const uint8_t t1_p = 17;  // Датчик бак 1
const uint8_t t2_p = 18;  // Датчик бак 2


// двигатель
const uint8_t step_p = 4;
const uint8_t dir_p = 5;
int acc = 15;    // max 40
int speed = 80;  // 10-100
int c_step = 0;  // счётчик шагов

// пременые паузы
long s_in = 0;      // счётчик шагов для паузы
bool d_in = false;  // направление
int step_in = 0;    // текущий шаг

// тензо
GyverHX711 sensor(A1, A0, HX_GAIN64_A);
// HX_GAIN128_A - канал А усиление 128
// HX_GAIN32_B - канал B усиление 32
// HX_GAIN64_A - канал А усиление 64
int t_cl = 0;
int t_old = 0;
int t_now = 0;
int t_sum = 0;
bool t = false;
bool empt = false;
long time = millis();

//main
int step = 0;
String str;
int tank = 0;
int err = 0;


void setup() {
  // включаем сериал
  Serial.begin(9600);
  // IO
  pinMode(step_p, OUTPUT);
  pinMode(dir_p, OUTPUT);

  pinMode(m0_p, OUTPUT);
  pinMode(m1_p, OUTPUT);
  pinMode(m2_p, OUTPUT);
  pinMode(m3_p, OUTPUT);

  pinMode(up_p, INPUT);
  pinMode(t1_p, INPUT);
  pinMode(t2_p, INPUT);
  pinMode(m_p, INPUT_PULLUP);
  pinMode(in_p, INPUT);
  pinMode(enc_p, INPUT);
  pinMode(zero_p, INPUT);

  strip.begin();  // initialize strip (required!)
  strip.show();   // Update strip to match
  strip.setBrightness(BRIGHTNESS);

  // прерывание энкодера
  //attachInterrupt(0, encIsr, RISING);  //CHANGE FALLING RISING
  // и датчика 0
  attachInterrupt(1, inIsr, FALLING);
  delay(500);
  sensor.tare();
}


void encIsr() {
  if (dir) enc++;
  else enc--;
  //Serial.print(":");
  //Serial.print(enc);
}

void inIsr() {  // если сработал датчик на входе
  if (digitalRead(in_p)) {
    input_b = true;
    //Serial.print("input");
  }
}

void encoder() {
  if (enc_on && !digitalRead(enc_p)) {
    enc++;
    //if (dir) enc++;
    //else enc--;
    enc_on = false;
  }
  if (!enc_on && digitalRead(enc_p)) enc_on = true;
}

void m_select(int inp) {  // выбор порта мультиплексора
  digitalWrite(m0_p, adr_space[inp][0]);
  digitalWrite(m1_p, adr_space[inp][1]);
  digitalWrite(m2_p, adr_space[inp][2]);
  digitalWrite(m3_p, adr_space[inp][3]);
}

void tenzo_f_read() {
  t_sum = 0;
  for (int i = 0; i < 10;) {
    if (sensor.available()) {
      t_sum += sensor.read();
      i++;
    }
  }
  t_now = t_sum / 109;
  t_sum = 0;
  t_cl = 0;
}


int tenzo_read(bool reset = false) {
  //ready = false;
  int s = 0;
  if (reset) {
    s = 0;
    t_sum = 0;
    t_cl = 0;
    t_now = s;
    sensor.tare();
  }
  if (sensor.available()) {
    t_sum += sensor.read();
    t_cl++;
    if (t_cl == 20) {
      s = t_sum / 219;  //109.5
      t_sum = 0;
      t_cl = 0;
      t_now = s;
      return (s);
    }
  }
}


void led_all(uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
  }
}

void led_ind(uint32_t color) {
  for (int i = 0; i < 8; i++) {
    strip.setPixelColor(i, color);
    strip.show();
  }
  for (int i = strip.numPixels() - 4; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
  }
}