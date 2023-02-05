#include <Arduino.h>

// battery
#include <driver/adc.h>
#include "esp_adc_cal.h"

// deepsleep
#include "esp_sleep.h"

// epd
#include "epd_driver.h"

// font
#include "opensans8b.h"
#include "opensans9b.h"
#include "opensans10b.h"
#include "opensans12b.h"
#include "opensans18b.h"
#include "opensans24b.h"

#include "configurations.h"
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

#include "defines.h"

#include "configurations.h"

#include "draw.h"

Rect_t GetTileRect(int x, int y, int width, int height)
{
    Rect_t area = {.x = x, .y = y, .width = width, .height = height};
    return area;
}

Rect_t GetTileRect(int x, int y)
{
    int tile_width = TILE_WIDTH - TILE_GAP; 
    int tile_height = TILE_HEIGHT - TILE_GAP;

    Rect_t area = {.x = x, .y = y, .width = tile_width, .height = tile_height};
    
    return area;
}

Rect_t GetSensorTileRect(int x, int y, int width, int height)
{
    Rect_t area = {.x = x, .y = y, .width = width, .height = height};
}

Rect_t GetSensorTileRect(int x, int y)
{
    int tile_width = SENSOR_TILE_WIDTH - TILE_GAP; 
    int tile_height = SENSOR_TILE_HEIGHT - TILE_GAP;

    Rect_t area = {.x = x, .y = y, .width = tile_width, .height = tile_height};
    
    return area;
}



// this will place a tile on screen that includes icon, staus and name of the HA entity
Rect_t DrawTile(int x, int y, int width, int height, const uint8_t *image_data, String state, String label)
{
  // this assumes images are 100x100px size. make sure images are cropped to 100x100 before converting
  int image_x = int((width - TILE_IMG_WIDTH)/2) + x; 
  int image_y = int((height - TILE_IMG_HEIGHT)/2) + y;
  int top_txt_cursor_x = width / 2 + x;
  int top_txt_cursor_y = image_y + TILE_IMG_HEIGHT + 10 + 12;
  int bottom_txt_cursor_x = int(width / 2) + x;
  int bottom_txt_cursor_y = y + 21;

  Rect_t area = {.x = x, .y = y, .width = width, .height = height};

  drawRect(x, y, width, height, Black);
  drawRect(x + 1, y + 1, width - 2, height - 2, Black);
  drawImage(image_x, image_y, TILE_IMG_WIDTH, TILE_IMG_HEIGHT, image_data);
  drawString(top_txt_cursor_x, top_txt_cursor_y, label, CENTER);
  drawString(bottom_txt_cursor_x, bottom_txt_cursor_y, state, CENTER);

  return area;
}

Rect_t DrawSensorTile(int x, int y, int width, int height, const uint8_t* image_data, String label)
{
  // this assumes images are 128x128px size. make sure images are cropped to 128x128 before converting
  int image_x = int((width - SENSOR_TILE_IMG_WIDTH)/2) + x; 
  int image_y = y + 10;  
  int txt_cursor_x = int(width/2) + x;
  int txt_cursor_y = image_y + SENSOR_TILE_IMG_HEIGHT + 10 + 12;

  Rect_t area = {.x = x, .y = y, .width = width, .height = height};

  drawRect(x, y, width, height, Black);
  drawRect(x+1, y+1, width-2, height-2, Black);
  drawImage(image_x, image_y, SENSOR_TILE_IMG_WIDTH, SENSOR_TILE_IMG_HEIGHT, image_data);
  drawString(txt_cursor_x, txt_cursor_y, label, CENTER);

  return area;
}

