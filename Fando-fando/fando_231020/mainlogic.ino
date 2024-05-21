void mainlogic() {
  /*
  Сообщения:
  E1   - датчик на входе, во время движения
  E2   - механическая остановка барабана
  E3   - бутылка застряла в барабане?
  E4   - переполнен бак 1
  E5   - переполнен бак 2
  E6   - неверная команда
  E7   - превышен вес
  E8   - датчик присутствия, при отсутствии веса.

  IN   - сработал датчик на входе
  R    - барабан на исходной/тензо сброшены
  Hxx  - данные с тензо
  M0/1 - данные от индукционных
  */
  /* все команды:
  0  - ожидание
  1  - инициализация
  2  - обработка датчика входа
  3  - проверка, что внутри что-то есть
  4  - взвесили и сканируем

  10 - чтение 0 го датчика
  11 - чтение тензо
  12 - сброс тензо
  13 - опрос индуктивных
  14 - опрос мультиплексора
  15 - опрос баков
  16 - обработка превышения веса
  20 - барабан в 0
  21 - барабан скан
  22 - захват бутылки 1
  23 - опрокидывание 1
  24 - захват бутылки 2
  25 - опрокидывание 2
  26 - открыть барабан
  */

  if (alarm) {
    led_all(strip.Color(200, 0, 0, 100));
    led_ind(strip.Color(250, 0, 0, 0));
  }

  switch (step) {
    case 0:  // ожидание
      if (!alarm) {
        //led_all(strip.Color(0, 0, 0, 100));
        led_ind(strip.Color(50, 150, 0, 0));
        m_select(0);
      }
      if (alarm && !digitalRead(in_p) && step_in > 0) {
        alarm = false;
        led_all(strip.Color(0, 0, 0, 150));
        step = 27;
      }
      if (!alarm && input_b) {
        if (digitalRead(in_p)) {
          t_old = t_now;
          input_b = false;
          step = 2;
        }
      }
      if (!alarm && t_now > 150) step = 16;
      tenzo_f_read();
      break;

    case 1:  // инициализация
      led_all(strip.Color(0, 0, 0, 100));
      led_ind(strip.Color(0, 250, 10, 0));
      speed = 10;
      h_move(-185570);
      if (ready) {
        delay(700);
        enc = 0;
        step = 12;
        err = 0;
      }
      break;

    case 2:  // обработка датчика входа (сработал)
      led_all(strip.Color(0, 0, 0, 200));
      led_ind(strip.Color(0, 250, 200, 50));
      Serial.println("IN");
      //tenzo_read(1);  // сбрасываем 0
      step = 3;
      break;

    case 3:  // обработка датчика входа (перестал) взвешиваем
      if (!digitalRead(in_p)) {
        delay(500);
        tenzo_f_read();
        if (t_now > t_old + 8) step = 4;
        else if (digitalRead(up_p)) {
          Serial.println("E8");
          step = 0;
        } else {
          Serial.println("R");
          step = 0;
        }
      }
      break;

    case 4:  // Отправляем данные и сканируем
      // delay(500);
      tenzo_f_read();
      Serial.print("H");
      Serial.println(t_now);
      step = 0;  //21
      break;

    case 10:  // чтение 0го датчика
      Serial.print("Выбран вход - ");
      Serial.println(step);
      //m_select(0);
      //input = false;
      //delay(10);
      if (digitalRead(up_p)) Serial.println(" * ");
      else Serial.println(" - ");
      step = 0;
      break;

    case 11:  // чтение тензо

      //strip.setPixelColor(2, strip.Color(200, 0, 200));
      //strip.show();

      //tenzo_read();
      //Serial.print("H");
      //Serial.println(t_now);
      tenzo_f_read();
      //Serial.print("F");
      Serial.println(t_now);
      step = 0;
      break;

    case 12:  // сброс тензо

      //strip.setPixelColor(1, strip.Color(255, 255, 0));
      //strip.show();

      tenzo_read(1);
      delay(500);
      tenzo_f_read();
      t_old = t_now;
      Serial.println("R");
      //Serial.println(t_now);
      step = 0;
      break;

    case 13:  // опрос индуктивных
      Serial.print("Магнитится? - ");
      if (digitalRead(m_p)) Serial.println(" нет ");
      else Serial.println(" Да! ");
      step = 0;
      break;

    case 14:  // опрос мультиплексора
      Serial.print("Опрос портов :");
      //Serial.println(step);
      for (int i = 0; i <= 15; i++) {
        m_select(i);
        //input = false;
        delay(100);
        if (digitalRead(in_p)) {
          switch (i) {
            case 0:
              Serial.print("Вторжение! ");
              break;
            case 1:
              Serial.print("контейнер 1 ");
              break;
            case 2:
              Serial.print("контейнер 2 ");
              break;
            case 3:
              Serial.print("на барабане ");
              break;
            case 4:
              Serial.print("4- ");
              break;
            case 5:
              Serial.print("5- ");
              break;
            case 6:
              Serial.print("6- ");
              break;
            case 7:
              Serial.print("7- ");
              break;
          }
          Serial.print(i);
          Serial.println("  * ");
        } else {
          // Serial.print(i);
          // Serial.println(" . ");
        }
      }
      m_select(0);
      step = 0;
      break;

    case 15:  // опрос баков
      if (ready) {
        delay(500);
        if (!digitalRead(t1_p)) Serial.println("E4");
        if (!digitalRead(t2_p)) Serial.println("E5");
        tenzo_f_read();
        t_old = t_now;  // сбрасываем 0
        Serial.println("R");
        step = 0;
      }
      break;

    case 16:  // слишком большой вес
      Serial.println("E7");
      alarm = true;
      step = 0;
      break;

    case 20:  // исходная позиция барабана (+ заполненость баков)
      alarm = false;
      led_all(strip.Color(0, 0, 0, 150));
      led_ind(strip.Color(100, 200, 0, 0));
      speed = 40;
      if (c_step > 2000) {  //8000
        move_to(5700);      //4750 после сброса левого
        c_step = 0;
      } else {
        move_to(10);
      }  //10
      if (ready) {
        speed = 20;
        home = false;
        move(-18000);
        step = 15;
      }
      break;

    case 21:  // сканирование
      led_all(strip.Color(0, 0, 0, 250));
      led_ind(strip.Color(250, 100, 0, 200));
      //strip.setPixelColor(0, strip.Color(255, 155, 0));
      //strip.show();

      speed = 8;
      move_to(-3300);  // -2625
      if (ready && !alarm) {
        if (digitalRead(m_p)) Serial.println("M0");
        else Serial.println("M1");
        delay(500);
        step = 20;
      }
      if (alarm) step = 0;
      break;

    case 22:  // захват 1
      if (!alarm) {
        led_ind(strip.Color(150, 150, 0, 0));
      }
      speed = 30;
      move_to(1140);  //1050
      if (ready && !alarm) {
        delay(300);
        step = 23;
      } else if (alarm) step = 0;
      break;

    case 23:  // сброс 1
      speed = 40;
      move_to(-1100);  //-3500
      if (ready && !alarm) {
        speed = 40;
        move(20);  //200
        delay(600);
        tenzo_f_read();                      // Serial.print(t_old); Serial.print(" - "); Serial.println(t_now);
        if (t_old + 10 > t_now && err < 3)   // если выпала
        {
          //Serial.println("Выпала!");
          step = 20;
        } else {
          step = 23;
          err++;
          if (err > 3) {  // ну не выпадает!
            Serial.println("E3");
            alarm = true;
            err = 0;
            //step = 0;
          }
        }
      } else if (alarm)
        step = 22;
      break;

    case 24:  // захват 2
      if (!alarm) {
        led_ind(strip.Color(150, 150, 0, 0));
      }
      speed = 30;
      move_to(1140);
      if (ready && !alarm) {
        delay(300);
        step = 25;
      } else if (alarm) {
        //Serial.println("Err");
        step = 0;
      }
      break;

    case 25:          // сброс 2
      move_to(3500);  // 11600
      if (ready && !alarm) {
        speed = 40;
        move(-20);
        delay(500);
        tenzo_f_read();
        if (t_old + 10 > t_now && err < 3)  // если выпала
        {
          err = 0;
          step = 20;
        } else {  // ещё потрясём
          //Serial.print(t_old); Serial.print(" - "); Serial.println(t_now);
          step = 25;
          err++;
          if (err > 3) {  // ну не выпадает!
            Serial.println("E3");
            alarm = true;
            err = 0;
            //step = 0;
          }
        }
      } else if (alarm)
        step = 24;
      break;

    case 26:  // открыть барабан
      speed = 40;
      move_to(1050);  //4200
      if (ready && !alarm) {
        step = 0;
      }
      break;

    case 27:                              // снять с паузы
      //d_in ? s_in = s_in : s_in = -s_in;  // в какую сторону заканчиваем вращение
      //move(s_in);
      if (ready && !alarm) {
       //s_in = 0;
        step = step_in;
        Serial.println(step_in);
      }
      break;

    case 28:  // под тест
      Serial.print("Тест - ");
      speed = 40;
      enc = 0;   //707
      move(35);  //18557
      if (ready) {
        Serial.print("Энкодер - ");
        Serial.println(enc);
        step = 0;
      }
      break;

    case 29:  // где мы?
      Serial.print("Текущая позиция - ");
      Serial.println(step);
      Serial.println(c_step);
      step = 0;
      break;

    default:
      Serial.println("E6");
      step = 0;
      break;
  }
}
/*
void initialization() {
  speed = 10;
  h_move(-185570);
  delay(700);
  sensor.tare();
  enc = 0;
}
*/

void loop() {
  if (!home) step = 1;

  if (Serial.available()) { // && step == 0
    step = Serial.parseInt();
    str = Serial.readStringUntil('\n');
    Serial.print(str);
  }
  // tenzo_read();

  if (ready) { mainlogic(); }

  /*
  strip.setPixelColor(0, strip.Color(255, 255, 0));	strip.show();
	
•red: (255, 0, 0) 
•green: (0, 255, 0) 
•blue: (0, 0, 255) 
•cyan: (0, 255, 255) 
•purple: (255, 0, 255) 
•yellow: (255, 255, 0) 
•white: (255, 255, 255) 
•black (off): (0, 0, 0) 
*/
}
