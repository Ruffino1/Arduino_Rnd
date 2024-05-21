#include <Wire.h>

 

#define TOF050F_IIC_ADDR 0x69 
 // IIC address for TOF050F module

 

void setup() {

  Wire.begin();

  Serial.begin(9600);
  Serial.println("ok");
}

 

void loop() {

  // Request distance measurement

  Wire.beginTransmission(TOF050F_IIC_ADDR);

  Wire.write(0x00); // Command register

  Wire.write(0x01); // Command to initiate measurement

  Wire.endTransmission();

 

  // Read distance data

  Wire.requestFrom(TOF050F_IIC_ADDR, 2);

  if (Wire.available() >= 2) {

    int distance = Wire.read() << 8 | Wire.read();

    Serial.print("Distance: ");

    Serial.print(distance);

    Serial.println(" mm");

 }

 

  delay(1000);  // Delay for 1 second

}