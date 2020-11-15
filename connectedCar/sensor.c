#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#define BTNPIN 3
#define debounceTime 100
#define TRIG 23
#define ECHO 24

volatile int state = LOW;

void buttonPressed(void){
	static unsigned long lastTime = 0;
	unsigned long now = millis();
	if((now-lastTime) > debounceTime)
		state = !state;
	lastTime = now;
}

int main(void){
	int start_time, end_time;
	float distance;

	wiringPiSetup();

	pinMode(BTNPIN, INPUT);
	pinMode(TRIG,OUTPUT);
	pinMode(ECHO,INPUT);

	wiringPiISR(BTNPIN, INT_EDGE_RISING, buttonPressed);

	while(1){
		delay(100);
		if(state){
			digitalWrite(TRIG, LOW);
			delay(500);
			digitalWrite(TRIG, HIGH);
			delayMicroseconds(10);
			digitalWrite(TRIG,LOW);
			while(digitalRead(ECHO) == 0);
			start_time = micros();
			while(digitalRead(ECHO) == 1);
			end_time = micros();

			distance = (end_time - start_time) /29. / 2. ;

			printf("distance %.2f cm\n", distance);
			
		}
	}

	return 0;
}

