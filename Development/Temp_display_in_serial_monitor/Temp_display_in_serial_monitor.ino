#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE); 
void setup(void)
{ 
  dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature\n\n");
  delay(700);

}
void loop() {
    float h = dht.readHumidity(); // read humidity
    float t = dht.readTemperature(); // read temperature        
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
  delay(800);
}
