#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include "defines.h"
#include "epd_driver.h"

enum class EntityType 
{
    UNKNOWN,
    ACTUATOR,
    SENSOR
};

class Entity
{
public:
    enum class DrawType
    {
        NORMAL,
        INACTIVE,
        PUSHED
    };

    Entity(String name, String id, EntityType type)
    : m_entityType(type),
      m_name(name), m_id(id)
    {

    }

    virtual ~Entity()
    {

    }

    const String& GetName() const
    {
        return m_name;
    }

    const String& GetId() const
    {
        return m_id;
    }

    void SetRectangle(const Rect_t &rect)
    {
        m_rectangle = rect;
    }

    const Rect_t& GetRectangle()
    {
        return m_rectangle;
    }

    virtual void Draw(Entity::DrawType type = Entity::DrawType::NORMAL)
    {
        //empty
    }

protected:
    ::EntityType m_entityType;
    String m_name;
    String m_id;
    Rect_t m_rectangle;

};

class Sensor: public Entity
{
public:
    union SensorValue
    {
        bool m_binaryValue;
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

    bool GetValue(bool& value)
    {
        if(m_valueType == SensorValueType::ONOFF)
        {
            value = m_value.m_binaryValue;
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

    void Draw(Entity::DrawType type = Entity::DrawType::NORMAL);


protected:
    SensorValue m_value;
    SensorValueType m_valueType;
    SensorType m_sensorType;
};




class Actuator: public Entity
{
public:
    Actuator(String name, String id, ActuatorType type)
    : Entity(name, id, EntityType::ACTUATOR),
      m_actuatorType(type)
    {

    }

    ActuatorType GetType() const
    {
        return m_actuatorType;
    }

    ActuatorState GetState() const
    {
        return m_state;
    }



    void Draw(Entity::DrawType type = Entity::DrawType::NORMAL);   

protected:
    ActuatorType m_actuatorType;
    ActuatorState m_state;

};

#endif // __ENTITY_H__