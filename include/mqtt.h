#include "declarations.h"




// void setupWiFi();
// void setupMQTT();
// void onConnectionEstablished();


void setupWiFi();
void setupMQTT();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);