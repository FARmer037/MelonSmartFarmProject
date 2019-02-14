#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

String apiKey = "4XLX9DWGXLT7G02I";
const char* ssid = "AndroidAP";
const char* password = "fnei9721"; 

ESP8266WiFiMulti WiFiMulti;
HTTPClient http;

int analogPin = A0;
int val = 0;
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
    val = analogRead(analogPin);
    Serial.println(val);

    http.begin("http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(val));

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
  delay(20000);
}
