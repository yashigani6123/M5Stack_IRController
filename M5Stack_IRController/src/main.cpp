#include <M5Stack.h>

void setup(){
  M5.begin();

  M5.Lcd.setCursor(20,60);
  M5.Lcd.setTextSize(4);
  M5.Lcd.print("Hello workd");
}

void loop(){
  
}