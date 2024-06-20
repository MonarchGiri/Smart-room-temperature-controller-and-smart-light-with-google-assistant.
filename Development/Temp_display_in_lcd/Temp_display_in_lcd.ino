#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

#include <LiquidCrystal_I2C.h> // including the LCD I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
dht.begin();
lcd.begin(16,2);
lcd.init();   // initializing the LCD
lcd.backlight(); // Enable or Turn On the backlight
lcd.clear();
lcd.print("Temperature");
lcd.setCursor(0,1); // seting cursor in LCD
lcd.print("Sensor");
delay(1000);

}

void loop(){
  float t = dht.readTemperature(); // read temperature

  lcd.clear();
  lcd.print("Temperature");
  lcd.setCursor(0,1);
  lcd.print(t);
  lcd.print("C");
  delay(2000);
}
