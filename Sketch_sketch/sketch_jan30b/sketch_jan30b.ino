#include "senser.h"
#include "delay.h"

uint16_t Time;
unsigned int Test1;  // Debug test
unsigned int test2;
extern unsigned char ucrxfinish1;  // complete the logo position
extern unsigned char ucRxFinish2;
Extern UNSIGNED CHAR USART2_UCRXData[100];  // Data buffer
extern unsigned char usart3_ucRxData[100];
extern unsigned char CMD_1[8];
extern unsigned char CMD_2[8];

void Delay(void) {
  unsigned int i = 0;
  while (++i < 1000);
}

void GetData(void) {
  unsigned int uiData1;
  unsigned int uiData2;
  USART2_Send(CMD_1, 8);
  USART3_Send(CMD_2, 8);
  Time = 0;
  while ((Time < 100) && ((ucRxFinish1 == 0) || (ucRxFinish2 == 0))) {
    Delay();
  }
  if (ucRxFinish1 && ucRxFinish2) {
    uiData1 = usart2_ucRxData[3] << 8 | usart2_ucRxData[4];
    uiData2 = usart3_ucRxData[3] << 8 | usart3_ucRxData[4];
    test1 = uiData1;
    test2 = uiData2;
    if ((uiData1 >= 265 && uiData1 <= 325) && (uiData2 >= 173 && uiData2 <= 203)) {
      USART1_SendByte('1');
    } else if ((uiData1 >= 302 && uiData1 <= 325) && (uiData2 >= 249 && uiData2 <= 265)) {
      USART1_SendByte('2');
    } else if ((uiData1 >= 329 && uiData1 <= 345) && (uiData2 >= 280 && uiData2 <= 292)) {
      USART1_SendByte('3');
    } else if ((uiData1 >= 350 && uiData1 <= 360) && (uiData2 >= 296 && uiData2 <= 303)) {
      USART1_SendByte('4');
    } else
      USART1_SendByte('0');
    //		printf("Sensor%d Distance:%d\r\n",usart2_ucRxData[0],uiData1);
    //		printf("Sensor%d Distance:%d\r\n",usart3_ucRxData[0],uiData2);
  }
  ucRxFinish1 = 0;
  ucRxFinish2 = 0;
}