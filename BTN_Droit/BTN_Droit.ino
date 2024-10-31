#include <ESP32Servo.h>
#define buzzer_pin 25

const int RIGHT_BTN_PIN = 27; // 👉

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  dring();
  pinMode(RIGHT_BTN_PIN, INPUT);
}

void loop() {
  dring();
}

void dring()
{
  if(!digitalRead(RIGHT_BTN_PIN)){
  tone(buzzer_pin, 12000);
  delay(10);
  }
  noTone(buzzer_pin);  //Close
  delay(10);
}
