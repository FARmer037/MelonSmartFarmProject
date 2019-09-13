#include <MCP3008.h>
#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

#include "DHT.h"
#define DHTTYPE DHT22
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

float t = 0;
float h = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  delay(2000);

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
}
