#include <Arduino.h>

#include "configurations.h"

// Change to your WiFi credentials
const char* ssid        = "iBulik";
const char* password    = "iot123987";

// url to HA server. Only http is supported for now!
const String ha_server  = "http://192.168.88.147:8123";
// create a long lived access token and put it here. ref: https://www.home-assistant.io/docs/authentication/
const String ha_token   = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI3ZDkxODMxYWE5NzM0NWZmODcyODE4NmRiYTEyZGIyMiIsImlhdCI6MTY3NDQxMzEzNywiZXhwIjoxOTg5NzczMTM3fQ.dwrZ7Fu8p5pALr-z5EMa-BDkrfMiNtKPjibpwGeaRkI";

// GMT Offset in seconds. UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically -18000, AU is typically (+8hrs) 28800
int   gmtOffset_sec     = 3600;