//Пины управления шаговиком
#define STEP_PIN         2
#define DIR_PIN          3
#define ENABLE_PIN       4

//Пин кнопки
#define start_button 6

//Здесь можно настроить время поворота и паузы
#define move_forward_time 3000    //время прямого хода в мсек
#define move_back_time 3000       //время обратного хода в мсек
#define pause_time 4000           //время паузы в мсек
#define frequency 2250            //Время между импульсами в мксек. 1000 мксек = 1 мсек = 0.001 сек. Частота следования импульсов 1/0.001 = 1 кГц, 
//Не рекомендуется устанавливать время меньше 100 мсек, т.к. частота будет 10 кГц

//Таймер для millis()
uint32_t timer = 0;

//Логический флаг для рабочего режима
bool flag = 0;

char incomingByte = '0';

void setup() {
    //Подтягиваем кнопку к питанию
  Serial.begin(9600);
  pinMode(STEP_PIN   , OUTPUT);       //Настраиваем пины управления
  pinMode(DIR_PIN    , OUTPUT);
  pinMode(ENABLE_PIN , OUTPUT);

  digitalWrite(ENABLE_PIN , HIGH);    //Выключаем мотор, чтобы не грелся
}

void loop () {
  incomingByte = Serial.read();
  if(incomingByte == 'S') {       //Если нажали на кнопку
    digitalWrite(ENABLE_PIN , LOW);       //Включаем мотор
    flag = 1;                             //Активируем флаг рабочего режима
    timer = millis();   
    Serial.println("я работаю");                 //Запускаем таймер
  }

  if (flag) {                                 //Если флаг активирован
    digitalWrite(DIR_PIN    , HIGH);         //Задаем направление вращения

    //    for (int i = 0; i < 200; i++) {          //Выполняем нужное число шагов 200*16 = 3200 шагов оборот
    //      digitalWrite(STEP_PIN    , HIGH);
    //      delayMicroseconds(frequency);
    //      digitalWrite(STEP_PIN    , LOW);
    //    }
    //    flag = 0;

    do {                                      //Крутим движок move_forward_time мсек
      digitalWrite(STEP_PIN    , HIGH);
      delayMicroseconds(frequency);
      digitalWrite(STEP_PIN    , LOW);
    } while (millis() - timer < move_forward_time);

    delay(pause_time);                        //Пауза pause_time мсек
    timer = millis();                         //Снова запускаем таймер
    digitalWrite(DIR_PIN    , LOW);        //Крутим движок move_back_time мсек
    do {
      digitalWrite(STEP_PIN    , HIGH);
      delayMicroseconds(frequency);
      digitalWrite(STEP_PIN    , LOW);
    } while (millis() - timer < move_back_time);
    flag = 0;                                 //Гасим флаг
    digitalWrite(ENABLE_PIN , HIGH);        //Выключаем движок, чтобы не грелся
  }
}
