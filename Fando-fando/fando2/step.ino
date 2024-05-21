void move_to(long dist) {
  // Serial.println(dist);
  move(dist - c_step);
}

void move_open() {  // открыть барабан для захвата бутылки
  speed = 100;
  move_to(3100);
}

void move_home() {  // повернуть барабан в начальную позицию
  speed = 400;
  if (c_step < 0) move_to(0);
  else {
    home = false;
    move_to(14000);
  }
}

void Bottle_or_no() {  // осталась ли бытылка в барабане после сброса в бак
  bool while_flag = false;
  bool flag_hand = false;
  // опрашиваем датчик пока тот не выйдет из сработки
  while (!while_flag) {
    if (get_value_tenzo() > 30) {
      if (!digitalRead(in_hand) && !flag_hand) {  // если датчик сработал
        flag_hand = true;
      }
    } else if(!flag_hand) {
      while_flag = true;
      //close_door();
      Serial.end();
      Serial.begin(9600);
      Serial.println("Ready");
    }
    if (flag_hand) {  // ждем пока датчик выйдет из сработки
      if (digitalRead(in_hand)) {
        delay(2000);
        if (get_value_tenzo() < 30) {

          while_flag = true;
          //close_door();
          Serial.end();
          Serial.begin(9600);
          Serial.println("Ready");
        }
      }
    }


    delay(30);
  }
}

void Open_or_home() {  // если бутылка осталась в барабане после сброса - открыть для ручного изъятия
  if (get_value_tenzo() > 30) {
    move_open();
    Serial.println("E3");
    //open_door();

  } else {
    move_home();
    Serial.println("Ready");
  }
}

void move_mode(int mode) {  // команды для барабана
  switch (mode) {
    case 0:  // открыть
      move_open();
      Serial.println("Ready");
      break;
    case 1:  // контейнер левый
      move_open();
      delay(1300);
      speed = 100;
      move_to(-3500);
      delay(1300);
      Open_or_home();
      break;
    case 2:  // контейнер правый
      move_open();
      delay(1300);
      speed = 100;
      move_to(7700);
      delay(1300);
      Open_or_home();
      //Serial.println("Ready");
      break;
    case 3:  // сканирование
      speed = 400;
      move_to(-7600);
      Serial.println("Ready");
      break;
    case 4:  // домой
      move_home();
      Serial.println("Ready");
      break;
    case 5:  // поиск дома
      home = false;
      speed = 800;
      move_to(-10000);
      Serial.println("Ready");
      break;
    case 6:  //осталась ли бытылка в барабане после сброса в бак
      Bottle_or_no();
      break;
  }
}

bool r = false;
int min_speed = 1000;
int f_speed = min_speed;
int acc_l = 0;

void move(long dist) {

  // Не готовы, пока не закончим
  ready = false;

  // Направление
  if (dist > 0) {
    dir = false;
    digitalWrite(out_dir, LOW);
  } else {
    dir = true;
    digitalWrite(out_dir, HIGH);
  }

  long steps_left = abs(dist);  // надо пройти столько шагов

  long f_steps = steps_left;  // помним в начале, сколько надо было пройти

  acc_l = (min_speed - speed) / acc;
  // Serial.print("ACC "); Serial.println(acc_l);
  // if (f_steps / 2 > acc_l) acc_l = f_steps / 2;
  // Serial.print("ACC_2 ");Serial.println(acc_l);


  // пока не прошли нужное количество:
  while (steps_left > 0) {

    // ограничения скорости
    if (f_speed < speed) f_speed = speed;
    if (f_speed > min_speed) f_speed = min_speed;

    if (f_steps - steps_left < acc_l) f_speed -= acc;
    if (steps_left < acc_l) f_speed += acc;

    // В самом начале сбрасываем ноль в домашней позиции
    if (!digitalRead(in_zero) && !home) {
      Serial.println("Ready");  //Serial.println(c_step);
      home = true;
      c_step = 0;
      steps_left = 0;
    }

    unsigned long now = micros();


    delayMicroseconds(f_speed);  //80 - max 1200 - min
    r = !r;
    if (r) {
      digitalWrite(out_step, HIGH);
      steps_left--;
      dir ? c_step-- : c_step++;
    } else {
      digitalWrite(out_step, LOW);
    }
  }
  ready = true;
}