Rect_t DrawTile(int x, int y, ActuatorState state, ActuatorType type, String name, String value)
{
    int tile_width = TILE_WIDTH - TILE_GAP; 
    int tile_height = TILE_HEIGHT - TILE_GAP;

    Rect_t area = {.x = x, .y = y, .width = tile_width, .height = tile_height};

    String state_txt = "OFF";
    if (state == ActuatorState::ON) state_txt = "ON"; 
    else if (state == ActuatorState::UNAVAILABLE) state_txt = "UNAVAILABLE"; 
    switch (type)
    {
    case ActuatorType::SWITCH:
        if (state == ActuatorState::ON) DrawTile(x,y,tile_width,tile_height,switchon_data, name, state_txt); 
        else if (state == ActuatorState::OFF) DrawTile(x,y,tile_width,tile_height,switchoff_data, name, state_txt); 
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "SWITCH"); 
        break;
    case ActuatorType::LIGHT:
        if (state == ActuatorState::ON)  DrawTile(x,y,tile_width,tile_height,lightbulbon_data,name, state_txt);
        else if (state == ActuatorState::OFF)  DrawTile(x,y,tile_width,tile_height,lightbulboff_data,name, state_txt);
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "LIGHT"); 
        break;
    case ActuatorType::FAN:
        if (state == ActuatorState::ON)  DrawTile(x,y,tile_width,tile_height,fanon_data,name, state_txt);
        else if (state == ActuatorState::OFF)  DrawTile(x,y,tile_width,tile_height,fanoff_data,name, state_txt);
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "FAN"); 
        break;
    case ActuatorType::EXFAN:
        if (state == ActuatorState::ON)  DrawTile(x,y,tile_width,tile_height,exhaustfanon_data,name, state_txt);
        else if (state == ActuatorState::OFF)  DrawTile(x,y,tile_width,tile_height,exhaustfanoff_data,name, state_txt);
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "EXHAUST FAN"); 
        break;
    case ActuatorType::AIRPURIFIER:
        if (state == ActuatorState::ON)  DrawTile(x,y,tile_width,tile_height,airpurifieron_data,name, state_txt);
        else if (state == ActuatorState::OFF)  DrawTile(x,y,tile_width,tile_height,airpurifieroff_data,name, state_txt);
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "AIR PURIFIER"); 
        break;
    case ActuatorType::WATERHEATER:
        if (state == ActuatorState::ON)  DrawTile(x,y,tile_width,tile_height,waterheateron_data,name, state_txt);
        else if (state == ActuatorState::OFF)  DrawTile(x,y,tile_width,tile_height,waterheateroff_data,name, state_txt);
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "WATER HEATER"); 
        break;
    case ActuatorType::PLUG:
        if (state == ActuatorState::ON)  DrawTile(x,y,tile_width,tile_height,plugon_data,name, state_txt);
        else if (state == ActuatorState::OFF)  DrawTile(x,y,tile_width,tile_height,plugoff_data,name, state_txt);
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "PLUG"); 
        break;
    case ActuatorType::AIRCONDITIONER:
        if (state == ActuatorState::ON)  DrawTile(x,y,tile_width,tile_height,airconditioneron_data,name, state_txt);
        else if (state == ActuatorState::OFF)  DrawTile(x,y,tile_width,tile_height,airconditioneroff_data,name, state_txt);
        else DrawTile(x,y,tile_width,tile_height,warning_data, name, "AIR CONDITIONER"); 
    break;
    case ActuatorType::PLANT:
        if (value.toInt() >= 80)
            DrawTile(x, y, tile_width, tile_height, plantwateringok_data, name, value + "%");
        else if (value.toInt() > 0)
            DrawTile(x, y, tile_width, tile_height, plantwateringlow_data, name, value + "%");
        else
            DrawTile(x, y, tile_width, tile_height, warning_data, name, "PLANT");
        break;
    default:
        break;
    }

    return area;
}

void DrawSensorTile(int x, int y, ActuatorState state, SensorType type, String name)
{
    int tile_width = SENSOR_TILE_WIDTH - TILE_GAP; 
    int tile_height = SENSOR_TILE_HEIGHT - TILE_GAP;
    switch (type)
    {
    case SensorType::DOOR:
        if (state == ActuatorState::ON) DrawSensorTile(x,y,tile_width,tile_height,dooropen_data, name); 
        else if (state == ActuatorState::OFF) DrawSensorTile(x,y,tile_width,tile_height,doorclosed_data, name); 
        else DrawSensorTile(x,y,tile_width,tile_height,sensorerror_data, name); 
        break;
    case SensorType::MOTION:
        if (state == ActuatorState::ON) DrawSensorTile(x,y,tile_width,tile_height,motionsensoron_data, name); 
        else if (state == ActuatorState::OFF) DrawSensorTile(x,y,tile_width,tile_height,motionsensoroff_data, name); 
        else DrawSensorTile(x,y,tile_width,tile_height,sensorerror_data, name); 
        break;
    default:
        break;
    }
}

