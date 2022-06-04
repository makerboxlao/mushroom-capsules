#include <Arduino.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT22

float humids[2];
float temps[2];

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht[] = {{4, DHTTYPE}, {5, DHTTYPE}};

void setup() {
  Serial.begin(115200);

  for (auto &sensor : dht) {
      sensor.begin();
  }
}

void loop() {
  for (int index = 0; index < 2; index++) {
      humids[index] = dht[index].readHumidity();
      temps[index] = dht[index].readTemperature();
  }

  Serial.printf("humid 1: %f", humids[0]);
  Serial.printf("\thumid 2: %f", humids[1]);
  Serial.printf("\ttemp 1: %f", temps[0]);
  Serial.printf("\ttemp 2: %f\n", temps[1]);

  delay(2000);
}