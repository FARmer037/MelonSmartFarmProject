#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

String apiKey = "Z5KNX8GBCLG1CWJZ";
const char* ssid = "AndroidAP";
const char* password = "fnei9721";

#define DHTPIN 5 // what pin we're connected to
#define DHTTYPE DHT22
 
DHT dht(DHTPIN, DHTTYPE);

ESP8266WiFiMulti WiFiMulti;
HTTPClient http;

float t = 0;
float h = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("Connecting...");
  WiFiMulti.addAP(ssid, password);

}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    h = dht.readHumidity();
    t = dht.readTemperature();
//    if (isnan(h) || isnan(t)) {
//      Serial.println("Failed to read from DHT sensor!");
//      return;
//    }

    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.print("\t\t");
    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.println();

    http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h));

    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  delay(60000);
}
