#include <WiFi.h>

const char* ssid = "AndroidAP";
const char* password = "fnei9721";

void setup() {
  Serial.begin(115200);
  read_soil();
  WiFi.mode(WIFI_STA);
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
    read_soil();
    delay(1000);
}
void read_soil() {
  int value = analogRead(34);     //อ่านค่าความชื้นดิน 
    Serial.println(value);          //แสดงผลค่าความชื้นดินบนคอนโซล
    int soil = map(value, 4095, 0, 0, 100);
    Serial.print(soil);
    Serial.println("%");
}
