#include <PinList.h>

bool STATUS_MOTION_SENSOR = false;

void setup() {
  Serial.begin(9600);
  setupPins();
}

void loop() {
  changeStatus();
  toggleLight();
}

void changeStatus()
{
  if(digitalRead(MOTION_SENSOR_PIN)) {
    STATUS_MOTION_SENSOR = true;
  }
}

void toggleLight()
{
  if(STATUS_MOTION_SENSOR) {
    digitalWrite(LED_PIN, HIGH);
    STATUS_MOTION_SENSOR = false;
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
    delay(10);
}

void setupPins() 
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);
}

