#include <ESP8266WiFi.h> // including the library for the wifi
#include <NTPClient.h> // including the library for the ntp client
#include <WiFiUdp.h>

#include <LiquidCrystal_I2C.h> // including the LCD I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "Adafruit_MQTT.h" // adafruit mqtt
#include "Adafruit_MQTT_Client.h" // adafruit mqtt client

#define WLAN_SSID       "Home wifi1"
#define WLAN_PASS       "sweethome09"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "MonarchGiri"
#define AIO_KEY         ""

WiFiClient client;  // Create an ESP8266 WiFiClient class to connect to the MQTT server.

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);       
Adafruit_MQTT_Subscribe lightcontrol = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lightcontrol");

void MQTT_connect();

int ledpin = 12; //setting the IO pins
int man = 13;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void setup() {
  pinMode(ledpin,OUTPUT);

lcd.begin(16,2);
lcd.init();   // initializing the LCD
lcd.backlight(); // Enable or Turn On the backlight
  
  
  // Initialize Serial Monitor
  Serial.begin(115200);
  
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
  mqtt.subscribe(&lightcontrol);

  // Initialize a NTPClient to get time
  timeClient.begin();
  //set timezone
  timeClient.setTimeOffset(20700);

}


void loop() {
  MQTT_connect();
  // function to get the current date and time from the NTP server
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  //function returns the time in HH:MM:SS format
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);  

  // create a time structure to access its elements to get information about the date.
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1; //+1 for January to corresponds to 1
  Serial.print("Month: ");
  Serial.println(currentMonth);

  int currentYear = ptm->tm_year+1900; //tm_year saves the number of years after 1900
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  // currentDate holds the current date in the YYYY-MM-DD format
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");
  
  lcd.setCursor(0,0);
  lcd.print("TIME=");
  lcd.setCursor(5,0);
  lcd.print(formattedTime);

  lcd.setCursor(0,1);
  lcd.print("DATE=");
  lcd.setCursor(5,1);
  lcd.print(currentDate);  

  Adafruit_MQTT_Subscribe *subscription;

  if (digitalRead(man) == HIGH) {
   if(currentHour >= 7 && currentHour <= 17){
      digitalWrite(ledpin,LOW);
    }  
    else{
      digitalWrite(ledpin,HIGH);
    }
  delay(1000);
    }
 
  else{
      while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &lightcontrol) {
      Serial.print(F("Got: "));
      Serial.println((char *)lightcontrol.lastread);
       if (!strcmp((char*) lightcontrol.lastread, "ON"))
      {
        digitalWrite(ledpin, HIGH);
      }
      else
      {
        digitalWrite(ledpin, LOW);
      }
    }
  }
    }
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
