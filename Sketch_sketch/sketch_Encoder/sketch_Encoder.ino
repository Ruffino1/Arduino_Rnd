#define InB 2
#define InA 3
#define InButton 4

int counter = 0;
int currentState;
int previousState;
int impulse_counter = 0;
bool flag = false;
String encdir ="";


void setup() {
  pinMode (InB,INPUT);
  pinMode (InA,INPUT);
  pinMode (InButton,INPUT);

  Serial.begin(9600);

  previousState = digitalRead(InB);
}

void loop() {
  currentState = digitalRead(InB);
  bool btnState = !digitalRead(InButton);
  if (btnState && !flag) {
    flag = true;
    Serial.println("Привет я кнопка!");
  }
  if (!btnState && flag) {
    flag = false;  
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
      Serial.print(counter);
      Serial.println();
      impulse_counter = 0;
    }
    else{
    impulse_counter ++;
    }
    previousState = currentState;
    
  }
}

