
#include <WiFi.h>
#include <ESP32Ping.h>


const char *ssid = ""; //Название вашей сети
const char *password = ""; //Пароль сети


void setup(){  
  Serial.begin(115200); 
  delay(10);
  WiFi.begin(ssid, password); //Wifi  
  while (WiFi.status() != WL_CONNECTED) {    
    delay (500);    
    Serial.print (".");  
  }
  for (){
    
  }  
}

void loop() {  
  timeClient.update(); //Обновляем дату    
  Serial.println(timeClient.getFormattedTime()); //Выводим время
  delay(1000);
}