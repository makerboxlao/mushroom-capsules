
#include <Arduino.h>

#define RESET 16
int state;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RESET,INPUT);
 
}
void loop() {
  state = digitalRead(RESET);
  Serial.println(state);
}