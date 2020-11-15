#include <PinChangeInterrupt.h>

// 초음파센서1
const int TPIN1 = A5;
const int EPIN1 = A4;

// 초음파센서2
const int TPIN2 = A3;
const int EPIN2 = A2;

// LED
// const int RLED;
const int GLED = 13;

// 8x8 dot matrix(HL-M1588BRG): row -> 나중에 shift register를 이용할 예정
const int R1 = 6; // 24
const int R2 = 7; // 23
const int R3 = 8; // 22 
const int R4 = 9; // 21
const int R5 = 5; // 4
const int R6 = 4; // 3
const int R7 = 3; // 2
const int R8 = 2; // 1

// shift register (74HC595)
const int dataPin = 10;
const int latchPin = 11;
const int clockPin = 12;

unsigned long echo_duration1 = 0;
unsigned long echo_duration2 = 0;

bool pass1 = false;
bool pass2 = false;

double startTime, endTime;

// 8x8 dot matrix digits data
byte digit_row[7] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02}; // 반복문을 돌면서 한 줄씩 내려가도록
// 1의 자리
byte digit_col0[10][7] = {
  {0x0F, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0F}, // 0
  {0x06, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x0F}, // 1
  {0x0F, 0x01, 0x01, 0x0F, 0x08, 0x08, 0x0F}, // 2
  {0x0F, 0x01, 0x01, 0x0F, 0x01, 0x01, 0x0F}, // 3
  {0x09, 0x09, 0x09, 0x0F, 0x01, 0x01, 0x01}, // 4
  {0x0F, 0x08, 0x08, 0x0F, 0x01, 0x01, 0x0F}, // 5
  {0x0F, 0x08, 0x08, 0x0F, 0x09, 0x09, 0x0F}, // 6
  {0x0F, 0x09, 0x09, 0x09, 0x01, 0x01, 0x01}, // 7
  {0x0F, 0x09, 0x09, 0x0F, 0x09, 0x09, 0x0F}, // 8
  {0x0F, 0x09, 0x09, 0x0F, 0x01, 0x01, 0x01}  // 9
};
// 10의 자리
byte digit_col1[10][7];

void echoISR1() {
  static unsigned long echo_begin = 0;
  static unsigned long echo_end = 0;

  unsigned int echo_pin_state = digitalRead(EPIN1);
  if (echo_pin_state == HIGH) {
    echo_begin = micros();
  } else {
    echo_end = micros();
    echo_duration1 = echo_end - echo_begin;
  }
}

void echoISR2() {
  static unsigned long echo_begin = 0;
  static unsigned long echo_end = 0;

  unsigned int echo_pin_state = digitalRead(EPIN2);
  if (echo_pin_state == HIGH) {
    echo_begin = micros();
  } else {
    echo_end = micros();
    echo_duration2 = echo_end - echo_begin;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(TPIN1, OUTPUT);
  pinMode(EPIN1, INPUT);
  pinMode(TPIN2, OUTPUT);
  pinMode(EPIN2, INPUT);

//  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  Serial.begin(115200);

  attachPCINT(digitalPinToPCINT(EPIN1), echoISR1, CHANGE);
  attachPCINT(digitalPinToPCINT(EPIN2), echoISR2, CHANGE);

  // 10의 자리 숫자 만들기
  for(int i=0; i<10; i++) {
    for(int j=0; j<7; j++) {
      digit_col1[i][j] = digit_col0[i][j] << 4;
    }
  }
}

void dotMatrixClear() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, 0x00);
  digitalWrite(latchPin, HIGH);
  digitalWrite(R1,LOW);
  digitalWrite(R2,LOW);
  digitalWrite(R3,LOW);
  digitalWrite(R4,LOW);
  digitalWrite(R5,LOW);
  digitalWrite(R6,LOW);
  digitalWrite(R7,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(echo_duration1 == 0) {
    digitalWrite(TPIN1, LOW);
    delayMicroseconds(2);
    digitalWrite(TPIN1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TPIN1, LOW);
  } else {
    long distance1 = echo_duration1 / 58;
    Serial.print("distance1: ");
    Serial.print(distance1);
    Serial.print(" cm, ");
    if(distance1 < 5) {
      pass1 = true;
      startTime = millis();
    }
    echo_duration1 = 0;
  }
  if(echo_duration2 == 0) {
    digitalWrite(TPIN2, LOW);
    delayMicroseconds(2);
    digitalWrite(TPIN2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TPIN2, LOW);
  } else {
    long distance2 = echo_duration2 / 58;
    Serial.print("distance2: ");
    Serial.print(distance2);
    Serial.println(" cm");
    if(distance2 < 5) {
      pass2 = true;
      endTime = millis();
    }
    echo_duration2 = 0;
  }
  
  if(pass1 && pass2) {
    pass1 = false;
    pass2 = false;
    
    double seconds = ((endTime - startTime)/1000);
    double carSpeed = 5.5 / seconds;

//    Serial.print("# seconds: ");
//    Serial.print(seconds);
//    Serial.println(" s");
    Serial.print("# speed: ");
    Serial.print(carSpeed);
    Serial.println(" cm/s");

    int num1 = (int)carSpeed / 10;
    int num2 = (int)carSpeed % 10;

    Serial.print(num1);
    Serial.print("  ");
    Serial.println(num2);

    digitalWrite(GLED, HIGH);
    
    for(int j=0; j<=500; j++) { // 숫자를 반복해서 보여줌 (긴 시간동안 숫자 확인 가능)
      for(int i=0; i<7; i++) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 0xFF ^ (digit_col1[num1][i] | digit_col0[num2][i]));
      digitalWrite(latchPin, HIGH);

      digitalWrite(R1,0x80 & digit_row[i]);
      digitalWrite(R2,0x40 & digit_row[i]);
      digitalWrite(R3,0x20 & digit_row[i]);
      digitalWrite(R4,0x10 & digit_row[i]);
      digitalWrite(R5,0x08 & digit_row[i]);
      digitalWrite(R6,0x04 & digit_row[i]);
      digitalWrite(R7,0x02 & digit_row[i]);
      delay(1);
      }
    }

    digitalWrite(GLED, LOW);
    dotMatrixClear();
  }
}
