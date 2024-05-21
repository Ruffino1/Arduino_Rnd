// библиотеки для работы с OLED экраном Arduino IDE
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
int com = 10;
int bak_2 = 0;
bool flag = 1;
String str;
//github
Adafruit_SSD1306 display(128, 32, &Wire, 4);  // указываем размер экрана в пикселях

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // указываем адрес устройства на шине
  display.clearDisplay();
  display.setTextSize(2, 2);            // указываем размер шрифта
  display.setTextColor(SSD1306_WHITE);  // указываем цвет надписи

  display.setCursor(35, 10);
  display.println("IGOR");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.drawRect(0, 1, 22, display.height() - 2 * 1, SSD1306_WHITE);
  display.drawRect(106, 1, 22, display.height() - 2 * 1, SSD1306_WHITE);
  display.drawCircle(display.width() / 2, 16, 30, SSD1306_WHITE);
  display.display();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(3);  // Normal 1:1 pixel scale
  display.setCursor(48, 6);
  display.println(0);
  display.display();  // Update screen with each newly-drawn rectangle
  delay(1);
  delay(2000);  // очищаем экран
}

void loop() {
  if (Serial.available() && flag) {
    com = Serial.parseInt();
    str = Serial.readStringUntil('\n');
    Serial.print(str);
  }
  test();
}

void test() {
  switch (com) {
    case 0:

      break;
    case 1:
      flag = 0;
      if (Serial.available()) {
        bak_2 = Serial.parseInt();
      }
      if (bak_2 == 0) {
        display.drawRect(0, 1, 22, display.height() - 2, SSD1306_INVERSE);
        display.display();
        delay(400);
      } else {
        display.drawRect(0, 1, 22, display.height() - 2, SSD1306_WHITE);
        display.fillRect(1, 2, 20, display.height() - 3, SSD1306_BLACK);
        display.display();
        delay(1);
        for (int i = 30; i > bak_2; i -= 1) {
          display.drawLine(1, i, 20, i, SSD1306_WHITE);
          display.display();
          delay(100);
        }
        bak_2 = 0;
        flag = 1;
        com = 0;
      }

      break;
    case 2:
      flag = 0;
      if (Serial.available()) {
        bak_2 = Serial.parseInt();
      }
      if (bak_2 == 0) {
        display.drawRect(106, 1, 22, display.height() - 2, SSD1306_INVERSE);
        display.display();
        delay(400);
      } else {
        display.drawRect(106, 1, 22, display.height() - 2, SSD1306_WHITE);
        display.fillRect(107, 2, 20, display.height() - 3, SSD1306_BLACK);
        display.display();
        delay(1);
        for (int i = 30; i > bak_2; i -= 1) {
          display.drawLine(107, i, 127, i, SSD1306_WHITE);
          display.display();
          delay(100);
        }
        bak_2 = 0;
        flag = 1;
        com = 0;
      }
      break;
    case 3:
      flag = 0;
      if (Serial.available()) {
        bak_2 = Serial.parseInt();
      }
      if (bak_2 == 0) {
        display.setTextSize(3);  // Normal 1:1 pixel scale
        display.setCursor(48, 6);
        display.setTextColor(SSD1306_INVERSE);
        display.println(0);
        display.display();
        delay(400);
      } else {
        for (int i = 0; i <= bak_2; i++) {
          display.setTextColor(SSD1306_WHITE);
          display.setTextSize(3);  // Normal 1:1 pixel scale
          display.setCursor(48, 6);
          display.println(i);
          display.display();
          delay(100);
          if (i != bak_2) {
            display.setTextColor(SSD1306_BLACK);
            display.setTextSize(3);  // Normal 1:1 pixel scale
            display.setCursor(48, 6);
            display.println(i);
            display.display();
          }
        }
        bak_2 = 0;
        flag = 1;
        com = 0;
      }
      break;
  }
}
