#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16 ,2);


String myinfo = "20172256 YHS";
String str="";

int tempSensor = A0;
float temperature =0;
long val=0;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
 
}

void loop() {
  
  str="";
  static unsigned int col =0;
  val = analogRead(tempSensor);
  temperature = (val * 4.8828125);
  
 while(Serial.available()) {
  char temp = Serial.read();
      lcd.clear();
      delay(100);
      str.concat(temp);
      Serial.println(str);
   
 }
 
 char bufferDelete = Serial.read(); //마지막 useless char delete

  lcd.setCursor(0,0);
  lcd.print((long)temperature /10);
  lcd.print(".");
  lcd.print((long)temperature % 10);
  lcd.print((char)223);
 
  lcd.setCursor(0,1);
  lcd.print(str);
  delay(1000);
 

}
