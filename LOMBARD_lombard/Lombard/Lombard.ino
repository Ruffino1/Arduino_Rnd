#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define PIN_STRIP_1 5  // Pin for LED strip 1
#define PIN_STRIP_2 6  // Pin for LED strip 2
#define PIN_STRIP_3 7  // Pin for LED strip 3

#define NUM_LEDS_STRIP_1 6 // Number of LEDs in strip 1
#define NUM_LEDS_STRIP_2 8 // Number of LEDs in strip 2
#define NUM_LEDS_STRIP_3 6 // Number of LEDs in strip 3

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_STRIP_1, PIN_STRIP_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS_STRIP_2, PIN_STRIP_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS_STRIP_3, PIN_STRIP_3, NEO_GRB + NEO_KHZ800);

bool strip1On = false;
bool strip2On = false;
bool strip3On = false;

const int button2Pin = 2; 
const int acs712Pin = A0; // ACS712 analog input pin

const float thresholdCurrent = 1.1; 
const int servoSpeed = 30; 

Servo myServo;
bool clockwise = true;
char state = 0;

void setup() {
  pinMode(button2Pin, INPUT_PULLUP);
  myServo.attach(9);
  Serial.begin(9600);
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip1.show();
  strip2.show();
  strip3.show();
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    if (command == '1') {
      strip1On = !strip1On; // Toggle strip 1 state
      if (strip1On) {
        turnOnStrip(strip1, NUM_LEDS_STRIP_1);
      } else {
        turnOffStrip(strip1, NUM_LEDS_STRIP_1);
      }
    } else if (command == '2') {
      strip2On = !strip2On; // Toggle strip 2 state
      if (strip2On) {
        turnOnStrip(strip2, NUM_LEDS_STRIP_2);
      } else {
        turnOffStrip(strip2, NUM_LEDS_STRIP_2);
      }
    } else if (command == '3') {
      strip3On = !strip3On; // Toggle strip 3 state
      if (strip3On) {
        turnOnStrip(strip3, NUM_LEDS_STRIP_3);
      } else {
        turnOffStrip(strip3, NUM_LEDS_STRIP_3);
      }
    }
  
  else if (command == 'o') {
      rotateClockwise();
      Serial.println("Command 'o'");
      if (getCurrent() < thresholdCurrent) {
        state = 0;
        Serial.print("Open, state = ");
        Serial.println(state);
      }
    }

  else if (command == 'c') {
      rotateAntiClockwise();
      Serial.println("Command 'c'");
      if (getCurrent() < thresholdCurrent) {
        state = 1;
        Serial.print("Close, state = ");
        Serial.println(state);
      }
    }

  }
  }
   else if (digitalRead(button2Pin) == LOW || getCurrent() > thresholdCurrent) {
    stopServo();
    state = 2;
    if (!clockwise) {
      clockwise = true;
    } else {
      clockwise = false;
    }
    Serial.print("Stop, state = ");
    Serial.println(state);
  }

  float current = getCurrent(); // Get current value
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  delay(10); // Add a small delay for stability
}

void rotateClockwise() {
  clockwise = true;
  myServo.write(180); // Rotate clockwise
  delay(servoSpeed);
}

void rotateAntiClockwise() {
  clockwise = false;
  myServo.write(0); // Rotate anti-clockwise
  delay(servoSpeed);
}

void stopServo() {
  myServo.write(90); // Stop the servo
  delay(servoSpeed);
}

float getCurrent() {
  int sensorValue = analogRead(acs712Pin);
  float voltage = sensorValue * (5.0 / 1023.0); // Convert analog reading to voltage
  float current = (voltage - 2.5) / 0.185; // ACS712 sensitivity is 185mV/A
  return abs(current);
}

void turnOnStrip(Adafruit_NeoPixel &strip, int numLEDs) {
  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255)); // Set color to white (you can change this)
  }
  strip.show();
}

void turnOffStrip(Adafruit_NeoPixel &strip, int numLEDs) {
  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Set color to off
  }
  strip.show();
}
