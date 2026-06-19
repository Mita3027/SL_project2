#include "sensor.h" 


void readRTC()
{
    DateTime now = rtc.now();

    sprintf(g_time,
            "%02d:%02d:%02d",
            now.hour(),
            now.minute(),
            now.second());

    sprintf(g_date,
            "%02d/%02d/%04d",
            now.day(),
            now.month(),
            now.year());

    Serial.print("\nCurrent time:  ");
    Serial.print(g_time);
    Serial.print("\nCurrent date: ");
    Serial.println(g_date);        
}

void readSCD30()
{
    if (scd30.dataReady())
    {
        if (scd30.read())
        {
            g_co2 = scd30.CO2;
            g_temp = scd30.temperature;
            g_humidity = scd30.relative_humidity;
            Serial.print("\nCO2: ");
            Serial.print(g_co2);
            Serial.print("ppm \n");
            Serial.print("Temperature: ");
            Serial.print(g_temp);
            Serial.print(" C\n");
            Serial.print("Humidity: ");
            Serial.print(g_humidity);
            Serial.println(" %\n");
        }
    }
}

void readSGP40()
{
    uint16_t srawVoc;
    uint16_t error;

    uint16_t rhTicks =
        (uint16_t)((g_humidity * 65535.0f) / 100.0f);

    uint16_t tempTicks =
        (uint16_t)(((g_temp + 45.0f) * 65535.0f) / 175.0f);

    error = sgp40.measureRawSignal(
                rhTicks,
                tempTicks,
                srawVoc);
    
    if (!error)
    {
        g_voc = vocAlgorithm.process(srawVoc);
        Serial.print("VOC Index: ");
        Serial.println(g_voc);
    }
}


void updateUI()
{
    char buf[32];

    sprintf(buf, "%.1f C", g_temp);
    lv_label_set_text(ui_tempValueLabel, buf);

    sprintf(buf, "%.1f %%", g_humidity);
    lv_label_set_text(ui_humidityValueLabel, buf);

    sprintf(buf, "%.0f ppm", g_co2);
    lv_label_set_text(ui_co2ValueLabel, buf);

    lv_label_set_text(ui_timeLabel, g_time);
    lv_label_set_text(ui_timeLabel2, g_time);

    lv_label_set_text(ui_dateLabel, g_date);
    lv_label_set_text(ui_dateLabel2, g_date);

    sprintf(buf, "%ld", g_voc);
    lv_label_set_text(ui_vocValueLabel, buf);
}