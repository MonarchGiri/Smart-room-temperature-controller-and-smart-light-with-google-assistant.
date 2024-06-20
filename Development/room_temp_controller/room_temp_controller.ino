#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

const int Heater_pin = 12, FAN_pin = 14; // declare the IO pins

float SetTemp = 25; // the prefer room temperature is set as 25 degree celsius

#include <LiquidCrystal_I2C.h> //including the LCD I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
dht.begin();
lcd.begin(16,2);
pinMode(Heater_pin, OUTPUT);
pinMode(FAN_pin, OUTPUT);
lcd.init();   // initializing the LCD
lcd.backlight(); // Enable or Turn On the backlight
lcd.clear();
lcd.print("Temperature");
lcd.setCursor(0,1); // setting cursor in LCD display
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
  
// condition is applied for the fan and heater
  if(t > SetTemp){
    digitalWrite(FAN_pin, HIGH);
    delay(2000);
  }
  else{
    digitalWrite(FAN_pin, LOW);
    }
    
  if(t < SetTemp){
    digitalWrite(Heater_pin, HIGH);
    delay(2000);
    }
    else{
      digitalWrite(Heater_pin, LOW);
      }
}
