
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

