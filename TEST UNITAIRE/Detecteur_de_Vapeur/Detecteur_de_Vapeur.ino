#include <PinList.h>

void setup() {
  Serial.begin(9600);
  pinMode(SMOKE_SENSOR_PIN, INPUT);

}

void loop() {
  Serial.println(analogRead(SMOKE_SENSOR_PIN));
  delay(10);
}
