const uint8_t STEP = 2;
const uint8_t DIR = 3;
int step = 0;
int speed_for = 1000;
int speed_low = 1000;
int step_max = 24000;
int speed_max = 500;
int ex = 10;
bool flag = true;

void setup() {
  Serial.begin(9600);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    step = Serial.parseInt();
  }
  switch (step) {
    case (1):
      flag = true;
      for (int i = 0; i < step_max; i++) {
        if (Serial.available()) {
          if (Serial.parseInt() == 3) {
            digitalWrite(STEP, LOW);
            break;
          }
        }
        run_1(speed_for);
        if (speed_for > speed_max && flag) {
          speed_for -= ex;
        } else {
          flag = false;
        }
        if (i > (step_max - 500) && speed_for != speed_low) {
          speed_for += 5;
        }
      }
      step = 0;
      break;

    case (2):
      flag = true;
      for (int i = 0; i < step_max; i++) {
        if (Serial.available()) {
          if (Serial.parseInt() == 3) {
            digitalWrite(STEP, LOW);
            break;
          }
        }
        run_2(speed_for);
        if (speed_for > speed_max && flag) {
          speed_for -= ex;
        } else {
          flag = false;
        }
        if (i > (step_max - 500) && speed_for != speed_low) {
          speed_for += 5;
        }
      }

      break;
    case (3):

      break;
  }
}

void run_1(int speed) {
  digitalWrite(DIR, HIGH);
  digitalWrite(STEP, HIGH);
  delayMicroseconds(speed);
  digitalWrite(STEP, LOW);
  delayMicroseconds(speed);
}
void run_2(int speed) {
  digitalWrite(DIR, LOW);
  digitalWrite(STEP, HIGH);
  delayMicroseconds(speed);
  digitalWrite(STEP, LOW);
  delayMicroseconds(speed);
}
