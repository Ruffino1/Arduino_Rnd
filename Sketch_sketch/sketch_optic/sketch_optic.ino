#define optic_in 2

int count = 0;
bool opt_state;
bool flag = false;

void setup() 
{
  pinMode(optic_in, INPUT);
  Serial.begin(9600);
} 

void loop()
{
  opt_state = digitalRead(optic_in);

  if (!opt_state && !flag){
    flag = true;
    count ++;
    Serial.println(count);
  }
  if (opt_state && flag){
    flag = false;
  }
  
}
