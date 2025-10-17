#include <Arduino.h>
#include <DHT.h>
#define DHT_PIN 32
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
 Serial.begin(9600);
 dht.begin();
}

void loop() {
 float h = dht.readHumidity();
  float t = dht.readTemperature();
 Serial.println("Temp: "+String(t)+"; Hum: "+String(h));
 delay(1000);
}
