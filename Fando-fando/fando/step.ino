void move_to(int dist) {
  move(dist - c_step);
}
void move(long dist) {
  bool r = true;
  ready = false;
  if (dist > 0) {
    dir = true;
    d_in = true;
    digitalWrite(dir_p, HIGH);
  } else {
    dir = false;
    d_in = false;
    digitalWrite(dir_p, LOW);
  }
  step_in = step;
  last_time = 0;
  long steps_left = abs(dist);  // надо пройти столько шагов
  long f_steps = steps_left;
  long enc_step = steps_left;  // счётчик для энкодера
  enc = 0;

  // скорость
  int min_speed = 5000;           //10000
  int max_speed = 10000 / speed;  //200
  int step_delay = min_speed;
  // int a_step = (min_speed - max_speed) / (f_steps / acc);
  int a_step = (min_speed - max_speed) / (1000 / acc);

  while (steps_left > 0) {
    if (digitalRead(zero_p) && !home) {
      home = true;
      c_step = 0;
      enc = 0;
      steps_left = 0;
    }

    unsigned long now = micros();
    if (now - last_time >= step_delay) {
      /*
      // замедление по тикам
      if (steps_left < f_steps / acc / 2 && step_delay < min_speed) {
        step_delay += a_step;
      }
      // ускорение по тикам
      if (steps_left > f_steps / acc * (acc - 1) && step_delay > max_speed) {
        step_delay -= a_step;
      }
*/
      // замедление фикс
      if (steps_left < 1000 / acc / 2 && step_delay < min_speed) {
        step_delay += a_step;
      }
      // ускорение фикс
      if (steps_left > 1000 / acc * (acc - 1) && step_delay > max_speed) {
        step_delay -= a_step;
      }
      last_time = now;
      r = !r;
      if (r) {
        digitalWrite(step_p, HIGH);
        encoder();
        steps_left--;
        dir ? c_step++ : c_step--;
      } else {
        digitalWrite(step_p, LOW);
        //проверка вращения

        if (enc_step - 80 >= steps_left)  //177
        {
          enc_step = steps_left;
          if (enc >= 1 && !digitalRead(in_p)) {
            enc = 0;
          } else {
            alarm = true;
            //home = false;
            s_in = steps_left; //запомнили. сколько осталось
            steps_left = 0; // и остановились        
            if (digitalRead(in_p)) Serial.println("E1");
            else Serial.println("E2");
            ready = true;
            step = 0;
          }
        }
      }
    }
  }

  ready = true;
}

void h_move(long dist) {
  bool r = true;
  home = false;
  ready = false;
  if (dist > 0) {  // направление вращения
    dir = true;
    digitalWrite(dir_p, HIGH);
  } else {
    dir = false;
    digitalWrite(dir_p, LOW);
  }
  last_time = 0;           // храним время между тиками
  long steps_left = abs(dist);  // надо пройти столько шагов  * k_enc
  long f_steps = steps_left;    // всего требуется тиков в этом цикле

  // скорость
  int min_speed = 1000;           //10000
  int max_speed = 10000 / speed;  //200
  int step_delay = min_speed;
  int a_step = (min_speed - max_speed) / (f_steps / acc);


  while (steps_left > 0) {
    if (digitalRead(zero_p)) {
      home = true;
      c_step = 0;
      enc = 0;
      steps_left = 0;
    }
    unsigned long now = micros();
    if (now - last_time >= step_delay) {
      // замедление
      if (steps_left < f_steps / acc / 2 && step_delay < min_speed) {
        step_delay += a_step;
      }
      // ускорение
      if (steps_left > f_steps / acc * (acc - 1) && step_delay > max_speed) {
        step_delay -= a_step;
      }
      last_time = now;
      r = !r;
      if (r) {
        digitalWrite(step_p, HIGH);
        steps_left--;
        dir ? c_step-- : c_step++;
      } else {
        digitalWrite(step_p, LOW);
      }
    }
  }
  ready = true;
  delay(50);
  move_to(20);  //100
}