void loop() {
  currentState = digitalRead(InB);
  bool btnState = !digitalRead(InButton);
  if (btnState && !flag) {
    flag = true;
    Serial.println("Привет я кнопка!");
  }
  if (!btnState && flag) {
    flag = false;  
  }
  if (currentState != previousState){
    if (impulse_counter == 1) 
    {
      if (digitalRead(InA) != currentState){
        counter --;
      }
      else {
        counter ++;
      }
      Serial.print(counter);
      Serial.println();
      impulse_counter = 0;
    }
    else{
    impulse_counter ++;
    }
    previousState = currentState;
    
  }
}