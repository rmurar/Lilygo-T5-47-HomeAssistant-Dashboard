#ifndef __HOMEASSISTANTAPI_H__
#define __HOMEASSISTANTAPI_H__

#include <Arduino.h>
#include "defines.h"


ActuatorState checkOnOffState(String entity);
HAConfigurations getHaStatus();
String getSensorValue(String entity);
String getSensorAttributeValue(String entity, String attribute);
float getSensorFloatValue(String entity);


#endif // __HOMEASSISTANTAPI_H__