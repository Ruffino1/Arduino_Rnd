#include <Servo.h> // подключаем библиотеку для работы с сервоприводом

Servo servo; // объявляем переменную servo типа "servo1"
const int analogIn = A0;
int mVperAmp = 185; // use 185 for 5A Module, 100 for 20A Module, 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;
int i;
uint32_t tmr1;
uint32_t tmr2;
uint32_t tmr3;
uint32_t tmr4;
bool flag;

void setup() {
   servo.attach(A1); // привязываем сервопривод к аналоговому выходу 11
   Serial.begin(9600);
}

void loop() {
    for (i = 0; i <= 180; i = i + 2) {
      if (millis() - tmr3 >= 30) {
        tmr3 = millis();
        servo.write(i);
      }
      else {
          i = i - 2;
        } 
      if (i == 170){
        flag = true;
      }
    }
    if (flag == true) {
      for (i = 180; i >= 0; i = i - 2) {
        if (millis() - tmr4 >= 30) {
          tmr4 = millis();
          servo.write(i);
          if (i % 20 != 0) {
            RawValue = analogRead(analogIn);
            Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
            Amps = ((Voltage - ACSoffset) / mVperAmp);
            Serial.print("\t Amps = "); 
            Serial.println(Amps,2);
        } 
        }
        else {
          i = i + 2;
        }
        if(i == 0){
          flag = false;
        } 
      }
    }
}

