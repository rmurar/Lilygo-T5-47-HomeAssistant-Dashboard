#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdint.h>
#include <Arduino.h>

#include "defines.h"
#include "epd_drawing.h"
#include "epd_driver.h"

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

class Draw
{
public:
    Draw(int screen_width = EPD_WIDTH, int screen_height = EPD_HEIGHT);

    void Init();

    Rect_t GetTileRect(int x, int y, int width, int height);
    Rect_t GetTileRect(int x, int y);
    Rect_t GetSensorTileRect(int x, int y, int width, int height);
    Rect_t GetSensorTileRect(int x, int y);

    Rect_t DrawTile(int x, int y, int width, int height, const uint8_t *image_data, String state, String label);
    Rect_t DrawSensorTile(int x, int y, int width, int height, const uint8_t* image_data, String label);
    Rect_t DrawTile(int x, int y, EntityState state, ActuatorType type, String name, String value);
    void DrawSensorTile(int x, int y, EntityState state, SensorType type, String name);
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

    EpdDrawing& GetEpdDrawing()
    {
        return m_epdDrawing;
    }

private:
    EpdDrawing m_epdDrawing;
};
#endif // __DRAW_H__