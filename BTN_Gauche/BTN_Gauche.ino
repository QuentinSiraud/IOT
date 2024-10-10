#include <ESP32Servo.h>
Servo myservo;


const int WINDOW_SERVOR_PIN = 2; // Anciennement 5
const int LEFT_BTN_PIN = 16; // 👉
bool status_Window = false;

void setup() {
  pinMode(LEFT_BTN_PIN, INPUT);
  Serial.begin(9600);
  myservo.attach(WINDOW_SERVOR_PIN);
  myservo.write(176);
  delay(200);
}

void loop() {
  changeStatus();
  OpenWindow();
}

void changeStatus()
{
  if(!digitalRead(LEFT_BTN_PIN)){
    status_Window = !status_Window;
  }
}

void OpenWindow()
{
  if(status_Window == false){
  myservo.write(0);
  delay(10);
  }
  else {
  myservo.write(176);
  delay(10);
  }
}

