void u_c() {
  for (byte c = 0; c <= 20; c++) {
    digitalWrite(out_s_l, HIGH);
    delayMicroseconds(s_close);
    digitalWrite(out_s_l, LOW);
    delay(20 - (s_close / 1000));
  }
  for (byte c = 0; c <= 20; c++) {
    digitalWrite(out_s_c, HIGH);
    delayMicroseconds(s_open);
    digitalWrite(out_s_c, LOW);
    delay(20 - (s_open / 1000));
  }
  Serial.println("C");
}
void u_l() {
  for (byte c = 0; c <= 20; c++) {
    digitalWrite(out_s_c, HIGH);
    delayMicroseconds(s_close);
    digitalWrite(out_s_c, LOW);
    delay(20 - (s_close / 1000));
  }
  for (byte c = 0; c <= 20; c++) {
    digitalWrite(out_s_l, HIGH);
    delayMicroseconds(s_open);
    digitalWrite(out_s_l, LOW);
    delay(20 - (s_open / 1000));
  }
  Serial.println("L");
}
void u_off() {
  for (byte c = 0; c <= 20; c++) {
    digitalWrite(out_s_c, HIGH);
    digitalWrite(out_s_l, HIGH);
    delayMicroseconds(s_close);
    digitalWrite(out_s_c, LOW);    
    digitalWrite(out_s_l, LOW);
    delay(20 - (s_close / 1000));
  }
  Serial.println("Off");
}