const int APIN= A0;
const int LPIN= 10;

float temperature;  
int reading;  
const int lm35Pin = A2;
int time;


void setup() {
  Serial.begin(115200);
  Serial.println("setup");
}

 
void loop() {
  time = millis();
  int aValue = analogRead(APIN);

  if(aValue > 655)
    analogWrite(LPIN, aValue/4);
  else 
    analogWrite(LPIN, 0);

  //Serial.println(aValue);

 
  reading = analogRead(lm35Pin);
  temperature = (5.0 * reading * 100) /1024;

    if(time % 3000 ==0 ){
      delay(100);
      Serial.println(temperature);
    }

}
