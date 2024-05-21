#include <Wire.h>
#include "PCF8575.h"
#include <DS18B20.h>
#include <SoftwareSerial.h>
EspSoftwareSerial::UART mdbPort;
int temp1 = 34;


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  test_temp();
  pinMode(temp1, INPUT);
}


void loop() {
  delay(500);
  test_temp();
}

void test_temp() {
  Serial.print("Temp.sensors test\n");
  Serial.print("T1: ");
  Serial.println(analogRead(temp1));
}