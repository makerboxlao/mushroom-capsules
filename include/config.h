#ifndef __CONFIG__
#define __CONFIG__
#define __DEVELOPMENT__
// #define __PRODUCTION__
// WiFi config
const char *ssid = "MUSHROOM01"; // Enter your WiFi name
const char *password = "asdasdasd"; // Enter WiFi password
// client Info
// const char *mqtt_server = "45.32.111.51";
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
// Topics Machine For chat
/*Fan Controller Topic*/
const char *fan_control_topic = "****"; // Sub Controller Machine Topic
const char *fan_control_start = "1";
const char *fan_control_stop = "0";
// publish the machine status topic
#define PUB_Topic_in "in/dht/mushroom"
#define PUB_Topic_out "out/dht/mushroom"
//subscribe the machine status topic 
const char *SUB_FAN = "mlb/esp32/fan";
const char *SUB_PUM = "mlb/esp32/pum";
const char *SUB_AutoSW = "mlb/esp32/SWauto";
#endif
