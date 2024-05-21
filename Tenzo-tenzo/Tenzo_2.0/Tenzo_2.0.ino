#include <GyverHX711.h>
GyverHX711 sensor(A1, A0, HX_GAIN64_A);
// HX_GAIN128_A - канал А усиление 128
// HX_GAIN32_B - канал B усиление 32
// HX_GAIN64_A - канал А усиление 64
int t_sum = 0;
int t_now = 0;

void setup() {
  Serial.begin(9600);
  // если тарирование при первом запуске -
  // нужно выждать готовность датчика
  delay(500);
  sensor.tare();    // калибровка нуля
  //sensor.sleepMode(true);		// выключить датчик
  //sensor.sleepMode(false);	// включить датчик
}

void loop() {
  tenzo_f_read();
}

void tenzo_f_read() {
  t_sum = 0;
  for (int i = 0; i < 10;) {
    if (sensor.available()) {
      t_sum += sensor.read();
      i++;
    }
  }
  t_now = t_sum / 109;
  t_sum = 0;
  Serial.println(t_now);
}