#include <WiFi.h>

const char* ssid = "AndroidAP";
const char* password = "fnei9721";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    Serial.println("You can try to ping me");
    delay(5000);
  }
  else
  {
    Serial.println("Connection lost");
  }

}
