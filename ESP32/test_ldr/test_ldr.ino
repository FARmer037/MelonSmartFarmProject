int pump = 32;
int led = 33;
int soil_sensor = 34;
int ldr_sensor = 35;
int line = 4;

int state_water = 0;
int state_light = 0;

//typedef struct {
//  int soil;
//  int ldr;
//}Sensor;

void setup() {
  pinMode(pump, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(line, OUTPUT);

  Serial.begin(115200);

}

void loop() {
  int soil = read_soil();
  int ldr = read_ldr();

  Serial.println();

  turnOnTheLight(ldr);

  delay(1000);
}

int read_soil() {
  int value1 = analogRead(soil_sensor);
  int soil = map(value1, 4095, 0, 0, 100);

  Serial.println(soil);

  return soil;
}

int read_ldr() {
  int value2 = analogRead(ldr_sensor);
  int ldr = map(value2, 4095, 0, 0, 100);

  Serial.println(ldr);

  return ldr;
}

void water(int soil) {
  if(soil < 80) {
    digitalWrite(pump, 1);
  }

  if(soil >= 80) {
    digitalWrite(pump, 0);
  }
}

void turnOnTheLight(int ldr) {

  if(ldr < 50) {
    digitalWrite(led, 1);
    if(state_light == 0) {
      digitalWrite(line, 1);
      state_light = 1;
    }
    else {
      digitalWrite(line, 0);
    }
  }
  else if(ldr > 50){
    digitalWrite(led, 0);
    if(state_light == 1) {
      digitalWrite(line, 1);
      state_light = 0;
    }
    else {
      digitalWrite(line, 0);
    }
  }
}
