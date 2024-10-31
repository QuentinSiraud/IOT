#include <ESP32Servo.h>
#include <PinList.h>

Servo windowServo;


bool ACTIVITY = false;
bool smoke_status = false;


void setup() {
  setupPins();
}

void loop() {
  changeStatus();
  breath();
}

void changeStatus()
{
  if(!digitalRead(SMOKE_SENSOR_PIN)) {
    smoke_status = true;
  }
  else 
    smoke_status = false;
}


void breath()
{
  if(smoke_status) {
    delay(10);
  }
  else {
    fan();
    
    delay(10);
  }
    analogWrite(FAN_PIN, LOW);
}

void setupPins() 
{
  pinMode(FAN_PIN, OUTPUT);
  pinMode(SMOKE_SENSOR_PIN, INPUT);
}


void fan(){
  analogWrite(FAN_PIN, 180);
  delay(1000);
}
