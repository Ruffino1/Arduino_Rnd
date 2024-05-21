#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>


const char *ssid = "Xperia X_a157"; //Название вашей сети
const char *password = "ruffino12"; //Пароль сети
WiFiUDP ntpUDP; //Объект ntp 
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000); //Так-же можно более детально настроить пул и задержку передачи.

void setup(){  
  Serial.begin(115200);  
  WiFi.begin(ssid, password); //Wifi  
  while (WiFi.status() != WL_CONNECTED) {    
    delay (500);    
    Serial.print (".");  
  }  
  timeClient.begin(); //Запускаем клиент времени  
  timeClient.setTimeOffset(10800); //Указываем смещение по времени от Гринвича. Москва GMT+3 => 60*60*3 = 10800
}

void loop() {  
  timeClient.update(); //Обновляем дату    
  Serial.println(timeClient.getFormattedTime()); //Выводим время
  delay(1000);
}
