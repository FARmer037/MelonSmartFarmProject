#include <WiFi.h>
#include <time.h>

const char* ssid = "AndroidAP";
const char* password = "fnei9721";

int timezone = 7 * 3600;                      //ค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                  //ค่า Date Swing Time

/*  struct ใน library Time.h
struct tm {
   int tm_sec;         // วินาที,  range 0 to 59          
   int tm_min;         // นาที, range 0 to 59           
   int tm_hour;        // ชั่วโมง, range 0 to 23             
   int tm_mday;        // วันที่, range 1 to 31  
   int tm_mon;         // เดือน, range 0 to 11             
   int tm_year;        // ปีคริสศักราช ตั้งแต่ 1900   
   int tm_wday;        // วัน, range 0 to 6    
   int tm_yday;        // วันใน 1 ปี, range 0 to 365  
   int tm_isdst;       // daylight saving time             
};
*/

String ntp_day = "";
String ntp_time = "";

int state = 100;
int age = 0;

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
/*----------------------------------------------------------------------------------------------*/
void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
    time_t now = time(nullptr);
//    Serial.println(ctime(&now));
    struct tm* p_tm = localtime(&now);

    ntp_time = String(p_tm->tm_hour);
    ntp_time += ":";
    ntp_time += String(p_tm->tm_min);
    ntp_time += ":";
    ntp_time += String(p_tm->tm_sec);
  
    ntp_day = String(p_tm->tm_mday); 
    ntp_day += "-";
    ntp_day += String(p_tm->tm_mon + 1);
    ntp_day += "-";  
    ntp_day += String(p_tm->tm_year + 1900);

    Serial.print(ntp_day);
    Serial.print(" ");
    Serial.println(ntp_time);

    if(state != p_tm->tm_min) {
      age += 1;
      state = p_tm->tm_min;
    }

    Serial.println(age-2);
  }
  else
  {
    Serial.println("Connection lost");
  }

  delay(1000);
}
