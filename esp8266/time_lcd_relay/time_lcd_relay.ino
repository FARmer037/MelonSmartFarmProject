#include <ESP8266WiFi.h>
#include <time.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27   //  กำหนดตำแหน่ง Address ของ I2C
#define BACKLIGHT_PIN 3

LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);

const char* ssid = "AndroidAP";               //ใส่ชื่อ SSID Wifi
const char* password = "fnei9721";            //ใส่รหัสผ่าน

String ntp_day = "";
String ntp_time = "";

int timezone = 7 * 3600;                      //ค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                  //ค่า Date Swing Time

int relay = D4;

void setup()
{
  pinMode(relay, OUTPUT);
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
  
  lcd.begin (20,4); 
// Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();                    // ไปที่ตัวอักษรที่ 0 แถวที่ 1

  lcd.setCursor(5,0);             // ไปทตัวอักษรที่ 6 แถวที่ 1
  lcd.print("Welcome to");

  lcd.setCursor(2,2);             // ไปที่ตัวอักษรที่ 3 แถวที่ 3
  lcd.print("Melon Smart Farm");

  delay(3000);
}

void loop()
{
  show_in_lcd();
  water_the_plants();

  delay(1000);
}

void show_in_lcd() {
  lcd.clear();
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  
  Serial.print(p_tm->tm_hour); Serial.print(":");
  Serial.print(p_tm->tm_min); Serial.print(":");
  Serial.println(p_tm->tm_sec);
  Serial.println(ctime(&now));

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

  lcd.setCursor(1, 0);
  lcd.print("Date :");
  lcd.setCursor(10, 0);
  lcd.print(ntp_day);
  
  lcd.setCursor(1, 1);
  lcd.print("Time :");
  lcd.setCursor(10, 1); 
  lcd.print(ntp_time);
}

void water_the_plants() {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);       
  if(   p_tm->tm_year == 119 && p_tm->tm_mon == 3 
        && p_tm->tm_hour == 7 && p_tm->tm_min == 00) {
    digitalWrite(relay, 1);
  }
  else if(p_tm->tm_year == 119 && p_tm->tm_mon == 3 
          && p_tm->tm_hour == 7 && p_tm->tm_min == 20) {
    digitalWrite(relay, 0);
  }

  if ( p_tm->tm_year == 119 && p_tm->tm_mon == 3 
       && p_tm->tm_hour == 11 && p_tm->tm_min == 00) {
    digitalWrite(relay, 1);
  }
  else if(p_tm->tm_year == 119 && p_tm->tm_mon == 3 
          && p_tm->tm_hour == 11 && p_tm->tm_min == 20) {
    digitalWrite(relay, 0);
  }

  if (  p_tm->tm_year == 119 && p_tm->tm_mon == 3 
        && p_tm->tm_hour == 15 && p_tm->tm_min == 00) {
    digitalWrite(relay, 1);
  }
  else if(p_tm->tm_year == 119 && p_tm->tm_mon == 3 
          && p_tm->tm_hour == 15 && p_tm->tm_min == 20) {
    digitalWrite(relay, 0);
  }
}
