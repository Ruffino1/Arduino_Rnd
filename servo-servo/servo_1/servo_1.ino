int servoPin = 9; // сигнальный провод от серво на порт 9
int val;

void setup()
{
	pinMode(servoPin, OUTPUT);
	Serial.begin(9600);
	Serial.println("Servo is ready");
}

void loop()
{
    // convert number 0 to 9 to corresponding 0-180 degree angle
	val = Serial.read();
	if (val >= '0' && val <= '9')
	{
		val = val - '0'; // convert to numerical variable
		val = val * (180 / 9); // convert number to angle
		Serial.print("moving servo to ");
		Serial.print(val, DEC);
		Serial.println();
		// giving the servo time 
		//to rotate to commanded position
		for (int i = 0; i <= 50; i++) 
		{
			servoPulse(servoPin, val);
		}
	}
}

// define a servo pulse function
void servoPulse(int pin, int angle)
{
	// convert angle to 500-2480 pulse width
	int pulseWidth = (angle * 11) + 500; 
	digitalWrite(pin, HIGH); // set the level of servo pin as high
	delayMicroseconds(pulseWidth); // delay microsecond of pulse width
	digitalWrite(pin, LOW); // set the level of servo pin as low
	delay(20 - pulseWidth / 1000);
}
