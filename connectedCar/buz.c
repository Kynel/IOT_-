#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#define BTNPIN 3
#define debounceTime 100
#define TRIG 23
#define ECHO 24

volatile int state = LOW;
int buz = 26;

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
	pinMode(buz,OUTPUT);

	wiringPiISR(BTNPIN, INT_EDGE_RISING, buttonPressed);

	while(1){
		delay(100);
		if(state){
			digitalWrite(TRIG, LOW);
			delay(50);
			digitalWrite(TRIG, HIGH);
			delayMicroseconds(10);
			digitalWrite(TRIG,LOW);
			while(digitalRead(ECHO) == 0);
			start_time = micros();
			while(digitalRead(ECHO) == 1);
			end_time = micros();
			
			distance = (end_time - start_time) /29. / 2. ;

			printf("distance %.2f cm\n", distance);

			
			if(distance >= 30 && distance < 40){
				digitalWrite(buz,HIGH);
				delay(300);
				digitalWrite(buz, LOW);
				delay(300);
			}else if (distance >= 20 && distance <30){
				digitalWrite(buz,HIGH);
				delay(300);
				digitalWrite(buz, LOW);
				delay(200);
			}else if(distance >= 10 && distance < 20){
				digitalWrite(buz,HIGH);
				delay(300);
				digitalWrite(buz, LOW);
				delay(100);
			}else if(distance < 10){
				digitalWrite(buz,HIGH);
				delay(300);
				digitalWrite(buz, LOW);
				delay(50);
			}
		}
	}

	return 0;
}

