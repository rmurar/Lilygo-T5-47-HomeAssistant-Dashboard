
// wifi
#include <WiFi.h>

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

const gpio_num_t Dashboard::TOUCH_PANEL_INT = GPIO_NUM_13;

RTC_DATA_ATTR EntityState Dashboard::actuatorsStateArray[MAX_ACTUATOR_COUNT];
RTC_DATA_ATTR EntityState Dashboard::sensorsStateArray[MAX_SENSORS_COUNT];
RTC_DATA_ATTR float Dashboard::floatSensorsStateArray[MAX_FLOAT_SENSORS_COUNT];

Dashboard::Dashboard()
: m_flagTouchEvent(false)
{
    m_SensorsList.reserve(20);
    m_FloatSensorsList.reserve(20);
    m_AcuatorsList.reserve(20);

}

Dashboard::~Dashboard()
{

}

void Dashboard::Init()
{
/*
    Serial.begin(115200);
*/
    pinMode(TOUCH_PANEL_INT, INPUT_PULLDOWN);

    Wire.begin(15, 14);

    if (!m_touchClass.begin()) {
        Serial.println("start touchscreen failed");
    }
    else
    {
        Serial.println("start touchscreen OK");
    }

    if(!IsDeepSleepWakeupReason())
    {
        ClearRTCData();
    }

    ParseConfiguration();

    InitSwitchBar();
    InitSensorBar();

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

void Dashboard::InitSwitchBar()
{
    Rect_t area;

    int x = 3;
    int y = 23;
    int i = 0;
    for(Actuator* actuator: m_AcuatorsList){
        if(actuator->GetName() != "") {
            area = GetTileRect(x, y);
        }
      
        x = x + TILE_WIDTH; // move column right
        if (i == 5) { // move row down
            x = 3;
            y = y + TILE_HEIGHT;
        }

        actuator->SetRectangle(area);

        i++;
    }
}

void Dashboard::InitSensorBar()
{
    Rect_t area;

    int x = 3;
    int y = 345;
    for(Sensor* sensor: m_SensorsList)
    {
        if (sensor->GetName() != "")
        {
            area = GetSensorTileRect(x, y);
        }

        sensor->SetRectangle(area);

        x = x + SENSOR_TILE_WIDTH;
    }
}

void Dashboard::DrawSwitchBar()
{
    for(Actuator* actuator: m_AcuatorsList){
        actuator->Draw();
    }
}

void Dashboard::DrawSensorBar()
{
    for(Sensor* sensor: m_SensorsList)
    {
        sensor->Draw();
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


void Dashboard::GetValuesDashboard()
{

    EntityState state;
    SensorValueType value_type;
    float value;
    int index1, index2;
    bool changed;
    bool storeInitialValues = false;

    if(!IsDeepSleepWakeupReason())
    {
        //power cycle
        storeInitialValues = true;
    }

    index1 = 0;
    for(Actuator* actuator: m_AcuatorsList){
        state = checkOnOffState(actuator->GetId());

        changed = false;
        if(index1 < MAX_ACTUATOR_COUNT)
        {        
            if(storeInitialValues || (actuatorsStateArray[index1] != state))
            {
                //store initial value after power cycle
                //or store last known state
                actuatorsStateArray[index1] = state;
                changed = true;
            }
        }
        
        actuator->SetState(state, changed);
        index1++;
    }

    index1 = 0;
    index2 = 0;
    for(Sensor* sensor: m_SensorsList)
    {
        value_type = sensor->GetValueType();
        if(value_type == SensorValueType::ONOFF)
        {
            state = checkOnOffState(sensor->GetId());

            changed = false;
            if(index1 < MAX_SENSORS_COUNT)
            {
                if(storeInitialValues || (sensorsStateArray[index1] != state))
                {
                    //store initial value after power cycle
                    //or store last known state
                    sensorsStateArray[index1] = state;
                    changed = true;                    
                }
            }

            sensor->SetState(state, changed);
            index1++;
        }
        else if(value_type == SensorValueType::VALUE)
        {
            value = getSensorFloatValue(sensor->GetId());

            changed = false;
            if(index2 < MAX_FLOAT_SENSORS_COUNT)
            {
                if(storeInitialValues || (floatSensorsStateArray[index2] != value))
                {
                    //store initial value after power cycle
                    //or store last known state
                    floatSensorsStateArray[index2] = value;
                    changed = true;                    
                }                
            }

            sensor->SetValue(value, changed);
            index2++;
        }
    }


}

void Dashboard::DrawDashboard(int rssi, String dayStamp, String timeStamp)
{
#if 1
    epd_poweron();

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

    epd_poweroff();
#else
    epd_poweron();

    Rect_t area = {.x = 0, .y = 0, .width = EPD_WIDTH, .height = 20};
    epd_clear_area(area);

    DisplayStatusSection(rssi);
    DisplayGeneralInfoSection(dayStamp, timeStamp);

    epd_update_area(area);


    Serial.println("Drawing switchBar...");
    DrawSwitchBar();
/*
    Serial.println("Drawing sensorBar...");
    DrawSensorBar();
    Serial.println("Drawing bottomBar...");
    DrawBottomBar();
*/

    epd_poweroff();
#endif
}

void Dashboard::RefreshDashboard()
{
    int wifi_signal = WiFi.RSSI();

    GetValuesDashboard();
    DrawDashboard(wifi_signal, "", "");
}

void Dashboard::ScanTouchEvent()
{
    uint16_t x, y;
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause();    

    m_flagTouchEvent = false;


    if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT0)
    {
        if(m_touchClass.scanPoint())
        {
            m_touchClass.getPoint(x, y, 0);
            y = EPD_HEIGHT - y;

            Serial.printf("%ld: x=%d, y=%d\n", millis(), x, y);
        
            for(Actuator* actuator: m_AcuatorsList){
                if(actuator->IsInRectangle(x, y))
                {
                    actuator->SetTouchType(Entity::TouchType::SINGLE);
                    Serial.printf("Touch event on entity: %s\n", actuator->GetId().c_str());
                    m_flagTouchEvent = true;
                }
            }
        }
    }
}

void Dashboard::HandleTouchEvent()
{
    if(m_flagTouchEvent)
    {
        for(Actuator* actuator: m_AcuatorsList)
        {
            actuator->HandleTouchEvent();
        }
    }
}

void Dashboard::ClearRTCData()
{
    int index;

    for(index = 0; index < MAX_ACTUATOR_COUNT; index++)
    {
        actuatorsStateArray[index] = EntityState::UNKNOWN;
    }

    for(index = 0; index < MAX_SENSORS_COUNT; index++)
    {
        sensorsStateArray[index] = EntityState::UNKNOWN;
    }

    for(index = 0; index < MAX_FLOAT_SENSORS_COUNT; index++)
    {
        floatSensorsStateArray[index] = 0.0;
    }

}

bool Dashboard::IsDeepSleepWakeupReason()
{
    bool retval = false;

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason){
        case ESP_SLEEP_WAKEUP_EXT0 : 
        case ESP_SLEEP_WAKEUP_EXT1 : 
        case ESP_SLEEP_WAKEUP_TIMER : 
        case ESP_SLEEP_WAKEUP_TOUCHPAD : 
        case ESP_SLEEP_WAKEUP_ULP : retval = true; break;
        default : break;
    }    

    return retval;
}

bool Dashboard::IsTouchEvent()
{
    return m_flagTouchEvent;
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

