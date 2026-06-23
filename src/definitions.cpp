#include "declarations.h"

RTC_DS3231 rtc;
SensirionI2CSgp40 sgp40;
VOCGasIndexAlgorithm vocAlgorithm;
Adafruit_SCD30 scd30;

float g_temp = 0;
float g_humidity = 0;
float g_co2 = 0;
int32_t g_voc = 0;

char g_time[20];
char g_date[20];

String dateValue = "";
String timeValue = "";

bool loggingMode = false;

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

unsigned long myChannelNumber = 3;
const char * myWriteAPIKey = "FTUYIHJFFFQ46Q4C"; 

WiFiClient espClient;
AsyncMqttClient mqttClient;

const char* ssid = "Hotspot";
const char* password = "98761234";
const char* mqtt_server = "10.136.134.35";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");