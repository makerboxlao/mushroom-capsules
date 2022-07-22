#include <Arduino.h>
#include <Adafruit_Sensor.h>

int RelayFan = 4;//D2
int RelayPum = 0;//D3
// const int Relay2 = D7;

void setup() {
 Serial.begin(115200);
 Serial.println("Relay Test");

 pinMode(RelayFan,OUTPUT);
//  pinMode(Relay2,OUTPUT);

}
void loop() {
   digitalWrite(RelayFan,HIGH);
   digitalWrite(RelayPum,HIGH);
   Serial.println("on");
   delay(20000);
digitalWrite(RelayFan,LOW);
digitalWrite(RelayPum,LOW);
Serial.println("off");
delay(20000);
}
