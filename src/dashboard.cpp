
// epd
#include "epd_driver.h"

// font
#include "opensans8b.h"
#include "opensans9b.h"
#include "opensans10b.h"
#include "opensans12b.h"
#include "opensans18b.h"
#include "opensans24b.h"

#include "homeassistantapi.h"
#include "epd_drawing.h"

// Icons for Home Assistant
#include "icons/waterheateron.h"
#include "icons/waterheateroff.h"
#include "icons/lightbulbon.h"
#include "icons/lightbulboff.h"
#include "icons/exhaustfanon.h"
#include "icons/exhaustfanoff.h"
#include "icons/fanoff.h"
#include "icons/fanon.h"
#include "icons/airpurifieron.h"
#include "icons/airpurifieroff.h"
#include "icons/plugon.h"
#include "icons/plugoff.h"
#include "icons/switchon.h"
#include "icons/switchoff.h"
#include "icons/airconditioneron.h"
#include "icons/airconditioneroff.h"
#include "icons/warning.h"
#include "icons/plantwateringlow.h"
#include "icons/plantwateringok.h"

// sensor icons
#include "icons/dooropen.h"
#include "icons/doorclosed.h"
#include "icons/motionsensoron.h"
#include "icons/motionsensoroff.h"
#include "icons/sensorerror.h"

#include "dashboard.h"
#include "draw.h"
#include "configurations.h"

Dashboard::Dashboard()
{
    m_SensorsList.reserve(20);
    m_FloatSensorsList.reserve(20);
    m_AcuatorsList.reserve(20);

}

Dashboard::~Dashboard()
{

}

void Dashboard::ParseConfiguration()
{
    int i;
    uint16_t actuatorsCount = sizeof(haActuators) / sizeof(ConfigActuator);
    uint16_t sensorsCount = sizeof(haSensors) / sizeof(ConfigSensor);
    uint16_t floatSensorsCount = sizeof(haFloatSensors) / sizeof(ConfigSensor);

    ClearLists();

    for(i = 0; i < actuatorsCount; i++)
    {
        m_AcuatorsList.push_back(new Actuator(haActuators[i].name, haActuators[i].id, haActuators[i].type));
    }

    for(i = 0; i < sensorsCount; i++)
    {
        m_SensorsList.push_back(new Sensor(haSensors[i].name, haSensors[i].id, haSensors[i].valueType, haSensors[i].type));
    }

    for(i = 0; i < floatSensorsCount; i++)
    {
        m_FloatSensorsList.push_back(new Sensor(haFloatSensors[i].name, haFloatSensors[i].id, haFloatSensors[i].valueType, haFloatSensors[i].type));
    }

}

void Dashboard::DrawBottomBar()
{
    int tiles = 3;
    float totalEnergy = 0;
    float totalPower  = 0;
    String totalEnergyName;
    String totaPowerName;
    for(Sensor* sensor : m_FloatSensorsList)
    {
        if(sensor->GetType() == SensorType::ENERGYMETER)
        {
            totalEnergy = totalEnergy + getSensorFloatValue(sensor->GetId());
            totalEnergyName = sensor->GetName();
        }
        else if(sensor->GetType() == SensorType::ENERGYMETERPWR)
        {
            totalPower = totalPower + getSensorFloatValue(sensor->GetId());
            totaPowerName = sensor->GetName();
        }
    }
    int x = 3;
    int y = 456;
    // first one 
    if (totalEnergy != 0)
    {
        DrawBottomTile(x, y, String(totalEnergy) + " kWh", totalEnergyName);
        x = x + BOTTOM_TILE_WIDTH;
        tiles--;
    }
    if (totalPower != 0)
    {
        DrawBottomTile(x, y, String((int)totalPower) + " W", totaPowerName);
        x = x + BOTTOM_TILE_WIDTH;
        tiles--;
    }
    
    for(Sensor* sensor : m_FloatSensorsList)
    {
        if ((sensor->GetType() == SensorType::TEMP) && (tiles >= 1))
        {
            float temp = getSensorAttributeValue(sensor->GetId(), "current_temperature").toFloat();
            if (temp == 0)
                temp = getSensorValue(sensor->GetId()).toFloat();
            DrawBottomTile(x, y, String(temp, 1) + "° C", sensor->GetName());
            x = x + BOTTOM_TILE_WIDTH;
            tiles--;
        }
    }
}

