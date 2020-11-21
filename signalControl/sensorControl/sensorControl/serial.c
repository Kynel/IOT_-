#include <stdio.h> 
#include <stdint.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h> 

#include <wiringPi.h>
#include <wiringSerial.h>

          
char device[]= "/dev/ttyACM0";
          
int fd;
unsigned long baud = 115200;
unsigned long time=0;

         
int main(void);
void loop(void);
void setup(void);

void setup(){

    printf("load Temperature information..\n");
    fflush(stdout);

     if ((fd = serialOpen (device, baud)) < 0){
          fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
          exit(1); //error
     }
          
     if (wiringPiSetup () == -1){
            fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
            exit(1); //error
      }

}

void loop() {
      if(serialDataAvail (fd)){
            char newChar = serialGetchar (fd);
            printf("%c", newChar);
            fflush(stdout);
      }

}

int main(){
     setup();
 
     while(1) loop();
 
     return 0;
 }



