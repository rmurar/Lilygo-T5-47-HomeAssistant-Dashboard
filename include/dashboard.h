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

private:
    void ClearLists();

    std::vector<Sensor *> m_SensorsList;
    std::vector<Sensor *> m_FloatSensorsList;
    std::vector<Actuator *> m_AcuatorsList;
};


#endif // __DASHBOARD_H__