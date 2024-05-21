#include <Wire.h>
#include "PCF8575.h"
#include <DS18B20.h>
#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
int s = 0;
int ss = 0;
bool flag = false;
bool flag2 = false;
unsigned long timing;
PCF8575 PCF(0x20);
EspSoftwareSerial::UART mdbPort;
WiFiUDP ntpUDP; //Объект ntp 
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000); //Так-же можно более детально настроить пул и задержку передачи.
 

void setup() {
  Serial.begin(115200);
  //while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("Start");
  //  test_buzz();
 // test_relay();
  //test_od();
  //test_pcf();
  //  test_temp();
  test_modem();
  //test_mdb();
  //test_cctalk();

}

void loop() {

}

void printHex(char X) {
  if (X < 16) { Serial.print("0"); }
  Serial.print(X, HEX);
}


void printHexSting(char* buf, int len) {
  while (len--) {
    printHex(*buf++);
  }
  Serial.println("");
}

void test_out(int min) {

  pinMode(min, OUTPUT);
  digitalWrite(min, HIGH);
  delay(500);
  //digitalWrite(i, LOW);
}

void test_relay() {
  Serial.println("Relay test\n");
  test_out(4);
}

void test_od() {
  Serial.println("OD test\n");
  test_out(6);
  test_out(10);
}

void test_pcf() {
  PCF8575 PCF(0x20);
  if (PCF.begin(16, 15)) {
    Serial.print("Input num value:\n");

    Serial.println("OUT");
    for (int i = 0; i < 8; i++) {
      Serial.print(i);
      Serial.print(" ");
      Serial.println(PCF.read(i));
      PCF.write(1, 0);
    }
    Serial.println("in");
    for (int i = 0; i < 8; i++) {
      Serial.print(i);
      Serial.print(" ");
      Serial.println(PCF.read(i + 8));
    }
  }
}

void test_temp() {
  Serial.print("Temp.sensors test\n");
  DS18B20 ds1(10);
  DS18B20 ds2(11);
  DS18B20 ds3(12);

  if (ds1.selectNext()) {
    Serial.print("T1: ");
    Serial.println(ds1.getTempC());
  }

  if (ds2.selectNext()) {
    Serial.print("T2: ");
    Serial.println(ds2.getTempC());
  }

  if (ds3.selectNext()) {
    Serial.print("T3: ");
    Serial.println(ds3.getTempC());
  }
}

void showSerialData() {
  while (Serial2.available()) {
    Serial.print((char)Serial2.read());
  }
  Serial.println("");
}

void sendCommand(const char* command) {
  Serial2.println(command);
  delay(1000);
  showSerialData();
}



