#include <Wire.h>
#include "PCF8575.h"
#include <DS18B20.h>
#include <SoftwareSerial.h>
EspSoftwareSerial::UART mdbPort;

int R2=10000;
float Du;     //напряжение
float RT;     //сопротивление 
float logR2, T;
//c1, c2, c3 - коэффициенты Штейнхарта-Харта для термистора
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741; 



void setup() {
  Serial.begin(115200);
  Serial.println("Start");
}


void loop() {
  delay(2000);
  test_temp();
 // temp_analog();
}

void test_temp() {
  Serial.print("Temp.sensors test\n");
  DS18B20 ds1(32);
  if (ds1.selectNext()) {
    Serial.print("T1: ");
    Serial.println(ds1.getTempC());
  }
}

void temp_analog() 
{
  Du = analogRead(35);
  RT = R2/(1023/Du - 1); //преобразуем Du в сопротивление
  Serial.print("RT=");
  Serial.print(RT);
  logR2 = log(RT);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); //температура в Кельвине
  T = T - 273.15; //преобразование Кельвина в Цельсия
  Serial.print(", T="); 
  Serial.print(T);
  Serial.println("C");
}