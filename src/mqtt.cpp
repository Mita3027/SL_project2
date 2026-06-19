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












//---------------------------------------------------------------------------------------------------------------------------------


#include "mqtt.h"

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Topic: ");
    Serial.println(topic);

    String msg;

    for(unsigned int i=0;i<length;i++)
    {
        msg += (char)payload[i];
    }

    Serial.print("Message: ");
    Serial.println(msg);
}

void setupWiFi()
{
    Serial.println("Starting WiFi...");
    WiFi.begin(ssid,password);
}

void setupMQTT()
{
    client.setServer(mqtt_server,1883);
    client.setCallback(callback);
}

void reconnect()
{
    static unsigned long lastAttempt = 0;

    if(client.connected())
        return;

    if(millis() - lastAttempt > 5000)
    {
        lastAttempt = millis();

        Serial.println("Trying MQTT reconnect...");

        if(client.connect("ESP32Client"))
        {
            Serial.println("MQTT Connected");

            client.subscribe("esp32/test");
        }
        else
        {
            Serial.println("MQTT Failed");
        }
    }
}