#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

#include "entity.h"

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

    void Draw(Entity::DrawType type = Entity::DrawType::NORMAL) override;

    void HandleTouchEvent() override;   

protected:
    ActuatorType m_actuatorType;
    EntityState m_state;

};



#endif // __ACTUATOR_H__