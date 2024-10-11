#include <ESP32Servo.h>

const int BUZZER_PIN = 25; // 🔊
const int RIGHT_BTN_PIN = 27; // 👉

void setup() {
  setupPins();
  dring();
}

void loop() {
  dring();
}

void dring()
{
  if(!digitalRead(RIGHT_BTN_PIN)) {
    tone(BUZZER_PIN, 12000);
    delay(10);
  }
  noTone(BUZZER_PIN);  // Close
  delay(10);
}

void setupPins() 
{
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RIGHT_BTN_PIN, INPUT);
}
