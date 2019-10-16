#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <time.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define DHTPIN 5
#define DHTTYPE DHT22

#define LINE_TOKEN "ZrHx4oHAJMZyYuy9HJb3kxgsjXcB5ekgjdzXJsF0V61"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "FARmer037"
#define AIO_KEY  "f7fc3c178a014d4bbada37a32acaf9cb"

WiFiClient client;

DHT dht(DHTPIN, DHTTYPE);

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish soilmoisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/soilmoisture");
Adafruit_MQTT_Publish lightintensity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lightintensity");
Adafruit_MQTT_Publish age = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/age");

//--------------------------------------------------------------------------------------------------------------------------------//
const char* ssid = "AndroidAP";
const char* password = "fnei9721";

int timezone = 7 * 3600;                      //ค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                  //ค่า Date Swing Time

time_t plant = 1571208760;                    //ค่าเวลาปลูก (จำนวนวินาทีตั้งแต่ 1 มกราคม 1900 เวลา 00:00:00)

int soil_sensor = 34;
int ldr_sensor = 35;

const unsigned long eventInterval = 60000;
unsigned long previousTime = 0;

//------------------------------------------------SETUP FUNCTION-------------------------------------------------------------------//
void setup() {
  
  Serial.begin(115200);
  dht.begin();
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
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int soil = read_soil();
    int ldr = read_ldr();
    int n_day = age_of_melon();

    unsigned long currentTime = millis();

    
    // ส่งทุก ๆ 1 นาที
    if(currentTime - previousTime >= eventInterval) {
      print_value(t, h, soil, ldr, n_day);
      
      sendDataToAdafruit(temp, t);
      sendDataToAdafruit(humidity, h);
      sendDataToAdafruit(soilmoisture, soil);
      sendDataToAdafruit(lightintensity, ldr);
      sendDataToAdafruit(age, n_day);

      previousTime = currentTime;
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

  return soil;
}

int read_ldr() {
  int value2 = analogRead(ldr_sensor);
  int ldr = map(value2, 4095, 0, 0, 100);

  return ldr;
}

int age_of_melon() {
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);

  double diff = difftime(now, plant);
  int age = diff/86400;

  return age;
}

void print_value(int t, int h, int soil, int ldr, int age) {
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("      ");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println("");

  Serial.print("Soil Moistuer = ");
  Serial.print(soil);
  Serial.print("      ");
  Serial.print("Light Intensity = ");
  Serial.print(ldr);
  Serial.println("");

  Serial.print("Age of Melon = ");
  Serial.print(ldr);
  Serial.println("Days");
}

boolean MQTT_connect() {  
  int8_t ret; 
  if (mqtt.connected()) {    
    return true; 
  }  
  uint8_t retries = 3;  
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect(); delay(2000);
    retries--;
    if (retries == 0) {
      return false; 
    }
  }
  return true;
}

void sendDataToAdafruit(Adafruit_MQTT_Publish feed, int value) {
  if (MQTT_connect()) {
    if(feed.publish(value)) {
      Serial.println("Data sent successfully.");
    }
    else {
      Serial.println("Problem to send the data!");
    }
  }
  else {
    Serial.println("Problem connect to the site!");
  }
}
