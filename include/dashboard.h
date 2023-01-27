#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include <vector>

#include "entity.h"
#include "defines.h"

class Dashboard
{
public:
    Dashboard();
    virtual ~Dashboard();

    void ParseConfiguration();    

    void DrawSwitchBar();
    void DrawSensorBar();
    void DrawBottomBar();
    void DisplayStatusSection(int rssi);
    void DrawWifiErrorScreen(int rssi);
    void DisplayGeneralInfoSection(String dayStamp, String timeStamp);

    void DrawDashboard(int rssi, String dayStamp, String timeStamp);

private:
    void ClearLists();

    std::vector<Sensor *> m_SensorsList;
    std::vector<Sensor *> m_FloatSensorsList;
    std::vector<Actuator *> m_AcuatorsList;
};


#endif // __DASHBOARD_H__