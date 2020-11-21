#include <stdio.h>
#include <wiringPi.h>

int main(void) {
	const int green_car = 4;
	const int yellow_car =5;
	const int red_car = 6;
	
	const int green_person =7;
	const int red_person=8;

	const int buzzerpin=23;
	
	wiringPiSetup();

	pinMode(green_car ,OUTPUT);
	pinMode(yellow_car,OUTPUT);
	pinMode(red_car, OUTPUT);

	pinMode(green_person, OUTPUT);
	pinMode(red_person, OUTPUT);

	pinMode(buzzerpin, PWM_OUTPUT);

	while(1) {
		digitalWrite(green_car, HIGH); 
		digitalWrite(red_person, HIGH);
	
		delay(2700);

		digitalWrite(green_car, LOW);
		digitalWrite(red_person,LOW);

		digitalWrite(yellow_car, HIGH);
		
		delay(1200);
		
		int flag=0;
		digitalWrite(yellow_car, LOW);
		//buzzer음 내야 함
		if(flag==0) {
			pwmSetMode(PWM_MODE_MS);
			pwmSetClock(19);
			pwmSetRange(1000000);
			pwmWrite(buzzerpin, 1000000/2);
			flag=1;
		}
		digitalWrite(red_car, HIGH);
		digitalWrite(green_person, HIGH);

		delay(2700);

		pwmWrite(buzzerpin, 0);

		digitalWrite(red_car,LOW);
		digitalWrite(green_person, LOW);

		digitalWrite(yellow_car,LOW);
		
	}
	return 0;
}
		

