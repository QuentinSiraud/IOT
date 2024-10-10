#include <PinList.h>

void setup()
{
    Serial.begin(9600);
    pinMode(LEFT_BTN_PIN, INPUT);
}

void loop()
{
    if (!digitalRead(LEFT_BTN_PIN) )
    {
        Serial.println("Bouton gauche appuyé");
    }
    else
    {
        Serial.println("Bouton gauche relâché");
    }
    delay(10);
}