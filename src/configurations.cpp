#include <Arduino.h>

#include "configurations.h"

// Change to your WiFi credentials
const char* ssid        = "SSID";
const char* password    = "PASSWD";

// url to HA server. Only http is supported for now!
const String ha_server  = "URL";
// create a long lived access token and put it here. ref: https://www.home-assistant.io/docs/authentication/
const String ha_token   = "ACCESS_TOKEN";

// GMT Offset in seconds. UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically -18000, AU is typically (+8hrs) 28800
int   gmtOffset_sec     = 3600;