//int pump = 32;
//int led = 33;
int soil_sensor = 34;
int ldr_sensor = 35;
//int line = 4;
//
//int state_water = 0;
//int state_light = 0;

void setup() {
//  pinMode(pump, OUTPUT);
//  pinMode(led, OUTPUT);
//  pinMode(line, OUTPUT);

  Serial.begin(115200);

}

void loop() {
  int value1 = analogRead(soil_sensor);
  int soil = map(value1, 4095, 0, 0, 100);
  int value2 = analogRead(ldr_sensor);
  int ldr = map(value2, 4095, 0, 0, 100);

  Serial.println(soil);
  Serial.println(ldr);
  Serial.println();
  
  delay(1000);
}
