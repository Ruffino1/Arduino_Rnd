void parsing() {  // Парсинг входящих сообщений с линии RS485


  if (Serial.available() > 0) {

    //M = Serial.parseInt();
    //Serial.println(M);
    incomingByte = Serial.read();
    //Serial.print(incomingByte);

    if (incomingByte == 'D')  //работаем с дверью
    { step = 1; }
    if (incomingByte == 'B')  //управление барабаном
    { step = 2; }
    if (incomingByte == 'L')  //подсветка
    { step = 3; }
    if (incomingByte == 'T')  //тензо
    { step = 4; }
    if (incomingByte == 'R')  //сброс
    { step = 5; }
    if (incomingByte == ',')  //разделитель значений
    { string_convert = ""; }
    if (incomingByte != ',' && incomingByte != '\n' && incomingByte != 'D' && incomingByte != 'B' && incomingByte != 'L' && incomingByte != 'T' && incomingByte != 'R') {
      string_convert += incomingByte;  //Serial.println(string_convert);
    }

    //if (!P_select && Cena) { Cen = string_convert.toInt(); }
    if (incomingByte == '\n')  //тут конец строки
    {
      long m = string_convert.toInt();
      //Serial.print("step = ");
      //Serial.println(step);
      switch (step) {
        case 0:  // ожидание

          break;
        case 1:  // дверь
                 // Serial.print(" Door, ");
          door(m);
          break;
        case 2:  // барабан
          //Serial.print(" Baraban, ");
          //speed = 100;
          //move_to(m);
          move_mode(m);
          break;
        case 3:  // свет
          //step = 0;
          led(m);
          break;
        case 4:  // тензо
          if (m == 1) {
            Serial.print("H");
            Serial.println((int)get_value_tenzo());
          } else {
            get_zero_tenzo();
            Serial.println(0);
          }
          break;
        case 5:  // сброс
          Serial.print(" Reset ");
          led(0);
          get_zero_tenzo();
          delay(500);
          Serial.print("tare ");
          home = false;
          speed = 800;
          move_to(-10000);
          break;
      }
      step = 0;
    }
  }
}