#include <Arduino.h>
#include "header.h"

// ================================================================
// ===                          debug                           ===
// ================================================================
#ifdef DEBUG

void Debug_Serial_out()
{
    Serial.print("loopTime: ");
    Serial.print(loopTime);

    if (loopTime < 10)
    {
        Serial.print("   ");
    }
    else if (loopTime < 100)
    {
        Serial.print("  ");
    }

    Serial.print(" || ");

    Serial.println();
}

void Debug_WaitForSerial()
{
    // wait for USB connection
    while (!Serial)
    {
        delay(100);
    }

    Serial.println("press any button to start");

    // wait for serial input
    while (!Serial.available())
    {
        delay(100);
    }
    // clear serial data

    Serial.println();
    Serial.println("================================================================================");
    Serial.println();
}

void Debug_delay()
{
    for (int i = 0; i < 10; i++)
    {
        Serial.print(".");
        delay(500);
    }
}
#endif