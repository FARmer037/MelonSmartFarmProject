#include <ESP8266WiFi.h>
#include <time.h>

int led = D4;

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
  pinMode(led, OUTPUT);
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

  week_1();

  delay(1000);
}

void week_1(){
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);       
//  Serial.print(p_tm->tm_year);            //ตัวอย่างการนำค่าจาก Struct มาใช้
//  Serial.print(" ");
//  Serial.print(p_tm->tm_mon);
//  Serial.print(" ");
//  Serial.print(p_tm->tm_mday);
//  Serial.print(" ");
//  Serial.print(p_tm->tm_hour);
//  Serial.print(" ");
//  Serial.println(p_tm->tm_min);
  if(p_tm->tm_year == 119 && p_tm->tm_mon == 2 && p_tm->tm_mday == 15 && p_tm->tm_hour == 20 && p_tm->tm_min == 51) {
    digitalWrite(led, 1);
  }
  else if(p_tm->tm_year == 119 && p_tm->tm_mon == 2 && p_tm->tm_mday == 15 && p_tm->tm_hour == 20 && p_tm->tm_min == 52) {
    digitalWrite(led, 0);
  }
}
