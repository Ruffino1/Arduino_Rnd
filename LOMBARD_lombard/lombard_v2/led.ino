// Функция для плавного включения цвета
void fadeIn() {
  for(int brightness = 0; brightness <= 255; brightness++) {
    strip.fill(strip.Color(brightness, brightness, brightness, 0), 0, strip.numPixels());
    strip.show();
    delay(1);
  }
}

// Функция для плавного выключения цвета
void fadeOut() {
  for(int brightness = 255; brightness >= 0; brightness--) {
    strip.fill(strip.Color(brightness, brightness, brightness, 0), 0, strip.numPixels());
    strip.show();
    delay(1);
  }
}