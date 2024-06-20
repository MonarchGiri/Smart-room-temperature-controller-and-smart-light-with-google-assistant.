#include <ESP8266WiFi.h> // including the library for the wifi
#include "Adafruit_MQTT.h" // adafruit mqtt
#include "Adafruit_MQTT_Client.h" // adafruit mqtt client

#define WLAN_SSID       "Home wifi1"
#define WLAN_PASS       "sweethome09"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "MonarchGiri"
#define AIO_KEY         ""
int output=12;

WiFiClient client;     // Create an ESP8266 WiFiClient class to connect to the MQTT server.

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);       
Adafruit_MQTT_Subscribe lightcontrol = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/lightcontrol");

void MQTT_connect();

void setup() {

  Serial.begin(115200);
  delay(10);

pinMode(12,OUTPUT);

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
}

uint32_t x=0;
void loop() {
   MQTT_connect();

Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &lightcontrol) {
      Serial.print(F("Got: "));
      Serial.println((char *)lightcontrol.lastread);
       if (!strcmp((char*) lightcontrol.lastread, "ON"))
      {
        digitalWrite(12, HIGH);
      }
      else
      {
        digitalWrite(12, LOW);
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

uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected

       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}
