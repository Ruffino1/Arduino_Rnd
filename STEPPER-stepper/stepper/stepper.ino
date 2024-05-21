const uint8_t  STEP = 2;                     // Пин выбора для мультплексора
const uint8_t  DIR = 3;
int SPEED = 300; //max 30 min 10
int step = 0;
bool ready = true;
char incomingByte = '0';

void setup() {
  Serial.begin(9600);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
}

void loop() {
  if (ready) { mainlogic();}
}

void mainlogic () // основная логика программы
{
  switch(step)
  {
    case(0):
    if (Serial.available() > 0) step = 1;
    break;

    case(1):
    incomingByte = Serial.read();
    if(incomingByte == 'S')
      {
      Serial.println("go");
      run(); step = 0;
      }
    break;

    case(2): 
    if (ready) step = 3;  
    break;
  }
}

void run()
{
  ready = false;
  long del = 20000;
  int dist = 10000;
  int ASpeed = 0;
  long time_delay = micros();

  for (int i = 0; i<dist; i++)
  {
    if (i<dist/4 && i < SPEED) 
    {
      int acc = (dist/4)-(dist/4)+i;
      ASpeed=acc;
    }
    if (i>dist/4 ) 
    {
      int acc = dist-i;
      if (acc < SPEED) ASpeed=acc;
    }
    digitalWrite(STEP, HIGH);
    delayMicroseconds(del/ASpeed);
    digitalWrite(STEP, LOW);
    delayMicroseconds(del/ASpeed);
  }
  Serial.println("ready"); ready = true;
}