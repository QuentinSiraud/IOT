#ifndef LCD_DISPLAY_h
#define LCD_DISPLAY_h

// 📚 Librairies
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 📌 Définir les constantes pour les durées
const int SHORT_DURATION = 3000;   // 3s en ms
const int LONG_DURATION = 5000;    // 5s en ms

// 📌 Initialisation de l'écran LCD (adresse 0x27, 16 colonnes et 2 lignes)
LiquidCrystal_I2C mylcd(0x27, 16, 2);

// 📌 Fonction pour configurer l'écran LCD
void setupLCD() {
  mylcd.init();
  mylcd.backlight();
}

// 📌 Fonction pour mettre à jour l'écran avec un message et une durée
void updateLCDMessage(String message, int duration = SHORT_DURATION) {
  mylcd.clear();
  mylcd.setCursor(0, 0);
  mylcd.print(message);
  delay(duration);
  mylcd.clear();
}

#endif
