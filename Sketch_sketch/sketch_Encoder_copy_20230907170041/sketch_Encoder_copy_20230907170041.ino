#define InB 2
#define InA 3
#define InButton 4

int counter = 0;
int currentState;
int currentState_A;
int previousState;
int previousState_A;
int impulse_counter = 0;
bool flag = false;
bool flag_graf = false;
int graf = 0;
bool flag_graf_A = false;
int graf_A = 0;

String encdir ="";


void setup() {
  pinMode (InB,INPUT);
  pinMode (InA,INPUT);
  pinMode (InButton,INPUT);

  Serial.begin(9600);

  previousState = digitalRead(InB);
  previousState_A = digitalRead(InA);
}

void loop() {
  currentState = digitalRead(InB);
  currentState_A = digitalRead(InA);
  bool btnState = !digitalRead(InButton);
  if (btnState && !flag) {
    flag = true;
    Serial.println("Привет я кнопка!");
  }
  if (!btnState && flag) {
    flag = false;  
  }

  if ((previousState == currentState) && graf <= 10) {
    flag_graf = true;
    graf ++;
  }
  else{
    flag_graf = false;
  }
  if (previousState != currentState){
    graf = 0;
  }
//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
  if ((previousState_A == currentState_A) && graf_A <= 10) {
    flag_graf_A = true;
    graf_A ++;
  }
  else{
    flag_graf_A = false;
  }
  if (previousState_A != currentState_A){
    graf_A = 0;
  }

  if (flag_graf || flag_graf_A){
    Serial.print(digitalRead(InB));
    Serial.print(" ");
    Serial.println(digitalRead(InA));
  }
  if (currentState != previousState){
    if (impulse_counter == 1) 
    {
      if (digitalRead(InA) != currentState){
        counter --;
      }
      else {
        counter ++;
      }
      //Serial.print(counter);
      //Serial.println();
      impulse_counter = 0;
    }
    else{
    impulse_counter ++;
    }
    previousState = currentState;
    
  }
  previousState_A = currentState_A;
}
