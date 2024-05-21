#include <NTPClient.h>
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h>	
#include <SoftwareSerial.h>

const char *ssid = "Msky"; //Название вашей сети
const char *password = "1q2w3e4r"; //Пароль сети

WiFiUDP ntpUDP; //Объект ntp
//UDPNTPClient timeClient(ntpUDP);// 
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000); //Так-же можно более детально настроить пул и задержку передачи.

char X [7] ={0x5A,0xA5,0x05,0x82,0x50,0x00,0x00};
char R [7] ={0x5A,0xA5,0x05,0x82,0x51,0x00,0x00};
char H [7] ={0x5A,0xA5,0x06,0x82,0x50,0x10,0x00};
int a = 0;
int s = 0;

SoftwareSerial mySerial(3,1);

void setup() {
  //Serial.begin(115200);
  mySerial.begin(115200);
  WiFi.begin(ssid, password); //Wifi  
  while (WiFi.status() != WL_CONNECTED) {    
    delay (500);    
    //Serial.print (".");  
  }  
  timeClient.begin(); //Запускаем клиент времени  
  timeClient.setTimeOffset(10800); //Указываем смещение по времени от Гринвича. Москва GMT+3 => 60*60*3 = 10800
}

void loop() {
    timeClient.update(); //Обновляем дату  
    a = a + 1;
    mySerial.write(X , 7);
    mySerial.write(a);
    s = s + 2;
    mySerial.write(R , 7);
    mySerial.write(s);

    mySerial.write(H , 7);
    mySerial.write(timeClient.getHours());
    mySerial.write(timeClient.getMinutes());
    //mySerial.write(timeClient.getSeconds());
    delay(1000);

}
