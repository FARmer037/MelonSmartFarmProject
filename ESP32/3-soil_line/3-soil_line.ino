#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>

#define LINE_TOKEN "ZrHx4oHAJMZyYuy9HJb3kxgsjXcB5ekgjdzXJsF0V61"
String message1 = "%E0%B8%A3%E0%B8%94%E0%B8%99%E0%B9%89%E0%B8%B3%E0%B9%81%E0%B8%A5%E0%B9%89%E0%B8%A7%20!";    //  รดน้ำแล้ว!

int led = 4;
int soil_sensor = 34;

const char* ssid = "AndroidAP";
const char* password = "fnei9721";

int timezone = 7 * 3600;                      //ค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                  //ค่า Date Swing Time

void setup() {
  pinMode(led, OUTPUT);
  
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

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดึงเวลาจาก Server
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
      Serial.print("*");
      delay(1000);
    }
    Serial.println("");

}

void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    Serial.println(ctime(&now));
    struct tm* p_tm = localtime(&now);

    if(p_tm->tm_year == 119 && p_tm->tm_mon == 8 && p_tm->tm_mday <= 14) {
      water();
    }
  }
  else
  {
    Serial.println("Connection lost");
  }

  delay(1000);
}

bool LINE_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return false;   
  }

  String payload = "message=" + message;
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "User-Agent: ESP32\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(payload.length()) + "\r\n";
  req += "\r\n";
  req += payload;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  long timeOut = millis() + 30000;
  while(client.connected() && timeOut > millis()) {
    if (client.available()) {
      String str = client.readString();
      // Serial.print(str);
    }
    delay(10);
  }
  // Serial.println("-------------");

  return timeOut > millis();
}

int read_soil() {
  int value = analogRead(soil_sensor);     //อ่านค่าความชื้นดิน 
    Serial.println(value);          //แสดงผลค่าความชื้นดินบนคอนโซล
    int soil = map(value, 4095, 0, 0, 100);
    Serial.print(soil);
    Serial.println("%");
    
    return soil;
}

void water() {
    int soil = read_soil();
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    Serial.println(ctime(&now));
  
    struct tm* p_tm = localtime(&now);

    if(soil < 80) {
      if(p_tm->tm_hour == 8 && p_tm->tm_min == 0) {
        digitalWrite(led, 1);

        if(soil >= 80) {
          digitalWrite(led, 0);
          Serial.println(message1);
          LINE_Notify(message1);
        }
      }
    }
}
