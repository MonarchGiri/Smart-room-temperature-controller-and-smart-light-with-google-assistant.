#include <ESP8266WiFi.h> // including the library for the wifi
#include <NTPClient.h> // including the librart for the ntp client
#include <WiFiUdp.h>

#include <LiquidCrystal_I2C.h> // including the LCD I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Replace with your network credentials
const char * ssid     = "Home wifi1";
const char * password = "sweethome09";

int ledpin = 12; // setting the IO pin for the LED

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
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(20700);
}

void loop() {
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
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

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  Serial.println("");
  
  lcd.setCursor(0,0); // setting cursor in LCD display
  lcd.print("TIME=");
  lcd.setCursor(5,0);
  lcd.print(formattedTime);

  lcd.setCursor(0,1); // setting cursor in LCD display
  lcd.print("DATE=");
  lcd.setCursor(5,1);
  lcd.print(currentDate);  
  
// condition is applied when to turn on and off the light
  if(currentHour >= 7 && currentHour <= 17){
    digitalWrite(ledpin,LOW);
    }  
  else{
    digitalWrite(ledpin,HIGH);
    }
 
  delay(1000);
}
