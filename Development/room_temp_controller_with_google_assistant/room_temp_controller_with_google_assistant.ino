#include <ESP8266WiFi.h> // including the library for wifi
#include "Adafruit_MQTT.h" // adafruit mqtt
#include "Adafruit_MQTT_Client.h" // adafruit mqtt client
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

#define WLAN_SSID       "Home wifi1"
#define WLAN_PASS       "sweethome09"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "MonarchGiri"
#define AIO_KEY         ""

const int Heater_pin = 12, FAN_pin = 14, man = 13; //declare the IO pins

float SetTemp = 25; // teh prefer room temperature is set as 25 degree celsius

#include <LiquidCrystal_I2C.h> // including the LCD I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient client;     // Create an ESP8266 WiFiClient class to connect to the MQTT server.

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        
Adafruit_MQTT_Subscribe LED_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED_Control");

void MQTT_connect();
void setup() {

Serial.begin(115200);
delay(10);
dht.begin(); // setup sensor 
lcd.begin(16,2); // setup lcd

pinMode(Heater_pin, OUTPUT);
pinMode(FAN_pin, OUTPUT);
pinMode(man, INPUT);

lcd.init();   // initializing the LCD
lcd.backlight(); // Enable or Turn On the backlight
lcd.clear();
lcd.print("Temperature");
lcd.setCursor(0,1);
lcd.print("Sensor");
delay(1000);
   

 // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&LED_Control);

}



void loop() {
  MQTT_connect();
  
  float t = dht.readTemperature();

  lcd.clear();
  lcd.print("Temp.");
  lcd.setCursor(0,1);
  lcd.print(t);
  Serial.print(t);
  lcd.print("C");
  lcd.setCursor(6,0);
  lcd.print("SetTemp=25");
  delay(1000);
 
  Adafruit_MQTT_Subscribe *subscription;

  if (digitalRead(man) == HIGH) {
    lcd.setCursor(7,1);
    lcd.print("Auto ON");
    if (t > SetTemp) {
      digitalWrite(FAN_pin, HIGH);
      delay(1000);
    } 
    else {
      digitalWrite(FAN_pin, LOW);
      }
      
    if(t < SetTemp){
      digitalWrite(Heater_pin, HIGH);
      delay(1000);
      }
      else{
       digitalWrite(Heater_pin, LOW); 
        }
  } 
  else {
     lcd.setCursor(7,1);
     lcd.print("Auto OFF");
    while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &LED_Control) {
      Serial.print(F("Got: "));
      Serial.println((char *)LED_Control.lastread);
      if (!strcmp((char*) LED_Control.lastread, "FAN ON"))
      {
        fanOn();
      }
      if (!strcmp((char*) LED_Control.lastread, "HEATER ON"))
      {
        fanOff();
      }
      if (!strcmp((char*) LED_Control.lastread, "ALL OFF"))
      {
        allOff();
      }
    }
  }
  }
}


void fanOn(){
   digitalWrite(FAN_pin, HIGH);
    digitalWrite(Heater_pin, LOW);
  }
void fanOff(){
      digitalWrite(Heater_pin, HIGH);
      digitalWrite(FAN_pin, LOW);
    }
void allOff(){
  digitalWrite(FAN_pin, LOW);
  digitalWrite(Heater_pin, LOW);
  }

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

 Serial.print("Connecting to MQTT... ");


  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds


      
  }
  Serial.println("MQTT Connected!");
}
