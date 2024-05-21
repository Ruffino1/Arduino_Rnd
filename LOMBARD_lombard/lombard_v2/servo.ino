// функция для ускорения и замедления
void rotateServo_speed(unsigned long StartTime_local, int& step_speed_local, bool& flag_racing_local) {  // (начало отчета времени, шаг скорости, сейчас ускорение?)
  if (millis() - StartTime_local < ACCELERATION_TIME / 4) {                                              // если не прошло больше 1/4 тогда ускоряемся
    if (step_speed_local < PULSE_WIDTH) {                                                                // и если скороть не максимальная
      step_speed_local += ACCELERATION_STEP_racing;                                                      // ускорение
      flag_racing_local = false;                                                                         // сейчас ускорение
    }
  } else if (millis() - StartTime_local > (ACCELERATION_TIME / 2) && millis() - StartTime_local < (ACCELERATION_TIME - 1000)) {  // если не прошло больше 1/2 тогда замедляемся
    if (step_speed_local > 20) {                                                                                                 //и если скороть не минимальная
      step_speed_local -= ACCELERATION_STEP_braking;                                                                             // замедление                                                                       // замедление
    }
  } else if (millis() - StartTime_local > (ACCELERATION_TIME - 1000)) {  // очень плавное закрытие или открытие
    step_speed_local = 100;                                              // скорость сервы
  } else {                                                               // если не ускорение
    flag_racing_local = true;                                            // сейчас не ускорение
  }
}

// основной цикл работы серво
void rotateServoWithAcceleration(int inc) {
  button_flag = true;       // кнопка не нажата
  bool flag_racing = true;  // для более плавного ускорение

  Serial.println();  // очистить буфер

  unsigned long startTime = millis();  // начало отчета времени

  int step_speed = 0;  // шаг для ускорения и замедления сервы

  int current_mA = 0;        // нагрузка
  int current_mA_old = 100;  // нагрузка пердыдушее значение

  while (millis() - startTime < ACCELERATION_TIME) {        // пока не пройдет заданное время
    rotateServo_speed(startTime, step_speed, flag_racing);  // функция для ускорения и замедления, (начало отчета времени, шаг скорости, ускорение)

    if (inc) {             // закрыть
      if (!button_flag) {  //если нажата кнопка
        stopServo();       // остановить серво
        break;
      }
      if (current_mA > current_mA_old + 100) {  //если больше заданого значение остановить и поднять шатер
        stopServo();                            // остановить серво
        delay(50);                              // задержка после остановки
        open((millis() - startTime) * 4.4);     // вернуться в открытое положение
        break;
      }
      rotateServo(NEUTRAL_PULSE_WIDTH + step_speed);  // сделать шаг
    } else {                                          // открыть
      if (current_mA > current_mA_old + 120) {        //если больше заданого значение остановить
        stopServo();                                  // остановить серво
        break;
      }
      rotateServo(NEUTRAL_PULSE_WIDTH - step_speed);  // сделать шаг
    }

    if (current_mA > current_mA_old || !flag_racing) {  // передавать только значения которые больше чем предыдушие пока ускорение
      current_mA_old = current_mA;
    }
    current_mA = getCurrent_mA();  // получить значения амперметра
  }
}

void open(unsigned long Time) {                // плавно поднимает шатер с учетом пройденого времени
  unsigned long StartTime_local = millis();    // начало отчета времени
  while (millis() - StartTime_local < Time) {  // пока не пройдет заданное время
    rotateServo(1400);                         // открыть
  }
}

void rotateServo(int degrees) {  // шаг севро
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(degrees);
  digitalWrite(SERVO_PIN, LOW);
  delay(speed);  // Задержка для плавного вращения
}

void stopServo() {               // стоп севро
  digitalWrite(SERVO_PIN, LOW);  // Отключаем питание сервопривода
}