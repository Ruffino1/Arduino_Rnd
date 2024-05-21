#define MOTOR_PUL_PIN 9
#define MOTOR_DIR_PIN 8 
#define MOTOR_ENA_PIN 5


void setup() {
  pinMode (MOTOR_PUL_PIN, OUTPUT);
  pinMode (MOTOR_DIR_PIN, OUTPUT);
  pinMode (MOTOR_ENA_PIN, OUTPUT);

}

void loop() {

// мотор крутится по часовой стрелки
  for (int i=0; i<3200; i++)  
  {
    digitalWrite(MOTOR_DIR_PIN,HIGH);
    digitalWrite(MOTOR_ENA_PIN,HIGH);
    digitalWrite(MOTOR_PUL_PIN,HIGH);
    delayMicroseconds(200);
    digitalWrite(MOTOR_PUL_PIN,LOW);
    delayMicroseconds(200);
  }
 // мотор крутится по против часовой стрелки
  for (int i=0; i<3200; i++)  
  {
    digitalWrite(MOTOR_DIR_PIN,LOW);
    digitalWrite(MOTOR_ENA_PIN,HIGH);
    digitalWrite(MOTOR_PUL_PIN,HIGH);
    delayMicroseconds(200);
    digitalWrite(MOTOR_PUL_PIN,LOW);
    delayMicroseconds(200);
  }
}