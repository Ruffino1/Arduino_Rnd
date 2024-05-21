void rotate_servo() {  // рабочий цикл сервы

  if (!open) {
    for (byte l = 0; l <= 40; l++) {
      digitalWrite(out_servo, HIGH);
      delayMicroseconds(getit);
      digitalWrite(out_servo, LOW);
      delay(20 - (getit / 1000));
      h_servo = true;
      open = true;
    }
    Serial.println("open");
  } else if (open && !h_servo) {
    for (byte c = 0; c <= 20; c++) {
      digitalWrite(out_servo, HIGH);
      delayMicroseconds(d_close);
      digitalWrite(out_servo, LOW);
      delay(20 - (d_close / 1000));
      open = false;
    }
    Serial.println("close");
  } else if (open && h_servo) {
    for (byte c = 0; c <= 20; c++) {
      digitalWrite(out_servo, HIGH);
      delayMicroseconds(half);
      digitalWrite(out_servo, LOW);
      delay(20 - (half / 1000));
      h_servo = false;
    }
    Serial.println("half");
  }
}

void gohome() { //to home
  for (int i = 2; i <= 12; i++) {
    for (byte c = 0; c <= 20; c++) {
      digitalWrite(i, HIGH);
      delayMicroseconds(d_close);
      digitalWrite(i, LOW);
      delay(20 - (d_close / 1000));
    }
  }
}

void open_door() { // открыть дверь
  for (byte l = 0; l <= 40; l++) {
    digitalWrite(out_servo, HIGH);
    delayMicroseconds(getit);
    digitalWrite(out_servo, LOW);
    delay(20 - (getit / 1000));
    h_servo = true;
  }
}

void close_door() { // закрыть дверь
  if (h_servo) {
    //Serial.println(digitalRead(in_door));
    for (byte c = 0; c <= 20; c++) {
      digitalWrite(out_servo, HIGH);
      delayMicroseconds(half);
      digitalWrite(out_servo, LOW);
      delay(20 - (half / 1000));
      h_servo = false;
    }
    //Serial.println("half");
  }
  delay(500);
  //Serial.println(digitalRead(in_door));

  if (!h_servo && !digitalRead(in_door)) {
    for (byte c = 0; c <= 20; c++) {
      digitalWrite(out_servo, HIGH);
      delayMicroseconds(d_close);
      digitalWrite(out_servo, LOW);
      delay(20 - (d_close / 1000));
      open = false;
    }
  } else Serial.println("E1");
}

void door(int stat) { // команды для двери
  if (stat == 0) {
    open_door();
    Serial.println("Ready");  // open
  }
  if (stat == 1) {
    if (digitalRead(in_hand)) {
      close_door();
      Serial.println("Ready");
    } else {
      Serial.println("IN");
    }
  }
}