void DrawBottomTile(int x, int y, String value, String name)
{
    int tile_width = BOTTOM_TILE_WIDTH - TILE_GAP; 
    int tile_height = BOTTOM_TILE_HEIGHT - TILE_GAP;
    drawRect(x, y, tile_width, tile_height, Black);
    drawRect(x+1, y+1, tile_width-2, tile_height-2, Black);
    setFont(OpenSans24B);
    drawString(int(tile_width/2) + x, 508, value, CENTER);
    setFont(OpenSans9B);
    drawString(int(tile_width/2) + x, 532, name, CENTER);
}

void DrawBottomBar()
{
    int tiles = 3;
    float totalEnergy = 0;
    float totalPower  = 0;
    String totalEnergyName;
    String totaPowerName;
    for (int i = 0; i < (sizeof(haFloatSensors) / sizeof(haFloatSensors[0])); i++){
        if (haFloatSensors[i].type == SensorType::ENERGYMETER)
        {
            totalEnergy = totalEnergy + getSensorFloatValue(haFloatSensors[i].id);
            totalEnergyName = haFloatSensors[i].name;
        }
        else if (haFloatSensors[i].type == SensorType::ENERGYMETERPWR)
        {
            totalPower = totalPower + getSensorFloatValue(haFloatSensors[i].id);
            totaPowerName = haFloatSensors[i].name;
        }
    }
    int x = 3;
    int y = 456;
    // first one 
    if (totalEnergy != 0)
    {
        DrawBottomTile(x, y, String(totalEnergy) + " kWh", totalEnergyName);
        x = x + BOTTOM_TILE_WIDTH;
        tiles--;
    }
    if (totalPower != 0)
    {
        DrawBottomTile(x, y, String((int)totalPower) + " W", totaPowerName);
        x = x + BOTTOM_TILE_WIDTH;
        tiles--;
    }
    
    for (int i = 0; i < (sizeof(haFloatSensors) / sizeof(haFloatSensors[0])); i++){
        if (haFloatSensors[i].type == SensorType::TEMP && tiles >= 1)
        {
            float temp = getSensorAttributeValue(haFloatSensors[i].id, "current_temperature").toFloat();
            if (temp == 0)
                temp = getSensorValue(haFloatSensors[i].id).toFloat();
            DrawBottomTile(x, y, String(temp, 1) + "° C", haFloatSensors[i].name);
            x = x + BOTTOM_TILE_WIDTH;
            tiles--;
        }
    }
}

void DrawSwitchBar()
{
    setFont(OpenSans9B);
    int x = 3;
    int y = 23;
    for (int i = 0; i < sizeof(haActuators) / sizeof(haActuators[0]); i++){
        if (haActuators[i].name != "") {
          if (haActuators[i].type == ActuatorType::SWITCH ||
              haActuators[i].type == ActuatorType::LIGHT ||
              haActuators[i].type == ActuatorType::EXFAN ||
              haActuators[i].type == ActuatorType::FAN ||
              haActuators[i].type == ActuatorType::AIRPURIFIER ||
              haActuators[i].type == ActuatorType::WATERHEATER ||
              haActuators[i].type == ActuatorType::AIRCONDITIONER)
          {            
              DrawTile(x, y, checkOnOffState(haActuators[i].id), haActuators[i].type, haActuators[i].name, "");
          }
          else 
          {
              String val = getSensorValue(haActuators[i].id);
              DrawTile(x, y, ActuatorState::UNAVAILABLE, haActuators[i].type, haActuators[i].name, val);
          }
        }
      
        x = x + TILE_WIDTH; // move column right
        if (i == 5) { // move row down
            x = 3;
            y = y + TILE_HEIGHT;
        }
    }
}

void DrawSensorBar()
{
    setFont(OpenSans9B);
    int x = 3;
    int y = 345;
    for (int i = 0; i < sizeof(haSensors) / sizeof(haSensors[0]); i++){
        if (haSensors[i].type == SensorType::DOOR ||
            haSensors[i].type == SensorType::MOTION )
        {
      if (haSensors[i].name != "")
            DrawSensorTile(x,y,checkOnOffState(haSensors[i].id),haSensors[i].type, haSensors[i].name);
        }
        x = x + SENSOR_TILE_WIDTH;
    }
}

