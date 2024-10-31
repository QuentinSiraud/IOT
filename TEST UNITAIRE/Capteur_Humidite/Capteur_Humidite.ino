#include <DHT.h>
#include <PinList.h>
#define DHTTYPE DHT11
DHT dht(HUMIDITY_SENSOR_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  Serial.println("Humidite : " + String(dht.readHumidity(), 3));
  Serial.println("Temperature : " + String(dht.readTemperature(), 1 + "°C"));
  delay(1000);
}
