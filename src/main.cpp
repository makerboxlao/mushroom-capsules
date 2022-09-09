#include <WiFiManager.h>


void setup() {

    Serial.begin(115200);
    
    WiFiManager wm;
    // wm.resetSettings();

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("ESP32_MLB","asdasdasd"); 

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
 
        Serial.println("connected");
        Serial.println(WiFi.SSID());
    }

}

void loop(){}