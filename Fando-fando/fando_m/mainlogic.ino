void mainlogic() {
  /*
  Сообщения:
  E1   - датчик на входе, во время движения
  E2   - механическая остановка барабана
  E3   - бутылка застряла в барабане?
  E4   - переполнен бак 1
  E5   - переполнен бак 2
  E6   - неверная команда, сработал датчик руки
  E7   - превышен вес

  IN   - сработал датчик на входе
  R    - барабан на исходной
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
    colorWipe(strip.Color(0, 255, 0)); // red
  }

  switch (step) {

    case 0:  // ожидание
      if (!alarm) {
        colorWipe(strip.Color(155, 0, 0)); //
        // m_select(0);
      }
      if (!alarm && digitalRead(in_p)) { step = 30; }
      //Serial.println(t_now);
      //delay(50);
      if (!alarm && t_now > 200) step = 16;
      break;

    case 1:  // инициализация
      // strip.setPixelColor(0, strip.Color(200, 0, 200));
      // strip.show();
      colorWipe(strip.Color(0, 255, 0)); //
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
      tenzo_read(1); // сбрасываем 0 
      tenzo_f_read();
      t_old = t_now; // 
      colorWipe(strip.Color(0, 255, 0)); //
      Serial.println("IN");
      step = 3;
      break;

    case 3:  // обработка датчика входа (перестал) взвешиваем
      if (!digitalRead(in_p)) {
        delay(500);
        tenzo_f_read();
        colorWipe(strip.Color(255, 255, 0)); //
        if (t_now > 8) step = 4;  // добавь сравнение с t_old
        else step = 0;
      }
      break;

    case 4:  // Отправляем данные и сканируем
      // delay(500);
      tenzo_f_read();
      Serial.print("H");
      Serial.println(t_now);
      step = 0; //21
      break;

    case 10:  // чтение датчиков на барабане
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
      colorWipe(strip.Color(0, 255, 255)); //
      //tenzo_read();
      //Serial.print("H");
      //Serial.println(t_now);
      tenzo_f_read();
      //Serial.print("F");
      Serial.println(t_now);
      step = 0;
      break;

    case 12:  // сброс тензо
      colorWipe(strip.Color(0, 150, 0)); //
      tenzo_read(1);
      delay(500);
      tenzo_f_read();
      t_old = t_now;
      Serial.println("Вес сброшен");
      step = 0;
      break;

    case 13:  // опрос индуктивных
      Serial.print("Магнитится? - ");
      if (analogRead(m_p)>=50) Serial.println(" нет "); //digitalRead(m_p)
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
        else (Serial.println("Ok"));
        //if (digitalRead(t1_p) && digitalRead(t2_p)) Serial.println("норм ");
        tenzo_f_read();
        t_old = t_now;  // сбрасываем 0
        step = 0;
      }
      break;

    case 16:  // слишком большой вес
      tenzo_f_read();
      if(t_now>=50) {
      Serial.println("E7");
      colorWipe(strip.Color(100, 55, 0)); //
      alarm = true;
      step = 0;
      break;}
      else Serial.println("Вес в норме");
      // strip.setPixelColor(0, strip.Color(255, 0, 0));
      // strip.show();
      

    case 20:  // исходная позиция барабана (+ заполненость баков)
      colorWipe(strip.Color(0, 5, 100)); //
      speed = 40;
      alarm = false;
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
        Serial.println("R");
        step = 15;
      }
      break;

    case 21:  // сканирование
      colorWipe(strip.Color(100, 255, 0)); // orange
      speed = 8;
      move_to(-3400);  // -2625
      if (ready && !alarm) {
        if (analogRead(m_p)>=50) Serial.println("M0"); //
        else Serial.println("M1");
        delay(500);
        step = 20;
      }
      if (alarm) step = 0;
      break;

    case 22:  // захват 1
      if (!alarm) {
        colorWipe(strip.Color(150, 255, 0)); //
      }
      speed = 30;
      move_to(1180);  //1050
      if (ready && !alarm) {
        delay(300);
        if (!digitalRead(up_p))
          step = 23;
      } else if (alarm) step = 0;
      break;

    case 23:  // сброс 1

    if (!digitalRead(t1_p)) {
    ready = false; 
    Serial.println("E4");
    step = 0;
    }
      // //  Serial.print("Бак 1 - ");
      // //  Serial.println(step);
    if (ready && !alarm) {
      colorWipe(strip.Color(255, 155, 0));
      speed = 40;
    move_to(-1100);  //-3500
        speed = 40;
        move(20);  //200
        delay(600);
        tenzo_f_read();                     // Serial.print(t_old); Serial.print(" - "); Serial.println(t_now);
        if (t_old + 8 >= t_now && err < 3)  // если выпала
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
          }
        }
      } else if (alarm)
        step = 22;
      ready = true;
      break;

    case 24:  // захват 2
      if (!alarm) {
        colorWipe(strip.Color(155, 255, 0)); // yellow
      }
      speed = 30;
      move_to(1180);
      if (ready && !alarm) {
        delay(300);
        if (!digitalRead(up_p))
          step = 25;
      } else if (alarm) step = 0;
      break;

    case 25:  // сброс 2
    
    if (!digitalRead(t2_p)) {
      ready = false; 
      Serial.println("E5");
      step = 0;
      }
      
    if (ready && !alarm) {
      colorWipe(strip.Color(155, 155, 0)); // yellow
      move_to(3500);  // 11600
    speed = 40;
        move(-20);
        delay(500);
        tenzo_read();
        if (t_old + 8 >= t_now && err < 3)  // если выпала
        {
          err = 0;
          step = 20;
        } else {  // ещё потрясём
          step = 25;
          err++;
          if (err > 3) {  // ну не выпадает!
            Serial.println("E3");
            alarm = true;
            err = 0;
          }
        }
      } else if (alarm)
        step = 24;
      ready = true;
      break;

    case 26:  // открыть барабан
      speed = 40;
      move_to(1050);  //4200
      if (ready && !alarm) {
        step = 0;
      }
      break;

    case 27:  // под тест
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

  if (Serial.available() && step == 0) {
    step = Serial.parseInt();
    str = Serial.readStringUntil('\n');
    Serial.print(str);
  }
  // tenzo_read();

  if (ready) { mainlogic(); }

  /*
  strip.setPixelColor(0, strip.Color(255, 255, 0));	strip.show();
	
•
• orange(150, 255, 0)
• red(0, 255, 0)
• blue(0, 0, 255)
• green(255, 0, 0)

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