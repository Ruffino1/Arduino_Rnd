#include <Arduino.h>


//tenzo
#include <DFRobot_HX711.h>
DFRobot_HX711 TenzoScale(A1, A0);
DFRobot_HX711 TenzoScale2(A3, A2);
long zero_tenzo = 0;


//led
#include <Adafruit_NeoPixel.h>
#define LED_PIN 13
#define LED_COUNT 34
#define BRIGHTNESS 100
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

/*
//Display
#include <TM1637Display.h>
#define CLK A2  //13
#define DIO A3  //A0
TM1637Display display(CLK, DIO);

const uint8_t D_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

const uint8_t D_pu[] = {
  SEG_G,  //| SEG_DP
  SEG_F | SEG_A | SEG_B | SEG_G,
  SEG_E | SEG_G | SEG_C | SEG_D,
  SEG_G
};

//multipl
//#define S0 15
//#define S1 16
//#define S2 17
//#define S3 18

bool adr_space[][4] = {
  //Массив адресов аналогового мультиплексора
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
*/
//servo
const uint16_t d_close = 400;  //350   // Время импульса паузы сервопривода микросекунды
const uint16_t half = 940; //1200
const uint16_t getit = 2500;  //2550
const uint8_t out_servo = 4;

//stepper
const uint8_t out_step = 2;
const uint8_t out_dir = 3;

// inputs
const uint8_t in_zero = 5;
const uint8_t in_door = 6;
const uint8_t in_hand = 7;

int acc = 4;          // max 40
int speed = 180;      // 10-100
long int c_step = 0;  // счётчик шагов

int step = 0;
char incomingByte = '0';
String string_convert = "";
String answer = "";

// BOOL
bool dir, home, open = false;
bool h_servo, ready = true;
bool P_select = false;


int pusher = 0;
int Lp = 0;
int Cen = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  //tenzo_setup
  delay(600);
  zero_tenzo = TenzoScale.averageValue(1) + TenzoScale2.averageValue(1);  // калибровка нуля

  //Serial.println("START");
  delay(600);

  pinMode(in_zero, INPUT_PULLUP);  //INPUT_PULLUP
  pinMode(in_door, INPUT_PULLUP);
  pinMode(in_hand, INPUT_PULLUP);

  pinMode(out_servo, OUTPUT);
  pinMode(out_step, OUTPUT);
  pinMode(out_dir, OUTPUT);



  //pinMode(S0, OUTPUT);
  //pinMode(S1, OUTPUT);
  //pinMode(S2, OUTPUT);
  //pinMode(S3, OUTPUT);

  //digitalWrite(S0, LOW);
  //digitalWrite(S1, LOW);
  //digitalWrite(S2, LOW);
  //digitalWrite(S3, LOW);
  gohome();
  led(3);
}

void loop() {
  if (ready && !home) {
    speed = 400;
    move(100000);
  }
  parsing();
}

void get_zero_tenzo() {  // калибровка нуля
  zero_tenzo = TenzoScale.averageValue(1) + TenzoScale2.averageValue(1);
}

float get_value_tenzo() {  // получить значение тензо
  long val = (TenzoScale.averageValue(1) + TenzoScale2.averageValue(1) - zero_tenzo);
  if (val / 365 < 0) {
    return (0);
  } else {
    return (float)val / 365;  // 365;
  }
}

void chip_select(int inp) {  // Выбор ноги мультиплексора
  //Serial.print(" M = ");
  //Serial.println(inp);
  //digitalWrite(S0, adr_space[inp][0]);
  //digitalWrite(S1, adr_space[inp][1]);
  //digitalWrite(S2, adr_space[inp][2]);
  //digitalWrite(S3, adr_space[inp][3]);
}

void led(int stat) {  // команды для подвсетки
  if (stat == 0) {    // белый
    for (int j = 50; j <= 250; j += 25) {
      for (int i = 0; i <= LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, j));
        strip.show();
        
      }
      //delay(2);
    }
  }
  if (stat == 1) {  // красный
    for (int j = 50; j <= 250; j += 25) {
      for (int i = 0; i <= LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(j, 0, 0, 0));
        strip.show();
        
      }
      //delay(2);
    }
  }
  if (stat == 2) {  // желтый
    for (int j = 50; j <= 250; j += 25) {
      for (int i = 0; i <= LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(j, j, 0, 0));
        strip.show();
        
      }
      //delay(2);
    }
  }
  if (stat == 3) {  // зеленый
    for (int j = 50; j <= 250; j += 25) {
      for (int i = 0; i <= LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(0, j, 0, 0));
        strip.show();
        
      }
      //delay(2);
    }
  }
  if (stat == 4) {  // зеленый
    rainbow();
  }
}

void rainbow() {
  uint16_t i, j;
  while (Serial.available() == 0) {
    for (j = 100; j < 256; j++) {
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, j, 0, 0));
      }
      strip.show();
      if (Serial.available() > 0) {
        led(3);
        break;
      }
      delay(2);
    }
    for (j = 255; j != 100; j--) {
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, j, 0, 0));
      }
      strip.show();
      if (Serial.available() > 0) {
        led(3);
        break;
      }
      delay(6);
    }
  }
}


/*
void disp_cen() {
  chip_select(pusher - 1);
  display.setBrightness(0x0f);
  display.clear();
  display.showNumberDec(Cen);
}
*/