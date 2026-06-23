#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Adafruit_SCD30.h"
#include <SensirionI2CSgp40.h>
#include <VOCGasIndexAlgorithm.h>
#include <RTClib.h>
#include <AsyncMqttClient.h>
#include "ThingSpeak.h"
#include <Adafruit_Sensor.h>
#include <LittleFS.h>
#include <HTTPClient.h>


extern RTC_DS3231 rtc;
extern SensirionI2CSgp40 sgp40;
extern VOCGasIndexAlgorithm vocAlgorithm;
extern Adafruit_SCD30 scd30;
extern AsyncMqttClient mqttClient;
extern HTTPClient http;

extern AsyncWebServer server;
extern AsyncWebSocket ws;

extern float g_temp;
extern float g_humidity;
extern float g_co2;
extern int32_t g_voc;

extern char g_time[20];
extern char g_date[20];

extern String dateValue;
extern String timeValue;

extern bool loggingMode ;

extern unsigned long lastTime;
extern unsigned long timerDelay;

extern unsigned long myChannelNumber;
extern const char * myWriteAPIKey;

extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;

extern WiFiClient espClient;
extern AsyncMqttClient mqttClient;
