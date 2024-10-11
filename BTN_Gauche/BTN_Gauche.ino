#include <ESP32Servo.h>
Servo windowServo;

const int WINDOW_SERVO_PIN = 2;
const int LEFT_BTN_PIN = 16; // 👉

bool window_status = false;

void setup() {
  Serial.begin(9600);
  setupPins();
}

void loop() {
  changeStatus();
  openWindow();
}

void changeStatus()
{
  if(!digitalRead(LEFT_BTN_PIN)) {
    window_status = !window_status;
  }
}

void openWindow()
{
  if(window_status == false) {
    windowServo.write(0);
    delay(10);
  }
  else {
    windowServo.write(176);
    delay(10);
  }
}

void setupPins() 
{
  pinMode(LEFT_BTN_PIN, INPUT);
}

void attachServo() 
{
  windowServo.attach(WINDOW_SERVO_PIN);
  windowServo.write(176);
  delay(200);
}