void Dashboard::DrawSwitchBar()
{
    setFont(OpenSans9B);
    int x = 3;
    int y = 23;
    int i = 0;
    for(Actuator* actuator: m_AcuatorsList){
        if(actuator->GetName() != "") {
            ActuatorType type = actuator->GetType();

          if (type == ActuatorType::SWITCH ||
              type == ActuatorType::LIGHT ||
              type == ActuatorType::EXFAN ||
              type == ActuatorType::FAN ||
              type == ActuatorType::AIRPURIFIER ||
              type == ActuatorType::WATERHEATER ||
              type == ActuatorType::AIRCONDITIONER)
          {            
              DrawTile(x, y, checkOnOffState(actuator->GetId()), actuator->GetType(), actuator->GetName(), "");
          }
          else 
          {
              String val = getSensorValue(actuator->GetId());
              DrawTile(x, y, ActuatorState::UNAVAILABLE, actuator->GetType(), actuator->GetName(), val);
          }
        }
      
        x = x + TILE_WIDTH; // move column right
        if (i == 5) { // move row down
            x = 3;
            y = y + TILE_HEIGHT;
        }

        i++;
    }
}

void Dashboard::DrawSensorBar()
{
    setFont(OpenSans9B);
    int x = 3;
    int y = 345;
    for(Sensor* sensor: m_SensorsList)
    {
        SensorType type = sensor->GetType();
        if (type == SensorType::DOOR ||
            type == SensorType::MOTION )
        {
      if (sensor->GetName() != "")
            DrawSensorTile(x,y,checkOnOffState(sensor->GetId()), sensor->GetType(), sensor->GetName());
        }
        x = x + SENSOR_TILE_WIDTH;
    }
}

void Dashboard::DisplayStatusSection(int rssi)
{
  setFont(OpenSans8B);
  DrawBattery(5, 18);
  DrawRSSI(900, 18, rssi);
}

void Dashboard::DrawWifiErrorScreen(int rssi)
{
    epd_clear();
    DisplayStatusSection(rssi);
    epd_update();
}

void Dashboard::DisplayGeneralInfoSection(String dayStamp, String timeStamp)
{
    setFont(OpenSans8B);
    Serial.println("Getting haStatus...");
    HAConfigurations haConfigs = getHaStatus();
    Serial.println("drawing status line...");    
    drawString(EPD_WIDTH/2, 18, dayStamp + " - " +  timeStamp + " (HA Ver:" + haConfigs.version + "/" + haConfigs.haStatus + ", TZ:" + haConfigs.timeZone + ")", CENTER);
}

void Dashboard::DrawDashboard(int rssi, String dayStamp, String timeStamp)
{
    epd_clear();
    
    DisplayStatusSection(rssi);
    DisplayGeneralInfoSection(dayStamp, timeStamp);
    Serial.println("Drawing switchBar...");
    DrawSwitchBar();
    Serial.println("Drawing sensorBar...");
    DrawSensorBar();
    Serial.println("Drawing bottomBar...");
    DrawBottomBar();

    epd_update();
}


void Dashboard::ClearLists()
{
    for(Sensor *sensor : m_SensorsList)
    {
        delete(sensor);
    }
    
    m_SensorsList.clear();
    m_SensorsList.reserve(20);

    for(Sensor *sensor : m_FloatSensorsList)
    {
        delete(sensor);
    }
    
    m_FloatSensorsList.clear();
    m_FloatSensorsList.reserve(20);

    for(Actuator *actuator : m_AcuatorsList)
    {
        delete(actuator);
    }

    m_AcuatorsList.clear();
    m_AcuatorsList.reserve(20);

}

