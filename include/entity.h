#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include "defines.h"
#include "epd_driver.h"
#include "draw.h"

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

    Entity(String name, String id, EntityType type, Draw &draw)
    : m_entityType(type),
      m_name(name), m_id(id),
      m_touchType(TouchType::NO_TOUCH),
      m_changed(false), m_refreshRequired(false),
      m_draw(draw)
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

    virtual void HandleTouchEvent()
    {
        //empty
    }

protected:
    ::EntityType m_entityType;
    String m_name;
    String m_id;
    Rect_t m_rectangle;
    TouchType m_touchType;
    ::Draw &m_draw;

    bool m_changed;
    bool m_refreshRequired;

};

#endif // __ENTITY_H__