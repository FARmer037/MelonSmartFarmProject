#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>

#define LINE_TOKEN "ZrHx4oHAJMZyYuy9HJb3kxgsjXcB5ekgjdzXJsF0V61"
String m_Watered = "%E0%B8%A3%E0%B8%94%E0%B8%99%E0%B9%89%E0%B8%B3%E0%B9%81%E0%B8%A5%E0%B9%89%E0%B8%A7%20!";    //  รดน้ำแล้ว!
String m_TernOn = "%E0%B9%80%E0%B8%9B%E0%B8%B4%E0%B8%94%E0%B9%84%E0%B8%9F%20LED%20Grow%20Light%20%E0%B9%81%E0%B8%A5%E0%B9%89%E0%B8%A7%20!";   //  เปิดไฟ LED Grow Light แล้ว !
String m_TernOff = "%E0%B8%9B%E0%B8%B4%E0%B8%94%E0%B9%84%E0%B8%9F%20LED%20Grow%20Light%20%E0%B9%81%E0%B8%A5%E0%B9%89%E0%B8%A7%20!";           //  ปิดไฟ LED Grow Light แล้ว !

const char* ssid = "AndroidAP";
const char* password = "fnei9721";

int timezone = 7 * 3600;                      //ค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                  //ค่า Date Swing Time

int pump = 32;
int led = 33;
int soil_sensor = 34;
int ldr_sensor = 35;

int state_water = 0;
int state_light = 0;

//----------------------------------------------------------------------------------------------------------------------------------//

void setup() {
  pinMode(pump, OUTPUT);
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

//----------------------------------------------------------------------------------------------------------------------------------//

void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    int soil = read_soil();
    int ldr = read_ldr();
    
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    Serial.println(ctime(&now));
    struct tm* p_tm = localtime(&now);

    if(p_tm->tm_year == 119 && p_tm->tm_mon == 8 && p_tm->tm_mday <= 14) {
      water(soil);
      turnOnTheLight(ldr);
    }
  }
  else
  {
    Serial.println("Connection lost");
  }

  delay(1000);
}

//----------------------------------------------------------------------------------------------------------------------------------//

int read_soil() {
  int value1 = analogRead(soil_sensor);
  int soil = map(value1, 4095, 0, 0, 100);

  Serial.print("Soil = ");
  Serial.println(soil);
  Serial.println("%");

  return soil;
}

int read_ldr() {
  int value2 = analogRead(ldr_sensor);
  int ldr = map(value2, 4095, 0, 0, 100);

  Serial.print("ldr = ");
  Serial.println(ldr);
  Serial.println("%");

  return ldr;
}

void water(int soil) {
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

  if(soil < 80) {
    if(p_tm->tm_hour == 8 && p_tm->tm_min == 0) {
      digitalWrite(pump, 1);

      if(soil >= 80) {
        digitalWrite(pump, 0);
        LINE_Notify(m_Watered);
      }
    }
  }
}

void turnOnTheLight(int ldr) {
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

  if((p_tm->tm_hour >= 18 && p_tm->tm_min == 0) || p_tm->tm_hour <= 6 || ldr < 50) {          //*** มากกว่า 18 แต่ น้อยกว่า 16 คือ ??? ***//
    digitalWrite(led, 1);

    if(state_light == 0) {
      LINE_Notify(m_TernOn);
      state_light = 1;
    }
  }
  else {
    digitalWrite(led, 0);

    if(state_light == 1) {
      LINE_Notify(m_TernOff);
      state_light = 0;
    }
  }
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
