#include "file_log.h"
#include "LittleFS.h"
#include <HTTPClient.h>

void initLogger()
{
    if (!LittleFS.begin(true))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    Serial.println("LittleFS Mounted");

    File file = LittleFS.open("/sensor_data.csv", FILE_WRITE);

    if (file)
    {
        Serial.println("File Created");
        file.close();
    }
}

void saveRecord()
{
    File file = LittleFS.open("/sensor_data.csv", FILE_APPEND);

    if (!file)
    {
        Serial.println("File did not open");
        return;
    }

    String record =
        String(g_temp, 1) + "," +
        String(g_humidity, 1) + "," +
        String(g_co2, 0) + "," +
        String(g_voc) + "," +
        String(g_date) + "," +
        String(g_time);

    file.println(record);

    file.close();

    Serial.println("Record Saved");
}

bool pendingRecords()
{
    File file = LittleFS.open("/sensor_data.csv");

    if (!file)
        return false;

    bool result = file.size() > 0;

    file.close();

    return result;
}

String getOldestRecord()
{
    File file = LittleFS.open("/sensor_data.csv", FILE_READ);

    if (!file)
    {
        return "";
    }

    String line = file.readStringUntil('\n');

    line.trim();

    file.close();

    return line;
}

void processOfflineQueue()
{
    Serial.println("=== PROCESS OFFLINE QUEUE CALLED ===");

    if (!pendingRecords())
    {
        Serial.println("No pending records");
        return;
    }

    String rec = getOldestRecord();

    Serial.print("Uploading Offline Record: ");
    Serial.println(rec);

    int p1 = rec.indexOf(',');
    int p2 = rec.indexOf(',', p1 + 1);
    int p3 = rec.indexOf(',', p2 + 1);
    int p4 = rec.indexOf(',', p3 + 1);
    int p5 = rec.indexOf(',', p4 + 1);

    float temp =
        rec.substring(0, p1).toFloat();

    float humidity =
        rec.substring(p1 + 1, p2).toFloat();

    float co2 =
        rec.substring(p2 + 1, p3).toFloat();

    int32_t voc =
        rec.substring(p3 + 1, p4).toInt();

    String dateStr =
        rec.substring(p4 + 1, p5);

    String timeStr =
        rec.substring(p5 + 1);

    dateStr.trim();
    timeStr.trim();

    Serial.print("dateStr = ");
    Serial.println(dateStr);

    Serial.print("timeStr = ");
    Serial.println(timeStr);

    String day =
        dateStr.substring(0, 2);

    String month =
        dateStr.substring(3, 5);

    String year =
        dateStr.substring(6, 10);

    int dayInt = day.toInt();
    int monthInt = month.toInt();
    int yearInt = year.toInt();

    Serial.println("Parsed Date:");

    Serial.print("Day = ");
    Serial.println(dayInt);

    Serial.print("Month = ");
    Serial.println(monthInt);

    Serial.print("Year = ");
    Serial.println(yearInt);

    int hour =
        timeStr.substring(0, 2).toInt();

    int minute =
        timeStr.substring(3, 5).toInt();

    int second =
        timeStr.substring(6, 8).toInt();

    DateTime recordTime(
        yearInt,
        monthInt,
        dayInt,
        hour,
        minute,
        second);

    Serial.print("Unix Time = ");
    Serial.println(recordTime.unixtime());

    DateTime utc =
        recordTime - TimeSpan(0, 5, 30, 0);

    char timestamp[25];

    sprintf(
        timestamp,
        "%04d-%02d-%02dT%02d:%02d:%02dZ",
        utc.year(),
        utc.month(),
        utc.day(),
        utc.hour(),
        utc.minute(),
        utc.second());

    String createdAt = String(timestamp);

    // String createdAt =
    //     String(utc.year()) + "-" +
    //     String(utc.month()) + "-" +
    //     String(utc.day()) + "T" +
    //     String(utc.hour()) + ":" +
    //     String(utc.minute()) + ":" +
    //     String(utc.second()) + "Z";

    Serial.println("Seperated Values:");
    Serial.println(temp);
    Serial.println(humidity);
    Serial.println(co2);
    Serial.println(voc);
    Serial.println(createdAt);

    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" +
                 String(myWriteAPIKey) +
                 "&field1=" + String(temp) +
                 "&field2=" + String(humidity) +
                 "&field3=" + String(co2) +
                 "&field4=" + String(voc) +
                 "&created_at=" + createdAt;

    Serial.println(url);

    http.begin(url);

    int httpCode = http.GET();

    http.end();

    Serial.print("HTTP Code = ");
    Serial.println(httpCode);

    if (httpCode == 200)
    {
        Serial.println("Offline Record Uploaded");

        deleteOldestRecord();
    }
    else
    {
        Serial.println("Upload Failed");
    }

    if (!pendingRecords())
    {
        loggingMode = false;
        Serial.println("Recovery of data was successful");
    }
}

void readFile()
{
    File file = LittleFS.open("/sensor_data.csv", FILE_READ);

    while (file.available())
    {
        Serial.write(file.read());
    }

    file.close();
}

void deleteOldestRecord()
{
    File source = LittleFS.open("/sensor_data.csv", FILE_READ);

    if (!source)
    {
        Serial.println("Cannot open source file");
        return;
    }

    File temp = LittleFS.open("/temp.csv", FILE_WRITE);

    if (!temp)
    {
        Serial.println("Cannot create temp file");
        source.close();
        return;
    }

    bool skipFirstLine = true;

    while (source.available())
    {
        String line = source.readStringUntil('\n');
        line.trim();

        if (skipFirstLine)
        {
            skipFirstLine = false;
            continue;
        }

        temp.println(line);
    }

    source.close();
    temp.close();

    LittleFS.remove("/sensor_data.csv");

    if (LittleFS.rename("/temp.csv", "/sensor_data.csv"))
    {
        Serial.println("Oldest Record Deleted");
    }
    else
    {
        Serial.println("Rename Failed");
    }
}

// void printStoredData()
// {
//     File file = LittleFS.open("/sensor_data.csv", FILE_READ);

//     if (!file)
//     {
//         Serial.println("Cannot open sensor_data.csv");
//         return;
//     }

//     Serial.println("===== FILE CONTENT =====");

//     while (file.available())
//     {
//         Serial.write(file.read());
//     }

//     Serial.println("\n========================");

//     file.close();
// }
