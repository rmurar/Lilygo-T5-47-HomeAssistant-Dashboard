#ifndef __HOMEASSISTANTAPI_H__
#define __HOMEASSISTANTAPI_H__

#include <Arduino.h>
#include "defines.h"


EntityState checkOnOffState(String entity);
HAConfigurations getHaStatus();
String getSensorValue(String entity);
String getSensorAttributeValue(String entity, String attribute);
float getSensorFloatValue(String entity);

void setOnOffState(String entity, bool on);

#endif // __HOMEASSISTANTAPI_H__