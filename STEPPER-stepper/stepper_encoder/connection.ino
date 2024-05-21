//encoder
#define InB 2
#define InA 3
#define InButton 4

int counter = 0;
int currentState;
int previousState;
int impulse_counter = 0;
bool flag = false;
String encdir ="";
//stepper
const uint8_t  STEP = 2;              
const uint8_t  DIR = 3;
int SPEED = 300; //max 30 min 10
int step = 0;
bool ready = true;
char incomingByte = '0';

void setup() {
  //stepper
  Serial.begin(9600);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  //encoder
  pinMode (InB,INPUT);
  pinMode (InA,INPUT);
  pinMode (InButton,INPUT);

  Serial.begin(9600);

  previousState = digitalRead(InB);
}