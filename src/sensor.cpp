#include "sensor.h"

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


void Sensor::Draw(Entity::DrawType type)
{
    if(m_valueType == SensorValueType::ONOFF)
    {
        m_draw.GetEpdDrawing().setFont(OpenSans9B);
        if (m_sensorType == SensorType::DOOR ||
            m_sensorType == SensorType::MOTION )
        {
            if (GetName() != "") {
                EntityState state;
                if(GetState(state) == true)
                { 
                    m_draw.DrawSensorTile(m_rectangle.x, m_rectangle.y, state, m_sensorType, GetName());
                }
            }
        }
    }
    else if(m_valueType == SensorValueType::VALUE)
    {

    }

}