void parsing() {  // Парсинг входящих сообщений с линии RS485


  if (Serial.available() > 0) {
    //M = Serial.parseInt();
    //Serial.println(M);

    incomingByte = Serial.read();
    if (incomingByte == 'D')  //работаем с дверью
    { step = 1; }
    if (incomingByte == 'C')  //управление разъёмом
    { step = 2; }
    if (incomingByte == 'L')  //подсветка
    { step = 3; }
    if (incomingByte == 'U')  //usb
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
          Serial.print(" Door, ");
         // door(m);
          break;
        case 2:  // разъём
          Serial.print(" USB - ");
          if (m == 0) u_off();
          if (m == 1) u_c();
          if (m == 2) u_l();
          //move_to(m);
          //move_mode(m);
          break;
        case 3:  // свет
          Serial.println(" Свет! ");
          break;
        case 4:  // тензо
          // if (m == 1) {
          //   for (int i = 0; i < 10;) {
          //     if (sensor.available()) {
          //       t_sum += sensor.read();
          //       i++;
          //     }
          //   }
          //   t_now = t_sum / 109;  //109
          //   t_sum = 0;
          //   Serial.print("H");
          //   Serial.println(t_now);
          // } else {
          //   sensor.tare();
          //   Serial.println("tare");
          // }
          break;
        case 5:  // сброс
          // Serial.print(" Reset ");
          // sensor.tare();
          // delay(500);
          // Serial.print("tare ");
          // home = false;
          // speed = 800;
          // move_to(-10000);
          break;
        default:
          break;
      }
      step = 0;
    }
  }
}