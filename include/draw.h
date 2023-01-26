#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdint.h>
#include <Arduino.h>

#include "defines.h"

void DrawTile(int x, int y, int width, int height, const uint8_t *image_data, String state, String label);
void DrawSensorTile(int x, int y, int width, int height, const uint8_t* image_data, String label);
void DrawTile(int x, int y, ActuatorState state, ActuatorType type, String name, String value);
void DrawSensorTile(int x, int y, ActuatorState state, SensorType type, String name);
void DrawBottomTile(int x, int y, String value, String name);
void DrawBottomBar();
void DrawSwitchBar();
void DrawSensorBar();
void DrawRSSI(int x, int y, int rssi) ;
void DrawBattery(int x, int y);

void DisplayGeneralInfoSection(String dayStamp, String timeStamp);
void DisplayStatusSection(int rssi);
void DrawWifiErrorScreen(int rssi);
void DrawHAScreen(int rssi, String dayStamp, String timeStamp);

#endif // __DRAW_H__