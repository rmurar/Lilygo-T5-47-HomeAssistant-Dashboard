// wifi
#include <WiFi.h>

#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "defines.h"

#include "homeassistantapi.h"
#include "configurations.h"

HTTPClient http;
WiFiClientSecure client;

EntityState checkOnOffState(String entity)
{
    String api_url = ha_server + "/api/states/" + entity;
    http.begin(api_url);
    http.addHeader("Authorization", "Bearer " + ha_token);
    int code = http.GET();
    if (code != HTTP_CODE_OK)
    {
        Serial.println("Error '" + String(code) + "' connecting to HA API: " + api_url);
        return EntityState::ERROR;
    }
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, http.getStream());
    http.end();
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return EntityState::ERROR;
    }
    String state = doc["state"];
    Serial.println("  - " + entity + " state: " + state);
    if (state == "on"){
        return EntityState::ON;
    }
    if (state == "unavailable"){
        return EntityState::UNAVAILABLE;
    }
    return EntityState::OFF;
}

void setOnOffState(String entity, bool on)
{
    String strPayload;
    String api_url = ha_server + "/api/states/" + entity;
    http.begin(api_url);
    http.addHeader("Authorization", "Bearer " + ha_token);
    http.addHeader("content-type", "application/json");

    strPayload = "{\"state\":";
    if(on)
    {
        strPayload += "\"on\"";
    }
    else
    {
        strPayload += "\"off\"";
    }
    strPayload += "}";

    int code = http.POST(strPayload);
    if (code != HTTP_CODE_OK)
    {
        Serial.println("Error '" + String(code) + "' connecting to HA API: " + api_url);
    }
    else
    {
        Serial.println("Execute OK:" + api_url);
    }
}

void turnOnOffLight(String entity, bool on)
{
    String strPayload;
    String api_url = ha_server + "/api/services/light/";

    if(on)
    {
        api_url += "turn_on";
    }
    else
    {
        api_url += "turn_off";
    }

    http.begin(api_url);
    http.addHeader("Authorization", "Bearer " + ha_token);
    http.addHeader("content-type", "application/json");

    strPayload = "{\"entity_id\":\"" + entity + "\" }";

    int code = http.POST(strPayload);
    if (code != HTTP_CODE_OK)
    {
        Serial.println("Error '" + String(code) + "' connecting to HA API: " + api_url + "(" + entity + ")");
    }
    else
    {
        Serial.println("Execute OK:" + api_url + "(" + entity + ")");
        
    }    
}

HAConfigurations getHaStatus()
{
    HAConfigurations haConfigs;
    haConfigs.haStatus = "ERROR";
    haConfigs.timeZone = "ERROR";
    haConfigs.version  = "ERROR"; 

    String api_url = ha_server + "/api/config";
    http.begin(api_url);
    http.addHeader("Authorization", "Bearer " + ha_token);
    int code = http.GET();
    if (code != HTTP_CODE_OK)
    {
        http.end();
        Serial.println("Error '" + String(code) + "' connecting to HA API: " + api_url);
        return haConfigs;
    }
    DynamicJsonDocument doc(4096);
    StaticJsonDocument<64> filter;
    // Filter JSON data to save RAM
    filter["time_zone"] = true;
    filter["version"] = true;
    filter["state"] = true;
    DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    http.end();
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return haConfigs;
    }
    String state = doc["state"];
    String timeZone = doc["time_zone"];
    String version = doc["version"];
    haConfigs.haStatus = state;
    haConfigs.timeZone = timeZone;
    haConfigs.version  = version; 
    Serial.println("Home Assistant config: " + state + " - " + timeZone + " - " + version);
    return haConfigs;
}

String getSensorValue(String entity)
{
    String api_url = ha_server + "/api/states/" + entity;
    http.begin(api_url);
    http.addHeader("Authorization", "Bearer " + ha_token);
    int code = http.GET();
    if (code != HTTP_CODE_OK)
    {
        Serial.println("Error '" + String(code) + "' connecting to HA API for: " + api_url);
        return "";
    }
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, http.getStream());
    http.end();
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return "";
    }
    String state = doc["state"];
    Serial.println("  - " + entity + " state: " + state);
    return state;
}

String getSensorAttributeValue(String entity, String attribute)
{
    String api_url = ha_server + "/api/states/" + entity;
    http.begin(api_url);
    http.addHeader("Authorization", "Bearer " + ha_token);
    int code = http.GET();
    if (code != HTTP_CODE_OK)
    {
        Serial.println("Error '" + String(code) + "' connecting to HA API for: " + api_url);
        return "";
    }
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, http.getStream());
    http.end();

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return "";
    }
    String attr = doc["attributes"][attribute];
    Serial.println(entity + "/" + attribute + ": " + attr);
    if (attr != NULL && attr != "" && attr != "null")
    {
        return attr;
    }

    return "";
}

float getSensorFloatValue(String entity)
{
    String state = getSensorValue(entity);
    return state.toFloat();    
}