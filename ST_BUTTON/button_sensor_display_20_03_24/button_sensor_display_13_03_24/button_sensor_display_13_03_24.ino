#include <math.h>
int button = 0;
int pars_button = 10;
unsigned long for_button = 0;
unsigned long for_button_old = 0;
int inc = 0;
int blz_button = 0;
int pin = 0;

#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define SIG A0

void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // остальную геометрию смотри в документации
}
void loop() {
  parsing();
}

int parsing() {
  for (int i = 0; i < 2; i++) {
    pin = i;
    pars_button = Button_read();
    if (pars_button != 10) {
      Serial.print("S,");
      Serial.print(i + 1);
      Serial.print(",");
      Serial.println(pars_button + 1);
      pars_button = 10;
      for_button = 0;
      delay(1000);
    }
  }
}


int Button_read() {
    for (int i = 0; i < 100; i++) {
      button = analogRead(pin);
      for_button_old += button;
      //delay(1);
    }
    for_button_old /= 100;
    for_button_old /= 5;

  if (for_button_old > 7 ) {
  
    for (int i = 0; i < 100; i++) {
      button = analogRead(pin);
      for_button += button;
      //delay(1);
    }
    for_button /= 100;
    for_button /= 5;
    Serial.println(for_button);
  }

  if (for_button >= 10 && for_button <= 16) {
    return 9;
  } else if (for_button >= 23 && for_button <= 37) {
    return 8;
  } else if (for_button >= 60 && for_button <= 80) {
    return 7;
  } else if (for_button >= 90 && for_button <= 105) {
    return 6;
  } else if (for_button >= 125 && for_button <= 147) {
    return 5;
  } else if (for_button >= 150 && for_button <= 164) {
    return 4;
  } else if (for_button >= 167 && for_button <= 178) {
    return 3;
  } else if (for_button >= 180 && for_button <= 186) {
    return 2;
  } else if (for_button >= 189 && for_button <= 193) {
    return 1;
  } else if (for_button >= 201 && for_button <= 206) {
    return 0;
  } else if (for_button < 7) {
    return 10;
  }
}


int readMux(int channel) {
  int controlPin[] = { S0, S1, S2, S3 };

  int muxChannel[16][4] = {
    { 0, 0, 0, 0 },  //channel 0
    { 1, 0, 0, 0 },  //channel 1
    { 0, 1, 0, 0 },  //channel 2
    { 1, 1, 0, 0 },  //channel 3
    { 0, 0, 1, 0 },  //channel 4
    { 1, 0, 1, 0 },  //channel 5
    { 0, 1, 1, 0 },  //channel 6
    { 1, 1, 1, 0 },  //channel 7
    { 0, 0, 0, 1 },  //channel 8
    { 1, 0, 0, 1 },  //channel 9
    { 0, 1, 0, 1 },  //channel 10
    { 1, 1, 0, 1 },  //channel 11
    { 0, 0, 1, 1 },  //channel 12
    { 1, 0, 1, 1 },  //channel 13
    { 0, 1, 1, 1 },  //channel 14
    { 1, 1, 1, 1 }   //channel 15
  };

  //loop through the 4 sig
  for (int i = 0; i < 4; i++) {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG);

  //return the value
  return val;
}