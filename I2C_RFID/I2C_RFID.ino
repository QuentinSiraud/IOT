// 📚 Librairies
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include "MFRC522_I2C.h"

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// IIC pins default to GPIO21 and GPIO22 of ESP32
// 0x28 is the i2c address of SDA, if doesn't match，please check your address with i2c.

// 📌 Pins
const int DOOR_SERVO_PIN = 4; // 🚪
const int LEFT_BTN_PIN = 16; // 👈
const int RGB_RGB_LED_PIN = 26; // 🚦

// 🧱 Macros 
const int LED_COUNT = 4;

// 🌟 Variables
bool ACTIVITY = false;
boolean btnFlag = 0;
String password = "";

// 💻 Prototypes
/* RFID */
void InitPorte();
void Inactivity();
void unlockDoorIfAllowed();
void ShowReaderDetails();
void colorWipe();
Servo windowServo;

Adafruit_NeoPixel strip(LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C mylcd(0x27, 16, 2);  // Initialisation du LCD I2C
MFRC522_I2C mfrc522(0x28, -1, &Wire);  // 0x28 est l'adresse I2C, -1 pour la broche de reset, &Wire pour l'instance du bus I2C.

void setup() {
  Serial.begin(115200);           // Initialisation de la communication série avec le PC
  RFIDSetup();
  InitPorte();
  Inactivity();
}

void loop() {
  Inactivity();
  unlockDoorIfAllowed();
}

void Inactivity() {
    if(ACTIVITY == false){
      colorWipe(strip.Color(255, 230, 0), 50); // Yellow
    }
};

void InitPorte() {
  doorServo.attach(DOOR_SERVO_PIN);
  doorServo.write(0);
  delay(200);
}

void RFIDSetup() {
  mylcd.init();
  mylcd.backlight();
  Wire.begin();                   // Initialisation de l'I2C
  mfrc522.PCD_Init();             // Initialisation du module MFRC522
  ShowReaderDetails();            // Affiche les détails du lecteur RFID
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
  doorServo.attach(DOOR_SERVO_PIN);
  pinMode(LEFT_BTN_PIN, INPUT);
  mylcd.setCursor(0, 0);
  mylcd.print("Card");
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void unlockDoorIfAllowed() {
      if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    password = "";
    if (btnFlag == 1) {
      boolean btnVal = digitalRead(LEFT_BTN_PIN);
      if (btnVal == 0) {  // Si le bouton est pressé, fermer la porte
        Serial.println("close");
        mylcd.setCursor(0, 0);
        mylcd.print("close");
        doorServo.write(0);  // Fermer la porte
        btnFlag = 0;
      }
    }
    return;
  }
  
  // Sélectionner la carte détectée
  // Sauvegarder l'UID
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i]);
    password = password + String(mfrc522.uid.uidByte[i]);
  }

  // Vérifier si l'UID correspond à un mot de passe correct
  if (password == "2418137160") {  // Numéro de carte valide
    Serial.println("open");
    mylcd.setCursor(0, 0);
    mylcd.clear();
    mylcd.print("open");
    colorWipe(strip.Color(  0, 255,   0), 50); // Green
    password = "";
    btnFlag = 1;
    doorServo.write(90);
    delay(10000);
    doorServo.write(0);
    ACTIVITY = false;
  } else {  // Numéro de carte incorrect
    password = "";
    mylcd.setCursor(0, 0);
    mylcd.print("error");
    doorServo.write(0);
    delay(200);
    colorWipe(strip.Color(255,   0,   0), 50); // Red
  }
}

void ShowReaderDetails() {
  // Afficher la version du logiciel MFRC522
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91) {
    Serial.print(F(" = v1.0"));
  } else if (v == 0x92) {
    Serial.print(F(" = v2.0"));
  } else {
    Serial.print(F(" (unknown)"));
  }
  Serial.println("");

  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}