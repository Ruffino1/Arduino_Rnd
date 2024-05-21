// Данные с INA219
int16_t readRegister(uint8_t reg) {
  Wire.beginTransmission(INA219_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom((int)INA219_ADDRESS, (int)2);
  return (Wire.read() << 8) | Wire.read();
}

// Функция для получения силы тока в мА
float getCurrent_mA() {
  int16_t raw_current = readRegister(0x04);  // Регистр сырых данных о токе
  return raw_current * 0.1;                  // Коэффициент преобразования для получения значения в мА
}

