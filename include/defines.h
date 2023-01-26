#ifndef __DEFINES_H__
#define __DEFINES_H__

enum class ActuatorState {ON, OFF, ERROR, UNAVAILABLE};
enum class ActuatorType {SWITCH, LIGHT, EXFAN, FAN, AIRPURIFIER, WATERHEATER, PLUG, AIRCONDITIONER, PLANT};

enum class SensorValueType {ONOFF, VALUE};
enum class SensorType {DOOR, MOTION, ENERGYMETER, TEMP, ENERGYMETERPWR};

//enum entity_state {ON, OFF, ERROR, UNAVAILABLE};
//enum entity_type {SWITCH, LIGHT, EXFAN, FAN, AIRPURIFIER, WATERHEATER, PLUG, AIRCONDITIONER, PLANT};
//enum entity_state_type {ONOFF, VALUE};
//enum sensor_type {DOOR, MOTION, ENERGYMETER, TEMP, ENERGYMETERPWR};

struct ConfigSensor
{
    String name;
    String id;
    SensorType type;
    SensorValueType valueType;    
};

struct ConfigActuator
{
    String name; 
    String id; 
    ActuatorType type;
};

struct HAConfigurations{
    String timeZone;
    String version;
    String haStatus;
};

#endif // __DEFINES_H__