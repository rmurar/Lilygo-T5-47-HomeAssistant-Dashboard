#include "actuator.h"

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

#include "draw.h"

void Actuator::Draw(Entity::DrawType type)
{
    bool localUpdate = false;

    setFont(OpenSans9B);
    if(GetName() != "") {
        ActuatorType type = GetType();

        if (type == ActuatorType::SWITCH ||
            type == ActuatorType::LIGHT ||
            type == ActuatorType::EXFAN ||
            type == ActuatorType::FAN ||
            type == ActuatorType::AIRPURIFIER ||
            type == ActuatorType::WATERHEATER ||
            type == ActuatorType::AIRCONDITIONER)
        {   
            if(localUpdate)
            {
                epd_clear_area(m_rectangle);
            }

            DrawTile(m_rectangle.x, m_rectangle.y, m_state, m_actuatorType, m_name, "");

            if(m_touchType == TouchType::SINGLE)
            {
                int x, y, r;
                x = m_rectangle.x + m_rectangle.width - 10;
                y = m_rectangle.y + m_rectangle.height - 10;
                r = 4;
                fillCircle(x, y, r, Black);
            }

            if(localUpdate)
            {
                epd_update_area(m_rectangle);
            }
        }
        else 
        {
            if(localUpdate)
            {
                epd_clear_area(m_rectangle);
            }

            String val = getSensorValue(GetId());
            DrawTile(m_rectangle.x, m_rectangle.y, EntityState::UNAVAILABLE, GetType(), GetName(), val);

            if(localUpdate)
            {
               epd_update_area(m_rectangle);
            }
        }
    }
}   


void Actuator::HandleTouchEvent()
{
    if(m_touchType == Entity::TouchType::SINGLE)
    {
        Serial.printf("Handle touch event on entity: %s\n", GetId().c_str());
        
        if(m_actuatorType == ActuatorType::LIGHT)
        {
            if(m_state == EntityState::OFF)
            {
                turnOnOffLight(GetId(), true);
            }
            else if(m_state == EntityState::ON)
            {
                turnOnOffLight(GetId(), false);
            }
        }

        m_touchType = Entity::TouchType::NO_TOUCH;
    }

}
