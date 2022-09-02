#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT22

int RelayFan = 4; // D2
int RelayPum = 0; // D3

float humids[2];
float temps[2];

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht[] = {{5, DHTTYPE}, {16, DHTTYPE}};
// Replace the next variables with your SSID/Password combination
const char* ssid = "MakerboxLao";
const char* password = "asdasdasd";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void callback(char* topic, byte* message, unsigned int length);
void setup_wifi();
void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(RelayFan, OUTPUT);
   pinMode(RelayPum, OUTPUT);
   for (auto &sensor : dht)
  {
    sensor.begin();
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");

    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    // digitalWrite(RelayFan, 1);
      // digitalWrite(RelayPum, 1); 
      // Serial.println("onFAN");
      // delay(50000);
      // digitalWrite(RelayFan, LOW);
      // digitalWrite(RelayPum, LOW);
      // Serial.println("off");
      // delay(5000);
      for (int index = 0; index < 2; index++)
  {
    humids[index] = dht[index].readHumidity();
    temps[index] = dht[index].readTemperature();
  }

  if (isnan(humids[0])){
    Serial.println("\nHumindity 1 is not detected");
  }else {
    Serial.printf("humid 1: %f", humids[0]);
    Serial.printf("\ttemp 1: %f", temps[0]);
  }
   if (isnan(humids[1])){
    Serial.println("\nHumindity 1 is not detected");
  }else {
    Serial.printf("humid 2: %f", humids[1]);
    Serial.printf("\t\ntemp 2: %f", temps[1]);
  }
  delay(5000);

    
  }
}
