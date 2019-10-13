#include <WiFi.h>
#include <time.h>

const char* ssid = "AndroidAP";
const char* password = "fnei9721";

int timezone = 7 * 3600;                      //ค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                  //ค่า Date Swing Time

time_t plant = 1570869727;

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
    int d, h, m, s;
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
    Serial.println(ctime(&now));
  
    double diff = difftime(now, plant);

    d = diff/86400;
    h = ((int)diff%86400)/3600;
    m = (((int)diff%86400)%3600)/60;
    s = ((((int)diff%86400)%3600)%60)%60;

    Serial.print("Age of Melon is ");
    Serial.print(d);
    Serial.print(" day ");
    Serial.print(h);
    Serial.print(" hours ");
    Serial.print(m);
    Serial.print(" minutes ");
    Serial.print(s);
    Serial.println(" second");

  }
  else
  {
    Serial.println("Connection lost");
  }

  delay(1000);
}
