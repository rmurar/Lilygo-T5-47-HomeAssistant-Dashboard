#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "entity.h"

class Sensor: public Entity
{
public:
    union SensorValue
    {
        EntityState m_state;
        float m_floatValue;
    };

    Sensor(String name, String id, SensorValueType valueType, SensorType type)
    : Entity(name, id, EntityType::SENSOR),
      m_valueType(valueType), m_sensorType(type)
    {

    }

    SensorType GetType() const
    {
        return m_sensorType;
    }

    SensorValueType GetValueType() const
    {
        return m_valueType;
    }

    bool GetState(EntityState& state)
    {
        if(m_valueType == SensorValueType::ONOFF)
        {
            state = m_value.m_state;
            return true;
        }

        return false;
    }


    bool GetValue(float& value)
    {
        if(m_valueType == SensorValueType::VALUE)
        {
            value = m_value.m_floatValue;
            return true;
        }

        return false;
    }

    void SetState(const EntityState& state, bool changed)
    {
        if(m_valueType == SensorValueType::ONOFF)
        {
            m_value.m_state = state;
            m_changed = changed;
        }
    }

    void SetValue(float value, bool changed)
    {
        if(m_valueType == SensorValueType::VALUE)
        {
            m_value.m_floatValue = value;
            m_changed = changed;
        }
    }

    void Draw(Entity::DrawType type = Entity::DrawType::NORMAL);


protected:
    SensorValue m_value;
    SensorValueType m_valueType;
    SensorType m_sensorType;
};

#endif // __SENSOR_H__