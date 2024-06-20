
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup(){
  lcd.begin(16,2);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  //lcd.setCursor(0,0);
  lcd.print("HELLO"); // Start Printing
  //lcd.setCursor(9,0);
  lcd.print("WORLD");
}
void loop(){
}
