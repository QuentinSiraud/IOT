#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#include <PinList.h>

Servo windowServo;


bool ACTIVITY = false;
bool gas_status = false;
bool window_status = false;

const int LED_COUNT = 4;

Adafruit_NeoPixel strip(LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  setupPins();
  attachServo();
}

void loop() {
  breath();
  changeStatus();
  openWindow();
}

void changeStatus()
{
  if(!digitalRead(GAS_SENSOR_PIN)) {
    gas_status = true;
  }
  else 
    gas_status = false;
}


void breath()
{
  if(!gas_status) {
    window_status = false;
    delay(10);
  }
  else {
    fan();
    window_status = true;
    colorWipe(strip.Color(255,   0,   0), 50); // Red
    delay(10);
  }
    colorWipe(strip.Color(0,   0,   0), 50); // Red
    analogWrite(FAN_PIN, LOW);
}

void setupPins() 
{
  pinMode(FAN_PIN, OUTPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  
  
  strip.begin();
  strip.show();
  strip.setBrightness(50);
}

void attachServo() 
{
  windowServo.attach(WINDOW_SERVO_PIN);
  windowServo.write(176);
  delay(200);
}

void fan(){
  analogWrite(FAN_PIN, 180);
  delay(1000);
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


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void initColor(){
  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
