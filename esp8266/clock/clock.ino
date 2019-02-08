#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "AndroidAP";               //ใส่ชื่อ SSID Wifi
const char* password = "fnei9721";            //ใส่รหัสผ่าน

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

void setup()
{
 Serial.begin(115200);
 Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);                                              //เชื่อมต่อ Wifi
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
     Serial.print(",");
     delay(1000);
   }
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดึงเวลาจาก Server
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("");
}
        
void loop()
{
//  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดีงเวลาปัจจุบันจาก Server อีกครั้ง
  time_t now = time(nullptr);
  Serial.println(ctime(&now));
  
  struct tm* p_tm = localtime(&now);        //เอาเวลาปัจจุบันไปเก็บใน Struct
//  Serial.print(p_tm->tm_sec);             //ตัวอย่างการนำค่าจาก Struct มาใช้

  delay(1000);
}
