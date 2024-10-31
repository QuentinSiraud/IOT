#include <PinList.h>

void setup()
{
  Serial.begin(9600);
  pinMode(MOTION_SENSOR_PIN, INPUT);
}

void loop()
{
    if (digitalRead(MOTION_SENSOR_PIN) )
    {
      Serial.println("Il y a du mouvement");
    }
    else
    {
      Serial.println("Il n'y a pas de mouvement");
    }
    delay(10);
}