#include <Arduino.h>
const int servoPin = 18;
const int ledChannel = 0;
// Частота ШИМ для серво: 50 Гц
const int pwmFreq = 50;
const int resolution = 10;

void setup() {
  ledcSetup(ledChannel, pwmFreq, resolution);
  ledcAttachPin(servoPin, ledChannel);
}

void loop() {
  ledcWrite(ledChannel, 26);
  delay(1000);
  ledcWrite(ledChannel, 60);
  delay(1000);
}
