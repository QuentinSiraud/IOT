#include <PinList.h>

void setup()
{
  Serial.begin(9600);
  pinMode(RIGHT_BTN_PIN, INPUT);
}

void loop()
{
    if (!digitalRead(LEFT_BTN_PIN) )
    {
      Serial.println("Bouton droite appuyé");
    }
    else
    {
      Serial.println("Bouton droite relâché");
    }
    delay(10);
}