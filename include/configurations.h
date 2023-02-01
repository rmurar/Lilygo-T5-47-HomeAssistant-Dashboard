#ifndef __CONFIGURATIONS_H__
#define __CONFIGURATIONS_H__

#include "defines.h"

// Start of reserved configurations. Do not change if you dont know what you are doing

// End of reserved configurations


// Start of user configurations. Change as instructed in the comments

// Change to your WiFi credentials
extern const char* ssid;
extern const char* password;

// url to HA server. Only http is supported for now!
extern const String ha_server;
// create a long lived access token and put it here. ref: https://www.home-assistant.io/docs/authentication/
extern const String ha_token;

// GMT Offset in seconds. UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically -18000, AU is typically (+8hrs) 28800
extern int gmtOffset_sec;

/**
 *  Entities are shown in top two rows. Supported types are in entity_type and different icons are used for easy recognition
 *  Only 12 different entities are supported 6 cols x 2 rows. 
 *  Entities follow the format of { <Name that should be displayed>, <entity_id in HA>, <entity_type>, <entity_state_type>}
 *  User a short entity name so it can fit nicely in 160px width in 9px font. 
**/
const ConfigActuator haActuators [] {
    {"VENT. KUPELKA", "switch.ventilator_kupelka", ActuatorType::FAN},
    {"STOL", "light.ziarovka_stol", ActuatorType::LIGHT}
};

/**
 *  Sensors are shown in 3rd row. Supported types are DOOR and MOTION currently. Different icons are used for easy recognition
 *  Only 8 different entities are supported - 8 cols. 
 *  Entities follow the format of { <Name that should be displayed>, <entity_id in HA>, <DOOR|MOTION>, <ONOFF>}
 *  User a short entity name so it can fit nicely in 120px width in 9px font. 
**/
const ConfigSensor haSensors[] {
};

/**
 *  Sensors are shown in last row. Supported types are ENERGYMETER, ENERGYMETERPWR and  and TEMP currently. Different icons are used for easy recognition
 *  Only 3 different entities are supported - 3 cols. ENERGYMETER and ENERGYMETERPWR are grouped together and shown as total
 *  Entities follow the format of { <Name that should be displayed>, <entity_id in HA>, <ENERGYMETER|ENERGYMETERPWR|TEMP>, <VALUE>}
 *  User a short entity name so it can fit nicely in 120px width in 9px font. 
 *  You can have only one ENERGYMETER type and ENERGYMETERPWR type tile in the display. ENERGYMETER and ENERGYMETERPWR are grouped together and shown as total
 *  However you can have multiple temrature tiles (up to 3 if you are not using ENERGYMETER and ENERGYMETERPWR in you HA instances) 
 *  Or you can customize the code the way you see fit (advanced)
**/
const ConfigSensor haFloatSensors[] {
    {"TEPLOTA IZBA", "sensor.sonoff_izba_temperature", SensorType::TEMP, SensorValueType::VALUE},
    {"TEPLOTA VONKU", "sensor.aqara_vonku_temperature", SensorType::TEMP, SensorValueType::VALUE},
    {"SPOTREBA STOL", "sensor.shelly_plug_1_energy_power", SensorType::ENERGYMETERPWR, SensorValueType::VALUE}
};

#endif // __CONFIGURATIONS_H__