#include <Arduino.h>

#define FAN 2

void setup() {
  pinMode(FAN, OUTPUT);
}

void loop() {
  digitalWrite(FAN, HIGH);
  delay(2000);
  digitalWrite(FAN, LOW);
}
