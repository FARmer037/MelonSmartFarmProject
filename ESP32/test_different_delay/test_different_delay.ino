int led1 = 32;
int led2 = 33;
int led3 = 34;
int led4 = 35;

uint32_t ts, ts1, ts2;

void setup() {
    Serial.begin(115200);

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);

    ts = ts1 = ts2 = millis();
}

void loop() {

    ts = millis();

    if (ts - ts2 == 1000){
      digitalWrite(led1, 1);
    }
    else if(ts - ts2 == 2000){
      digitalWrite(led2, 1);
    }
    else if(ts - ts2 == 3000){
      digitalWrite(led3, 1);
    }
    else if(ts - ts2 == 4000){
      digitalWrite(led4, 1);
    }
    
    delay(10);

}