void sendGetRequest() {
  //Check if the module is responsive, expected value OK
  sendCommand("ATI");
  sendCommand("AT+CSQ");
  sendCommand("AT+GMM");
  //close or turn off network connection in case it was left open, expected value OK
  sendCommand("AT+CIPSHUT");
  // close GPRS context bearer in case it was left open, expected value OK
  sendCommand("AT+SAPBR=0,1");
  // open GPRS context establish GPRS connection
  sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  //Set the Access Point Name (APN) for the network provider
  //change this apn value for your SIM card
  sendCommand("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"");
  sendCommand("AT+SAPBR=3,1,\"USER\",\"mts\"");
  sendCommand("AT+SAPBR=3,1,\"PWD\",\"mts\"");
  //open GPRS context bearer
  //initiate HTTP requestt5t5n
  sendCommand("AT+SAPBR=1,1");
  sendCommand("AT+SAPBR=4,1");
  sendCommand("AT+SAPBR=2,1");

 
  timeClient.begin(); //Запускаем клиент времени  
  timeClient.setTimeOffset(10800); //Указываем смещение по времени от Гринвича. Москва GMT+3 => 60*60*3 = 10800
  timeClient.update(); //Обновляем дату    
  Serial.println(timeClient.getFormattedTime()); //Выводим время
  delay(1000);


  sendCommand("AT+HTTPINIT");
  sendCommand("AT+HTTPSSL=1");
  sendCommand("AT+HTTPPARA=\"REDIR\",1");
  //set parameters for http session, HTTP context identifier
  sendCommand("AT+HTTPPARA=\"CID\",1");
  //Change the URL from google.com to the server you want to reach
  sendCommand("AT+HTTPPARA=\"URL\",\"www.google.com\"");
  //Initiate the HTTP GET request, send http request to specified URL
  sendCommand("AT+HTTPACTION=0");
  // Wait for the response (adjust the delay as needed)
  Serial.println("Waiting...");
  delay(20000);
  // Read the HTTP response, normally contains status code 200 if successful
  sendCommand("AT+HTTPREAD");
  //Terminate the HTTP service
  sendCommand("AT+HTTPTERM");
  //close or turn off network connection
  sendCommand("AT+CIPSHUT");
  // close GPRS context bearer
  sendCommand("AT+SAPBR=0,1");
}

void postRequest(String jsonToSend) {
  //Example format of JSON:
  // String jsonToSend="{\"uploadedAt\":\"2023-06-26T20:18:22.826Z\",\"data\":[{\"unit\":\"C\",\"reading\":31}]}";
  sendCommand("ATI");
  sendCommand("AT+CIPSHUT");
  delay(500);
  showSerialData();
  sendCommand("AT+SAPBR=0,1");
  delay(2000);
  showSerialData();
  sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  //Set the Access Point Name (APN) for the network provider
  //change this apn value for your SIM card
  sendCommand("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"");
  sendCommand("AT+SAPBR=3,1,\"USER\",\"mts\"");
  sendCommand("AT+SAPBR=3,1,\"PWD\",\"mts\"");
  //open GPRS context bearer
  //initiate HTTP request
  sendCommand("AT+SAPBR=1,1");
  sendCommand("AT+SAPBR=4,1");
  sendCommand("AT+SAPBR=2,1");
  sendCommand("AT+HTTPINIT");
  sendCommand("AT+HTTPSSL=1");
  sendCommand("AT+HTTPPARA=\"REDIR\",1");
  //set parameters for http session, HTTP context identifier
  sendCommand("AT+HTTPPARA=\"CID\",1");
  //Change the URL from google.com to the server you want to reach
  sendCommand("AT+HTTPPARA=\"URL\",\"http://validate.jsontest.com\"");
  showSerialData();
  sendCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  showSerialData();
  sendCommand(("AT+HTTPDATA=" + String(jsonToSend.length()) + ",20000").c_str());
  delay(6000);
  Serial2.println(jsonToSend);
  delay(16000);
  showSerialData();
  sendCommand("AT+HTTPACTION=1");
  delay(20000);
  showSerialData();
  sendCommand("AT+HTTPREAD");
  showSerialData();
  sendCommand("AT+HTTPTERM");
  showSerialData();
  sendCommand("AT+CIPSHUT");
  showSerialData();
}

void test_modem() {
  Serial.print("Modem test\n");
  pinMode(2, OUTPUT);     //power pin
  digitalWrite(2, HIGH);  //power on
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  delay(5000);

  while (Serial2.available()) {
    Serial.print((char)Serial2.read());  //Serial.print((char)Serial2.read());
  }

  sendGetRequest();
  postRequest("{\"time\":\"2023-06-26T20:18:22.826Z\",\"data\":[{\"unit\":\"C\",\"reading\":31}]}"); //untested

  Serial.print("Modem poweroff\n");
  Serial2.println("AT+CPOWD=0");

  delay(500);
  digitalWrite(2, LOW);  //power off
  return;
}



//void test_mdb() {

// Serial.print("MDB test");

//  char inBuf[128];
//  mdbPort.begin(9600, SWSERIAL_8S1, 21, 14, false);

//  mdbPort.write(0x31, PARITY_MARK);
// mdbPort.write(0x31);
// delay(50);
// int avail = mdbPort.available();
// mdbPort.read(inBuf, avail);
// printHexSting(inBuf, avail);
//}

void test_cctalk() {
  Serial.print("CCTALK test");
  // crc-16
  //[destination] [length] [crc] [header] [data] [crc]
  Serial1.begin(9600, SERIAL_8N1, 41, 42, false);
  char inBuf[128];
  char tx_len = 5;

  //char buf[tx_len] = {0x28, 0, 0xb6, 0xfe, 0x21};
  //char buf[tx_len] = {0x28, 0, 49, 159, 93};
  char buf[tx_len] = { 0x28, 00, 0xdd, 245, 0x90 };

  while (true) {
    Serial1.write(buf, tx_len);

    delay(50);
    int avail = Serial1.available();
    Serial1.read(inBuf, avail);
    if (avail > tx_len) {
      printHexSting(&inBuf[tx_len], avail - tx_len);
    }

    delay(100);
  }
}

void test_buzz() {
  Serial.print("Buzzer test\n");
  ledcSetup(0, 3200, 8);
  ledcAttachPin(13, 0);
  ledcWrite(0, 127);
  delay(200);
  ledcDetachPin(13);
}
