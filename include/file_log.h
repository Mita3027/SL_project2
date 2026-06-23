#include "declarations.h"
#include "LittleFS.h"

void initLogger();
void saveRecord();
bool pendingRecords();
String getOldestRecord();
void readFile();
void processOfflineQueue();
void deleteOldestRecord();
void printStoredData();