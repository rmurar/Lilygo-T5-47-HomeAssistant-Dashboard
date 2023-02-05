#include <Arduino.h>
#include <stdlib.h>

// esp32 sdk imports
#include "esp_heap_caps.h"
#include "esp_log.h"

// epd
#include "epd_driver.h"

// battery
#include <driver/adc.h>
#include "esp_adc_cal.h"

// deepsleep
#include "esp_sleep.h"

// font
#include "opensans8b.h"
#include "opensans9b.h"
#include "opensans10b.h"
#include "opensans12b.h"
#include "opensans18b.h"
#include "opensans24b.h"

#include "epd_drawing.h"

// wifi
#include <WiFi.h>

#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "configurations.h"
#include "homeassistantapi.h"
#include "draw.h"

#include "dashboard.h"


// deep sleep configurations
long SleepDuration   = 1; // Sleep time in minutes, aligned to the nearest minute boundary, so if 30 will always update at 00 or 30 past the hour
int  WakeupHour      = 6;  // Wakeup after 06:00 to save battery power
int  SleepHour       = 23; // Sleep  after 23:00 to save battery power

long StartTime       = 0;
long SleepTimer      = 0;
int  CurrentHour = 0, CurrentMin = 0, CurrentSec = 0;

int vref = 1100; // default battery vref
int wifi_signal = 0;

// required for NTP time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String formattedDate;
String dayStamp = "";
String timeStamp = "";

Dashboard dashboard;

gpio_num_t TOUCH_PANEL_INT = GPIO_NUM_13;

/*
struct EntityState
{
    String entityID;
    entity_state state;
};

struct SensorState
{
    String entityID;
    float value;
};

RTC_DATA_ATTR EntityState entityStateArray[sizeof(haEntities) + sizeof(haSensors) ];
RTC_DATA_ATTR SensorState sensorStateArray[sizeof(haFloatSensors)];
*/

uint8_t StartWiFi() {
  Serial.println("\r\nConnecting to: " + String(ssid));
  IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;
  while (AttemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000) { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED) {
    wifi_signal = WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
    timeClient.begin();
    timeClient.setTimeOffset(gmtOffset_sec);
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  }
  else {
    wifi_signal = 0;
    Serial.println("WiFi connection *** FAILED ***"); 
  } 
  return connectionStatus;
}

void StopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void SetupTime()
{
        Serial.println("Getting time...");

        while(!timeClient.update()) {
            timeClient.forceUpdate();
        }
        formattedDate = timeClient.getFormattedDate();
        int splitT = formattedDate.indexOf("T");
        dayStamp = formattedDate.substring(0, splitT);
        timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);

        CurrentHour = timeClient.getHours();
        CurrentMin  = timeClient.getMinutes();
        CurrentSec  = timeClient.getSeconds();

}

void InitialiseSystem() {
  StartTime = millis();
  Serial.begin(115200);
  while (!Serial);
  Serial.println(String(__FILE__) + "\nStarting...");
  epd_init();
  framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) Serial.println("Memory alloc failed!");
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

  setFont(OpenSans9B);
}

void BeginSleep() {
  epd_poweroff_all();
  SleepTimer = (SleepDuration * 60 - ((CurrentMin % SleepDuration) * 60 + CurrentSec));
  esp_sleep_enable_timer_wakeup(SleepTimer * 1000000LL); // in Secs, 1000000LL converts to Secs as unit = 1uSec
  Serial.println("Awake for : " + String((millis() - StartTime) / 1000.0, 3) + "-secs");
  Serial.println("Entering " + String(SleepTimer) + " (secs) of sleep time");
  Serial.println("Starting deep-sleep period...");
  esp_deep_sleep_start();  // Sleep for e.g. 30 minutes
}

/**
 * Powers off everything into deepsleep so device and display.
 */
void start_deep_sleep_with_wakeup_sources()
{
    long sleepTime = 60;

    epd_poweroff_all();
    delay(400);
    esp_sleep_enable_ext0_wakeup(Dashboard::TOUCH_PANEL_INT, 1);

    esp_sleep_enable_timer_wakeup(sleepTime * 1000000LL);

    Serial.println("Sending device to deepsleep for " + String(sleepTime) + " secs");
    esp_deep_sleep_start();
}


/**
 * Function that prints the reason by which ESP32 has been awaken from sleep
 */
void print_wakeup_reason(){
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason){
        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}

void setup() {
    InitialiseSystem();

    print_wakeup_reason();

    dashboard.Init();

//    while(digitalRead(Dashboard::TOUCH_PANEL_INT))
//    {
//        Serial.printf("Touch panel int\n");
        dashboard.ScanTouchPoint();
//    }

    if (StartWiFi() == WL_CONNECTED) {
/*
        SetupTime();
        bool WakeUp = false;

        if (WakeupHour > SleepHour)
        {
            WakeUp = ((CurrentHour >= WakeupHour) || (CurrentHour <= SleepHour));
        }
        else
        {
            WakeUp = ((CurrentHour >= WakeupHour) && (CurrentHour <= SleepHour));
        }

        if (WakeUp) {
            dashboard.DrawDashboard(wifi_signal, dayStamp, timeStamp);
            //DrawHAScreen(wifi_signal, dayStamp, timeStamp);
        }
*/        
        dashboard.DrawDashboard(wifi_signal, dayStamp, timeStamp);

    }
    else {
        dashboard.DrawWifiErrorScreen(wifi_signal);
    }
    
    //sleep(5);

    //BeginSleep();
    start_deep_sleep_with_wakeup_sources();
}

void loop() {    

  //test
#if 1
    if(!WiFi.isConnected())
    {
        //try to connect
        if (StartWiFi() == WL_CONNECTED) {
            SetupTime();
        }
        else 
        {
            dashboard.DrawWifiErrorScreen(wifi_signal);
        }
    }

    if(WiFi.isConnected())
    {
        dashboard.DrawDashboard(wifi_signal, dayStamp, timeStamp);
    }

    sleep(20);

#endif

}

