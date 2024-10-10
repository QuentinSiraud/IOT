#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27, 16, 2);  // Initialisation du LCD I2C

#include <ESP32Servo.h>
Servo myservo;
#include <Wire.h>
#include "MFRC522_I2C.h"

// IIC pins default to GPIO21 and GPIO22 of ESP32
// 0x28 is the i2c address of SDA, if doesn't match，please check your address with i2c.
MFRC522_I2C mfrc522(0x28, -1, &Wire);  // 0x28 est l'adresse I2C, -1 pour la broche de reset, &Wire pour l'instance du bus I2C.

#define servoPin 13
#define btnPin 16
boolean btnFlag = 0;
String password = "";

void setup() {
  Serial.begin(115200);           // Initialisation de la communication série avec le PC
  mylcd.init();
  mylcd.backlight();
  Wire.begin();                   // Initialisation de l'I2C
  mfrc522.PCD_Init();             // Initialisation du module MFRC522
  ShowReaderDetails();            // Affiche les détails du lecteur RFID
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
  myservo.attach(servoPin);
  pinMode(btnPin, INPUT);
  mylcd.setCursor(0, 0);
  mylcd.print("Card");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    password = "";
    if (btnFlag == 1) {
      boolean btnVal = digitalRead(btnPin);
      if (btnVal == 0) {  // Si le bouton est pressé, fermer la porte
        Serial.println("close");
        mylcd.setCursor(0, 0);
        mylcd.print("close");
        myservo.write(0);  // Fermer la porte
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
  if (password == "17121741227") {  // Numéro de carte valide
    Serial.println("open");
    mylcd.setCursor(0, 0);
    mylcd.clear();
    mylcd.print("open");
    myservo.write(180);  // Ouvrir la porte
    password = "";
    btnFlag = 1;
  } else {  // Numéro de carte incorrect
    password = "";
    mylcd.setCursor(0, 0);
    mylcd.print("error");
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
