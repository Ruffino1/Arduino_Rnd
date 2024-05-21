#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>

int t = 0;
const byte rxPin = 0;  
const byte txPin = 1;  

unsigned char Buffer[9];
#define text_list 0x61

unsigned char text_[8] = { 0x5a, 0xa5, 0x05, 0x82, text_list, 0x00, 0x00, 0x00 };

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}


void loop() {
  sensor_data();
  delay(10);
  realy_control();
  delay(50);
}


void sensor_data() {
  if (Serial.available()) { 
      t = Serial.parseInt();
    }
  text_[6] = highByte(t);
  text_[7] = lowByte(t);
  Serial.write(text_, 8);
}

void realy_control() {
  if (Serial.available()) {
    for (int i = 0; i <= 8; i++)
    {
      Buffer[i] = Serial.read();
      Serial.println(Buffer[i]);
    }

    if (Buffer[0] == 0X5A) {
      if (Buffer[8] == 1) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("ON");
      } else {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("OFF");
      }
    }
  }
}