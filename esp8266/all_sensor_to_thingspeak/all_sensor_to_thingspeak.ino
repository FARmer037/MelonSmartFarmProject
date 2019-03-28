#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <MCP3008.h>

#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

#define DHTPIN 2 // what pin we're connected to
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

String apiKey = "5FXVUTMEBRIKVPR1";
const char* ssid = "AndroidAP";
const char* password = "fnei9721";

ESP8266WiFiMulti WiFiMulti;
HTTPClient http;

float t = 0;
float h = 0;

void setup() {
  dht.begin();
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("Connecting...");
  WiFiMulti.addAP(ssid, password);

}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    int SoilMoisture_sensor = adc.readADC(0);
    int SoilMoisture = map(SoilMoisture_sensor, 1024, 0, 0, 100);
    int LDR_sensor = adc.readADC(1);
    int LDR = map(LDR_sensor, 1024, 0, 0, 100);
    
    h = dht.readHumidity();
    t = dht.readTemperature();

    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.print("\t\t");
    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.println();
    Serial.print("Soil Moisture = ");
    Serial.print(SoilMoisture);
    Serial.print(" %");
    Serial.print("\t\t");
    Serial.print("LDR = ");
    Serial.print(LDR);
    Serial.print(" %");
    Serial.println();
    Serial.println();

    http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(t) 
                + "&field2=" + String(h) + "&field3=" + String(SoilMoisture) + "&field4=" + String(LDR));

    int httpCode = http.GET();
    if (httpCode > 0) {
//      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
//        Serial.println(payload);
      }
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  delay(60000);
}
