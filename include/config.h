#ifndef __CONFIG__
#define __CONFIG__
#define __DEVELOPMENT__
// #define __PRODUCTION__
// WiFi config
const char *ssid = "MakerboxLao"; // Enter your WiFi name
const char *password = "asdasdasd"; // Enter WiFi password
// client Info
const char *mqtt_server = "45.32.111.51";
const int mqtt_port = 1883;
// Topics Machine For chat
/*Fan Controller Topic*/
const char *fan_control_topic = "****"; // Sub Controller Machine Topic
const char *fan_control_start = "1";
const char *fan_control_stop = "0";
// publish the machine status topic
#endif
