#include <Wire.h>
const int stepPin = 5; 
const int dirPin = 6; 
const int enPin = 7;
int speeddown = 400; //Скорость вниз(ниже быстрее)
int speeddup = 100; //Скорость вверх(ниже быстрее)
int mindis = 30; // Минимальное расстояние от сенсора
int maxdis = 40; // Максимальное расстояние от сенсора
int n = 1; // Среднее значение сенсора, чем быше тем точнее но медленее

unsigned char ok_flag;
unsigned char fail_flag;
unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag=0;
unsigned char ccnt = 82;

int x_mm; // 
float x; 

void setup() {
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,HIGH);
  Wire.begin(); 
  Serial.begin(9600); 
}


void loop() {
  int aver=ReadDistance();
       for(int tof = 0; tof <=  n; tof++)
    {
      x_mm = x_mm + ReadDistance();
    }
       x = x_mm / (n+1);
       x_mm = 0;
       Serial.println(x);

     if(x<mindis){
  digitalWrite(dirPin,LOW);
  for(int x = 0; x < 2400; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(speeddown);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(speeddown);
  }
  }
    else if(x>maxdis){
      digitalWrite(dirPin,HIGH);
       for(int x = 0; x < 2400; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(speeddup);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(speeddup);
  }}
  // else if(x>175){
  //   digitalWrite(stepPin,LOW);
  //   delayMicroseconds(speeddup);
  // }
  else{
    digitalWrite(stepPin,LOW);
    delayMicroseconds(speeddup);
  }
     }

void SensorRead(unsigned char addr,unsigned char* datbuf,unsigned char cnt) 
{
  unsigned short result=0;
  Wire.beginTransmission(ccnt); // передать на устройство 82 (0x52)
  Wire.write(byte(addr));      // устанавливает адрес данных расстояния (адрес)
  Wire.endTransmission();      // прекратить передачу
  delay(1);                   // в документации указано, что жда
  Wire.requestFrom(ccnt, cnt);    // запрос cnt байтов от ведо
  if (cnt <= Wire.available()) { // если два байта были получены
    *datbuf++ = Wire.read();  // получить старший байт (перезаписывает предыдущее считывание)
    *datbuf++ = Wire.read(); // получить младший байт как младшие 8 бит
  }
}

int ReadDistance(){
    SensorRead(0x00,i2c_rx_buf,2);
    lenth_val=i2c_rx_buf[0];
    lenth_val=lenth_val<<8;
    lenth_val|=i2c_rx_buf[1];
    delay(100); 
    return lenth_val;
}