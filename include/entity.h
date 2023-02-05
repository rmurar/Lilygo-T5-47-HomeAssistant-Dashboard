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

    enum class TouchType
    {
        NO_TOUCH,
        SINGLE
    };

    Entity(String name, String id, EntityType type)
    : m_entityType(type),
      m_name(name), m_id(id),
      m_touchType(TouchType::NO_TOUCH),
      m_changed(false)
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

    void SetTouchType(TouchType type)
    {
        m_touchType = type;
    }

    TouchType GetTouchType() const
    {
        return m_touchType;
    }

    bool IsInRectangle(int x, int y) const
    {
        if((x > m_rectangle.x) && (x < (m_rectangle.x + m_rectangle.width)) &&
           (y > m_rectangle.y) && (y < (m_rectangle.y + m_rectangle.height))
          )
        {
            return true;
        }

        return false;
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
    TouchType m_touchType;

    bool m_changed;

};

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

    EntityState GetState() const
    {
        return m_state;
    }

    void SetState(const EntityState& state, bool changed)
    {
        m_state = state;
        m_changed = changed;
    }

    void Draw(Entity::DrawType type = Entity::DrawType::NORMAL);   

protected:
    ActuatorType m_actuatorType;
    EntityState m_state;

};

#endif // __ENTITY_H__