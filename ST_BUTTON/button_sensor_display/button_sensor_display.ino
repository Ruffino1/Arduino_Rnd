int button = 0;
int pars_button = 10;

#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define SIG A6



void setup() {
  Serial.begin(9600);

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
    readMux(0);
    pars_button = Button_read();
    if (pars_button != 10) {
      Serial.print("S,");
      Serial.print(i);
      Serial.print(",");
      Serial.print("P,");
      Serial.print(pars_button);
      Serial.println("/n");
    }
  }
  delay(50);
}

int Button_read() {
  button = analogRead(6);
  if (button > 770) {
    return 0;
  } else if (button < 770 && button >= 710) {
    return 1;
  } else if (button < 710 && button >= 670) {
    return 2;
  } else if (button < 670 && button >= 620) {
    return 3;
  } else if (button < 620 && button >= 550) {
    return 4;
  } else if (button < 550 && button >= 420) {
    return 5;
  } else if (button < 420 && button >= 300) {
    return 6;
  } else if (button < 300 && button >= 150) {
    return 7;
  } else if (button < 150 && button >= 70) {
    return 8;
  } else if (button < 70 && button >= 15) {
    return 9;
  } else if (button == 0) {
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