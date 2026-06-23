// #include "mqtt.h"


// EspMQTTClient mqttClient(ssid,password, mqtt_server,"ESP32Client");

// void onConnectionEstablished()
// {
//     Serial.println("MQTT Connected");

//     mqttClient.subscribe("esp32/test", [](const String &payload)
//     {
//         Serial.print("Received: ");
//         Serial.println(payload);
//     });

//     mqttClient.publish(
//         "esp32/test",
//         "ESP32 Connected"
//     );
// }

// void setupMQTT()
// {
//     mqttClient.enableDebuggingMessages();
// }

// // void setupWiFi()
// // {
// //     Serial.print("Connecting to WiFi");

// //     WiFi.begin(ssid, password);

// //     while (WiFi.status() != WL_CONNECTED)
// //     {
// //         delay(500);
// //         Serial.print(".");
// //     }

    
// //     Serial.println();
// //     Serial.println("WiFi Connected");
// //     Serial.print("IP Address: ");
// //     Serial.println(WiFi.localIP());
// // }

// void setupWiFi()
// {
//     Serial.println("Starting WiFi...");
//     WiFi.begin(ssid, password);
// }












#include "mqtt.h"

void onMqttConnect(bool sessionPresent)
{
    Serial.println("MQTT Connected");
    mqttClient.subscribe("aqm/mqtt2", 2);
    Serial.println("Hello");
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.print("MQTT Disconnected: ");
    Serial.println((int8_t)reason);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    Serial.print("Topic: ");
    Serial.println(topic);

    String msg;
    for(size_t i = 0; i < len; i++)
    {
        msg += payload[i];
    }

    Serial.print("Message: ");
    Serial.println(msg);
}

void setupWiFi()
{
    Serial.println("Starting WiFi...");
    WiFi.begin(ssid, password);
    delay(500);
    Serial.println(WiFi.localIP());
}

void setupMQTT()
{
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onMessage(onMqttMessage);
}

void reconnect()
{
    if(!mqttClient.connected() && WiFi.status() == WL_CONNECTED)
    {
        mqttClient.connect();
    }
}