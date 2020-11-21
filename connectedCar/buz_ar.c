#include <stdio.h> //for printf
#include <stdint.h> //uint8_t definitions
#include <stdlib.h> //for exit(int);
#include <string.h> //for errno
#include <errno.h> //error output
 
//wiring Pi
#include <wiringPi.h>
#include <wiringSerial.h>

#define BTNPIN 3
#define debounceTime 100
#define TRIG 23
#define ECHO 24

volatile int state = LOW;
int buz = 26;

char device[]= "/dev/ttyACM0";
 
// filedescriptor
int fd;
unsigned long baud = 9600;
unsigned long time=0;

void buttonPressed(void){
	static unsigned long lastTime = 0;
	unsigned long now = millis();
	if((now-lastTime) > debounceTime)
		state = !state;
 lastTime = now;
}


int main()
{
  printf("%s \n", "Raspberry Startup!");
  fflush(stdout);
  
	int start_time, end_time;
	int distance;
	char buf[20];

	wiringPiSetup();

	pinMode(BTNPIN, INPUT);
	pinMode(TRIG,OUTPUT);
	pinMode(ECHO,INPUT);
	pinMode(buz,OUTPUT);

	wiringPiISR(BTNPIN, INT_EDGE_RISING, buttonPressed);

  //get filedescriptor
  if ((fd = serialOpen (device, baud)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    exit(1); //error
  }
  
  //setup GPIO in wiringPi mode
  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    exit(1); //error
  }
 
  while(1)
  {
		delay(100);
     // Pong every 3 seconds
     if(state)
     {
			 digitalWrite(TRIG, LOW);
			 delay(50);
			 digitalWrite(TRIG,HIGH);
			 delayMicroseconds(10);
			 digitalWrite(TRIG, LOW);
			 while(digitalRead(ECHO) == 0);
			 start_time = micros();
			 while(digitalRead(ECHO) == 1);
			 end_time = micros();

			 distance = (end_time - start_time) / 29 / 2;

			 printf("%d\n",distance);
			 sprintf(buf, "%d",distance);
			 serialPuts (fd, buf);
			 serialPuts(fd, "\n");
			 
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


