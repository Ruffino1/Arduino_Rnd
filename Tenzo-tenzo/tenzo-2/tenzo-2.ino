#include "HX711.h"
#include <TM1637Display.h>

#define CLK 9
#define DIO 10


const int LOADCELL_DOUT_PIN = A1;
const int LOADCELL_SCK_PIN = A0;

HX711 scale;
TM1637Display display = TM1637Display(A1, A0);

float Calibration_Factor_Of_Load_cell = -71.70;
float U;
float O;
float O_copy;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
  display.clear();
  display.setBrightness(7);
}

void loop() {
  O_copy = O;
  O = 0;
  for (int i = 0; i < 10; i++) {
    scale.set_scale(Calibration_Factor_Of_Load_cell);
    U = scale.get_units();
    if (U < 0) {
      U = 0.00;
    }
    O += U * 0.035274;
    delay(50);
  }
  O = round(O * 100) / 100;
  if (O != O_copy) {
    display.clear();
    display.showNumberDecEx(O, 0b00100000);
    delay(500);
  }
}
