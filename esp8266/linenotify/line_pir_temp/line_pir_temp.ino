#include <ESP8266WiFi.h>
#include <DHT.h>

#define WIFI_SSID "AndroidAP"
#define WIFI_PASSWORD "fnei9721"

#define LINE_TOKEN_TMP "ZrHx4oHAJMZyYuy9HJb3kxgsjXcB5ekgjdzXJsF0V61"
//#define LINE_TOKEN_TMP "OuhpeAiuI0XD0q1Z4DKohTIZBHWxGsMxSeq8sNFw2ZU"

#define DHTPIN D7
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

String message1 = "%E0%B8%AD%E0%B8%B8%E0%B8%93%E0%B8%AB%E0%B8%A0%E0%B8%B9%E0%B8%A1%E0%B8%B4";
String message2 = "%E0%B8%84%E0%B8%A7%E0%B8%B2%E0%B8%A1%E0%B8%8A%E0%B8%B7%E0%B9%89%E0%B8%99%E0%B9%83%E0%B8%99%E0%B8%AD%E0%B8%B2%E0%B8%81%E0%B8%B2%E0%B8%A8";

bool beep_state = false;
bool send_state = false;
uint32_t ts, ts1, ts2;

void setup() {

  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  dht.begin();

  Serial.println("connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  delay(10000);

  read_sensor();

  ts = ts1 = ts2 = millis();
}

void loop() {

  ts = millis();

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if ((ts - ts2 >= 60000) && (WiFi.status() == WL_CONNECTED)) {
    read_sensor();
  }
  delay(10);

}

void read_sensor() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
//  Line_Notify(message1 + " " + t + "%C2%B0C");
  Line_Notify(message1 + " " + t + "%C2%B0C" + "\t" + 
              message2 + " " + h + "%E0%B9%80%E0%B8%9B%E0%B8%AD%E0%B8%A3%E0%B9%8C%E0%B9%80%E0%B8%8B%E0%B9%87%E0%B8%99");
}

void Line_Notify(String message) {
  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;
  }
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN_TMP) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  client.print(req);
  delay(30);
  ts2 = millis();
  // Serial.println("-----");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-----");
}
