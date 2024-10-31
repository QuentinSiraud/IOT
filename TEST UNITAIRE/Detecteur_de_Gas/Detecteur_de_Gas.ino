#include <PinList.h>

void setup() {
  Serial.begin(9600);
  pinMode(GAS_SENSOR_PIN, INPUT);

}

void loop() {
  if (digitalRead(GAS_SENSOR_PIN)) {
    Serial.println("Attention du gaz à été detecter");
  } else {
    Serial.println("Tout va bien");
  }
  delay(10);
}
