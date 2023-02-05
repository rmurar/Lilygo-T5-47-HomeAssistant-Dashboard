#include <Arduino.h>

#include "configurations.h"

// Change to your WiFi credentials
const char* ssid        = "iBulik";
const char* password    = "iot123987";

// url to HA server. Only http is supported for now!
const String ha_server  = "http://192.168.70.16:8123";
// create a long lived access token and put it here. ref: https://www.home-assistant.io/docs/authentication/
const String ha_token   = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI1MzBkZjhmNDI3NTY0ODgwYTIxNDI5ZDJkOWExNzZlMyIsImlhdCI6MTY3NDkzMDgxMSwiZXhwIjoxOTkwMjkwODExfQ.IwteWvh1e_TKIiL4IKGRYp8tXmEayWmg2HGwNMA6fdw";

// GMT Offset in seconds. UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically -18000, AU is typically (+8hrs) 28800
int   gmtOffset_sec     = 3600;