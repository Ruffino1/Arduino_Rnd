#include <Wire.h>
#include "PCF8575.h"
#include <DS18B20.h>
#include <SoftwareSerial.h>
int s = 0;
int ss = 0;
bool flag = true;
bool flag2 = false;
unsigned long timing;
EspSoftwareSerial::UART mdbPort;


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  test_pcf();
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  
}

void loop() {
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
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
  }
  if (millis() - timing > 300) {
    timing = millis();
    PCF8575 PCF(0x20);
    
    if (PCF.read(8) == 1) {
      if (!flag && !flag2) {
        pinMode(6, OUTPUT);
        digitalWrite(6, LOW);
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
  if (PCF.begin(16, 15)) {
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