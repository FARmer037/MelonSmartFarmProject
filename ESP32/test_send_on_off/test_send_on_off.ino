/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP32     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi.h>

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "FARmer037"
#define AIO_KEY  "f7fc3c178a014d4bbada37a32acaf9cb"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

boolean MQTT_connect();

boolean MQTT_connect() {  
  int8_t ret; 
  if (mqtt.connected()) {    
    return true; 
  }  
  uint8_t retries = 3;  
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect(); delay(2000);
    retries--;
    if (retries == 0) {
      return false; 
    }
  }
  return true;
}

Adafruit_MQTT_Publish Light Switch = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/melon-smart-farm.light-switch");

void setup() {
  Serial.begin(9600);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("AndroidAP","fnei9721");
  while((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print("..");

  }
  
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
}


void loop() {
    if (MQTT_connect()) {
      if (Light Switch.publish("ON")) {
        Serial.println("data sent: ON");
      } 
      else {
        Serial.println("Problem to send the random data.");
      }
      
      delay(10000);
      
      if (Light Switch.publish("OFF")) {
        Serial.println("data sent: OFF");
      } 
      else {
        Serial.println("Problem to send the random data.");
      }
      
    } 
    else {
      Serial.println("Problem connect to the site.");
    }
    delay(10000);

}
