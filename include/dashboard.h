#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include <vector>

#include "entity.h"
#include "defines.h"
#include "touch.h"

class Dashboard
{
public:
    static const gpio_num_t TOUCH_PANEL_INT;

    static RTC_DATA_ATTR bool actuatorsStateArray[MAX_ACTUATOR_COUNT];
    static RTC_DATA_ATTR bool sensorsStateArray[MAX_SENSORS_COUNT];
    static RTC_DATA_ATTR float floatSensorsStateArray[MAX_FLOAT_SENSORS_COUNT];

    Dashboard();
    virtual ~Dashboard();

    void Init();

    void ParseConfiguration();    

    void InitSwitchBar();
    void InitSensorBar();

    void DrawSwitchBar();
    void DrawSensorBar();
    void DrawBottomBar();
    void DisplayStatusSection(int rssi);
    void DrawWifiErrorScreen(int rssi);
    void DisplayGeneralInfoSection(String dayStamp, String timeStamp);

    void DrawDashboard(int rssi, String dayStamp, String timeStamp);

    void ScanTouchPoint();

    void ClearRTCData();
    bool IsDeepSleepWakeupReason();

private:
    void ClearLists();

    std::vector<Sensor *> m_SensorsList;
    std::vector<Sensor *> m_FloatSensorsList;
    std::vector<Actuator *> m_AcuatorsList;

    TouchClass m_touchClass;
};


#endif // __DASHBOARD_H__