#include "thing.h"


void thingSpeakUpdate() 
{
    ThingSpeak.setField(1, g_temp);
    ThingSpeak.setField(2, g_humidity);
    ThingSpeak.setField(3, g_co2);
    ThingSpeak.setField(4, g_voc);


    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200)
    {
      Serial.println("Channel update successful.");
    }
    else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

}