#include "HX711.h"               // Подключаем библиотеку HX711
HX711 scale;                     // Создаём объект scale 

#define DT  15                    // Указываем номер вывода, к которому подключен вывод DT  датчика HX711
#define SCK 14                    // Указываем номер вывода, к которому подключен вывод SCK датчика HX711

float weight_of_standard = 359;  // Указываем эталонный вес
float units = 0.035274;          // Указываем коэффициент для перевода из унций в граммы
float calibration_factor = 0;    // Создаём переменную для значения калибровочного коэффициента

void setup()
{
  Serial.begin(57600);           // Инициируем работу с последовательным портом на скорости 57600 бод
  scale.begin(DT, SCK);          // Инициируем работу с платой HX711
  scale.set_scale();             // Не калибруем полученные значения
  scale.tare();                  // Обнуляем вес на весах (тарируем)
  Serial.println("You have 10 seconds to set your known load");    // Выводим в монитор порта текст 
  delay(10000);                                                    // Ждём 10 секунд
  Serial.print("calibration factor: ");                            // Выводим текст в монитор последовательного порта
  calibration_factor = scale.get_units(10) / (weight_of_standard / units);      // Считываем значение с тензодатчика
  Serial.println(calibration_factor);                                           // Выводим в монитор порта значение корректирующего коэффициента
}

void loop() {}

