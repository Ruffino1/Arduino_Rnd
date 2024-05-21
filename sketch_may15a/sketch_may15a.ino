#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Msky";          //Название вашей сети
const char *password = "1q2w3e4r";  //Пароль сети


WiFiUDP ntpUDP;  //Объект ntp
//UDPNTPClient timeClient(ntpUDP);//
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);  //Так-же можно более детально настроить пул и задержку передачи.

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);  //Wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();               //Запускаем клиент времени
  timeClient.setTimeOffset(10800);  //Указываем смещение по времени от Гринвича. Москва GMT+3 => 60*60*3 = 10800
}



void loop() {
  timeClient.update();  //Обновляем дату
    Serial.print("Час: ");
    Serial.println(timeClient.getHours());
    Serial.print("Минута: " );
    Serial.println(timeClient.getMinutes());
    Serial.print("Секунда: ");
    Serial.println(timeClient.getSeconds());
  delay(1000);
}