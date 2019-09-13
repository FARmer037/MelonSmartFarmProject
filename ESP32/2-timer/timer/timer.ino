#include <WiFi.h>
#include <time.h>

int led = 4;

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

    if(p_tm->tm_year == 119 && p_tm->tm_mon == 7 && p_tm->tm_mday == 28 && p_tm->tm_hour == 17 && p_tm->tm_min == 15) {
      digitalWrite(led, 1);
    }
    else if(p_tm->tm_year == 119 && p_tm->tm_mon == 7 && p_tm->tm_mday == 28 && p_tm->tm_hour == 17 && p_tm->tm_min == 16) {
      digitalWrite(led, 0);
    }
  }
  else
  {
    Serial.println("Connection lost");
  }

  delay(1000);
}
