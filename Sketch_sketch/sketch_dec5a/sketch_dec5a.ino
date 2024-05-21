#include <Wire.h>
#include "PCF8575.h"
#include <DS18B20.h>
#include <SoftwareSerial.h>
EspSoftwareSerial::UART mdbPort;

int set_value_temp = 20;
bool flag_temp = true;
bool flag_temp2 = true;
int R2 = 4700;
float Du;  //напряжение
float RT;  //сопротивление
float logR2, T;
//c1, c2, c3 - коэффициенты Штейнхарта-Харта для термистора
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
bool flag = true;
bool flag2 = false;
bool money = false;
unsigned long timing;
PCF8575 PCF(0x20);


void setup() {
  delay(5000);
  Serial.begin(19200);
  Serial.println("Start");
  pinMode(27, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  delay(1000);
  if (Serial.available()) {
    if (Serial.parseInt() == 1){
      set_value_temp += 1;
    }
    if (Serial.parseInt() == 0){
      set_value_temp -= 1;
    }
     Serial.println(set_value_temp);
  }
  temp_logic();
  Serial.print("temp-1: ");
  Serial.println(temp_analog(34));
  Serial.print("temp-2: ");
  Serial.println(temp_analog(35));
  
  if (PCF.read(9) == 0) {
    flag = false;
    flag2 = true;
  }
  if (!flag) {
    motor();
  }
}


void motor() {

  if (!flag) {
    digitalWrite(13, HIGH);
  }
  if (millis() - timing > 300) {
    timing = millis();
    PCF8575 PCF(0x20);

    if (PCF.read(8) == 1) {
      if (!flag && !flag2) {
        digitalWrite(13, LOW);
        flag = true;
      }
      flag2 = true;
    } else {
      flag2 = false;
    }
  }
}


void test_pcf() {
  PCF8575 PCF(0x20);
  if (PCF.begin(33, 32)) {
    Serial.print("Input num value:\n");

    Serial.println("OUT");
    for (int i = 0; i < 8; i++) {
      Serial.print(i);
      Serial.print(" ");
      Serial.println(PCF.read(i));
    }
    Serial.println("in");
    for (int i = 0; i < 8; i++) {
      Serial.print(i);
      Serial.print(" ");
      Serial.println(PCF.read(i + 8));
    }
  }
}

void temp_logic() {
  if (temp_analog(35) > 5 && flag_temp) {
    if (temp_analog(34) > set_value_temp && flag_temp2) {
      digitalWrite(27, HIGH);
    } else {
      digitalWrite(27, LOW);
      if (temp_analog(34) < set_value_temp + 2) {
        flag_temp2 = false;
      } else {
        flag_temp2 = true;
      }
    }
  } else {
    digitalWrite(27, LOW);
    if (temp_analog(35) < 10) {
      flag_temp = false;
    } else {
      flag_temp = true;
    }
  }
}

float temp_analog(int pin) {
  Du = analogRead(pin);
  RT = R2 / (3950 / Du - 1);  //преобразуем Du в сопротивление
  logR2 = log(RT);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  //температура в Кельвине
  T = T - 273.15;
  return(T);                                          //преобразование Кельвина в Цельсия
}