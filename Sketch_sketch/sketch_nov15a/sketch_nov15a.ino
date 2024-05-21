
   	
#include <SoftwareSerial.h>

char X [7] ={0x5A,0xA5,0x05,0x82,0x50,0x00,0x00};
char R [7] ={0x5A,0xA5,0x05,0x82,0x51,0x00,0x00};
int a = 0;
int s = 0;

SoftwareSerial mySerial(2,3);

void setup() {
  mySerial.begin(115200);
}

void loop() {
    a = a + 1;
    mySerial.write(X , 7);
    mySerial.write(a);
    s = s + 2;
    mySerial.write(R , 7);
    mySerial.write(s);
    delay(1000);

}
