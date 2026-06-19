#include "mqtt.h"
#include "webSocket.h"
#include "thing.h"
#include "sensor.h"
#include "display.h"

unsigned long rtcTimer = 0;
unsigned long sensorTimer = 0;
unsigned long thingSpeakTimer = 0;
unsigned long mqttTimer = 0;
unsigned long wifiRetryTimer = 0;

const unsigned long RTC_DELAY        = 1000;
const unsigned long SENSOR_DELAY     = 10000;
const unsigned long THINGSPEAK_DELAY = 15000;
const unsigned long MQTT_DELAY       = 5000;

void setup()
{
    Serial.begin(115200);

    Wire.begin(48, 47);

    WiFi.mode(WIFI_STA);

    setupWiFi();
    setupMQTT();

    rtc.begin();
    scd30.begin();
    sgp40.begin(Wire);

    Display_Init();

    ThingSpeak.begin(espClient);

    initWebSocket();

    server.on("/", HTTP_GET,
    [](AsyncWebServerRequest *request)
    {
        request->send(200, "text/html", index_html);
    });

    server.begin();

    Serial.println("Setup Complete");
}

void loop()
{
    // LVGL
    Display_Timer();

    // WebSocket cleanup
    ws.cleanupClients();
      
  
    // WiFi reconnect every 5 sec
    if (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - wifiRetryTimer >= 5000)
        {
            wifiRetryTimer = millis();
            
            Serial.println("Disconnected");

            Serial.println("Trying WiFi reconnect...");

            WiFi.disconnect();
            WiFi.begin(ssid, password);
        }
      
    }
    else
    {
        // MQTT reconnect + processing
        reconnect();
        client.loop();
    }

    // RTC every 1 second
    if (millis() - rtcTimer >= RTC_DELAY)
    {
        rtcTimer = millis();

        readRTC();
        updateUI();

        if (WiFi.status() == WL_CONNECTED)
        {
            getData();
            notifyClients();
        }
    }

    // Sensors every 10 seconds
    if (millis() - sensorTimer >= SENSOR_DELAY)
    {
        sensorTimer = millis();

        readSCD30();
        readSGP40();

        Serial.println("Sensors Updated");
    }

    // MQTT publish every 5 seconds
    if (millis() - mqttTimer >= MQTT_DELAY)
    {
        mqttTimer = millis();

        if (client.connected())
        {
            client.publish(
                "esp32/test1",
                "RECEIVED !!"
            );

            Serial.println("MQTT Published");
        }
    }

    // ThingSpeak every 15 seconds
    if (millis() - thingSpeakTimer >= THINGSPEAK_DELAY)
    {
        thingSpeakTimer = millis();

        if (WiFi.status() == WL_CONNECTED)
        {
            thingSpeakUpdate();

            Serial.println("ThingSpeak Updated");
        }
    }
}