void DrawRSSI(int x, int y, int rssi) 
{
  int WIFIsignal = 0;
  int xpos = 1;
  for (int _rssi = -100; _rssi <= rssi; _rssi = _rssi + 20) {
    if (_rssi <= -20)  WIFIsignal = 20; //            <-20dbm displays 5-bars
    if (_rssi <= -40)  WIFIsignal = 16; //  -40dbm to  -21dbm displays 4-bars
    if (_rssi <= -60)  WIFIsignal = 12; //  -60dbm to  -41dbm displays 3-bars
    if (_rssi <= -80)  WIFIsignal = 8; //  -80dbm to  -61dbm displays 2-bars
    if (_rssi <= -100) WIFIsignal = 4;  // -100dbm to  -81dbm displays 1-bar
    
    if (rssi != 0) 
      fillRect(x + xpos * 8, y - WIFIsignal, 6, WIFIsignal, Black);
    else // draw empty bars
      drawRect(x + xpos * 8, y - WIFIsignal, 6, WIFIsignal, Black);
    xpos++;
  }
  if (rssi == 0) 
    drawString(x , y, "x", LEFT);
}

// void DrawBattery(int x, int y) {
//   uint8_t percentage = 100;
//   float voltage = analogRead(35) / 4096.0 * 7.46;
//   if (voltage > 1 ) { // Only display if there is a valid reading
//     Serial.println("Voltage = " + String(voltage));
//     percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
//     if (voltage >= 4.20) percentage = 100;
//     if (voltage <= 3.20) percentage = 0;  // orig 3.5
//     drawRect(x + 55, y - 15 , 40, 15, Black);
//     fillRect(x + 95, y - 9, 4, 6, Black);
//     fillRect(x + 57, y - 13, 36 * percentage / 100.0, 11, Black);
//     drawString(x, y, String(percentage) + "%", LEFT);
//     //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
//   }
// }

void DrawBattery(int x, int y)
{
  int vref = 1100; // default battery vref

  uint8_t percentage = 100;
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
    vref = adc_chars.vref;
  }
  float voltage = analogRead(36) / 4096.0 * 6.566 * (vref / 1000.0);
  if (voltage > 1 ) { // Only display if there is a valid reading
    Serial.println("\nVoltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20) percentage = 100;
    if (voltage <= 3.20) percentage = 0;  // orig 3.5
     drawRect(x + 55, y - 15 , 40, 15, Black);
     fillRect(x + 95, y - 9, 4, 6, Black);
     fillRect(x + 57, y - 13, 36 * percentage / 100.0, 11, Black);
     drawString(x, y, String(percentage) + "%", LEFT);
     drawString(x + 130, y,  String(voltage, 2) + "v", CENTER);
  }
}

void DisplayGeneralInfoSection(String dayStamp, String timeStamp)
{
    setFont(OpenSans8B);
    Serial.println("Getting haStatus...");
    HAConfigurations haConfigs = getHaStatus();
    Serial.println("drawing status line...");    
    drawString(EPD_WIDTH/2, 18, dayStamp + " - " +  timeStamp + " (HA Ver:" + haConfigs.version + "/" + haConfigs.haStatus + ", TZ:" + haConfigs.timeZone + ")", CENTER);
}

void DisplayStatusSection(int rssi)
{
  setFont(OpenSans8B);
  DrawBattery(5, 18);
  DrawRSSI(900, 18, rssi);
}

void DrawWifiErrorScreen(int rssi)
{
    epd_clear();
    DisplayStatusSection(rssi);
    epd_update();
}

void DrawHAScreen(int rssi, String dayStamp, String timeStamp)
{
    epd_clear();
    
    DisplayStatusSection(rssi);
    DisplayGeneralInfoSection(dayStamp, timeStamp);
    Serial.println("Drawing switchBar...");
    DrawSwitchBar();
    Serial.println("Drawing sensorBar...");
    DrawSensorBar();
    Serial.println("Drawing bottomBar...");
    DrawBottomBar();

    epd_update();
}