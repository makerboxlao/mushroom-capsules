#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <WiFiManager.h>

#define DHTTYPE DHT22

#define Topic_in "in/dht/mushroom"
#define Topic_out "out/dht/mushroom"

int RelayFan = 4; // D20
int RelayPum = 0; // D3
int LEDwifi = 2;
int LEDmqtt = 19;
int Reset = 16;

float humids[2];
float temps[2];
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht[] = {{5, DHTTYPE}, {16, DHTTYPE}};
// Replace the next variables with your SSID/Password combination
const char *ssid = "MakerboxLao";
const char *password = "asdasdasd";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.43.130";
const char *mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void callback(char *topic, byte *message, unsigned int length);
void setup_wifi();
String data_in;
String data_out;
WiFiManager wm;
bool res;
void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(RelayFan, OUTPUT);
  pinMode(RelayPum, OUTPUT);
  pinMode(LEDwifi, OUTPUT);
  pinMode(LEDmqtt, OUTPUT);
  pinMode(Reset, INPUT);
  for (auto &sensor : dht)
  {
    sensor.begin();
  }
    
}
void setup_wifi(){
  res = wm.autoConnect("ESP32_MLB","asdasdasd"); 
  if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
 
        Serial.println("connected");
        Serial.println(WiFi.SSID());
        digitalWrite(LEDwifi, HIGH);
    }
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
}
// void setup_wifi()
// {
//   delay(10);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//     digitalWrite(LEDwifi, LOW);
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   digitalWrite(LEDwifi, HIGH);
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT
  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "mlb/esp32/pum")
  {
    Serial.print("Changing output to ");
    if (messageTemp == "on")
    {
      digitalWrite(RelayFan, HIGH);
      Serial.println("\nonfan");
    }
    else if (messageTemp == "off")
    {
      digitalWrite(RelayFan, LOW);
      Serial.println("\nofffan");
    }
  }
  else if (String(topic) == "mlb/esp32/fan")
  {
    Serial.print("Changing output to ");
    if (messageTemp == "on")
    {
      digitalWrite(RelayPum, HIGH);
      Serial.println("\nonpum");
    }
    else if (messageTemp == "off")
    {
      digitalWrite(RelayPum, LOW);
      Serial.println("\noffpum");
    }
  }
}

String macToStr(const uint8_t *mac)
{
  String result;
  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Generate client name based on MAC address and last 8 bits of microsecond counter
    String clientName;
    clientName += "esp8266-";
    uint8_t mac[6];
    WiFi.macAddress(mac);
    clientName += macToStr(mac);
    clientName += "-";
    clientName += String(micros() & 0xff, 16);
    Serial.print("Connecting to ");
    Serial.print(mqtt_server);
    Serial.print(" as ");
    Serial.println(clientName);
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    if (client.connect((char *)clientName.c_str()))
    {
      // if (client.connect((char*) clientName.c_str()), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("mlb/esp32/fan");
      client.subscribe("mlb/esp32/pum");
      digitalWrite(LEDmqtt, HIGH);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      digitalWrite(LEDmqtt, LOW);
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
   if (digitalRead(Reset) == HIGH) 
  {
    Serial.println("reset wifi and restart...!");
    wm.resetSettings();
    ESP.restart();
    digitalWrite(LEDwifi, HIGH);
  }
  long now = millis();
  if (now - lastMsg > 60000)
  { 
    lastMsg = now;
    for (int index = 0; index < 2; index++)
    {
      humids[index] = dht[index].readHumidity();
      temps[index] = dht[index].readTemperature();
    }
    String Humin = String(humids[0]).c_str();
    String Temin = String(temps[0]).c_str();
    String Humout = String(humids[1]).c_str();
    String Temout = String(temps[1]).c_str();

    if (isnan(humids[0]))
    {
      Serial.println("\nHumindity 1 is not detected");
    }
    else
    {
      Serial.printf("\nhumid 1: %f", humids[0]);
      Serial.printf("\ttemp 1: %f", temps[0]);
      data_in = "{\"humidity\":" + Humin + ",\"temperature\":" + Temin + "}";
      Serial.print("\n");
      Serial.print(data_in);
      data_in.toCharArray(msg, (data_in.length() + 1));
      client.publish(Topic_in, msg);
    }
    if (isnan(humids[1]))
    {
      Serial.println("\nHumindity 2 is not detected");
    }
    else
    {
      Serial.printf("\nhumid 2: %f", humids[1]);
      Serial.printf("\ttemp 2: %f", temps[1]);
      data_out = "{\"humidity\":" + Humout + ", \"temperature\":" + Temout + "}";
      Serial.print("\n");
      Serial.print(data_out);
      data_out.toCharArray(msg, (data_out.length() + 1));
      client.publish(Topic_out, msg);
    }
  }
}
