#include <GyverHX711.h>
GyverHX711 sensor(0, 1, HX_GAIN64_A);
// HX_GAIN128_A - канал А усиление 128
// HX_GAIN32_B - канал B усиление 32
// HX_GAIN64_A - канал А усиление 64
int cl = 0;
int sum = 0;
int sum0 = 0;
bool empt = false;
long time;
bool t = false;

void setup() {
  Serial.begin(9600);
  
  // если тарирование при первом запуске -
  // нужно выждать готовность датчика
  delay(500);
  sensor.tare();    // калибровка нуля
  //sensor.sleepMode(true);		// выключить датчик
  //sensor.sleepMode(false);	// включить датчик
}

void loop() {
  // чтение только по доступности! if available
  if (sensor.available()) {
    if(cl<10)
    {
      sum+=sensor.read();
      cl++;
    }
    else 
    {
      sum = sum / 109.5;
      Serial.println(sum);
      if(sum >= sum0 + 7 || sum <= sum0 - 7)
      {
        if(!t)
       {
          time = millis();
          t = true;
       }

          if(time + 600 < millis() && (sum >= sum0 + 4 ) && t)
          {
            Serial.print("Привет! Мой вес = ");
            Serial.println(sum);
            sum0=sum;
            empt = true;
            t = false;
          }
          if(sum <= sum0 - 4){
            sum0=sum;
            t = false;
          }
      }
      else sum0 = sum;

      if ((sum <= 6 && sum >= -6) && empt == true) {
        Serial.println("Я пустой(");
        empt = false;
      }
      sum=0;
      cl=0;
    }
  }
  if (Serial.available()) 
  {
    if (Serial.parseInt() == 0)
    {
      sensor.tare();
      delay (500);
      Serial.println("Ok");
    }
    else Serial.println("ой");
    //Serial.read()=="0"? sensor.tare(): Serial.println("ой");
  } 
}