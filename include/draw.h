#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdint.h>
#include <Arduino.h>

#include "defines.h"

#define White 0xFF
#define LightGrey 0xBB
#define Grey 0x88
#define DarkGrey 0x44
#define Black 0x00

#define BATT_PIN            36

#define TILE_IMG_WIDTH  100
#define TILE_IMG_HEIGHT 100
#define TILE_WIDTH      160
#define TILE_HEIGHT     160
#define TILE_GAP        6
#define SENSOR_TILE_WIDTH      120
#define SENSOR_TILE_HEIGHT     110
#define SENSOR_TILE_IMG_WIDTH  64
#define SENSOR_TILE_IMG_HEIGHT 64
#define BOTTOM_TILE_WIDTH      320
#define BOTTOM_TILE_HEIGHT     90

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