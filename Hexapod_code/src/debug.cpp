#include <Arduino.h>
#include "header.h"

// ================================================================
// ===                          debug                           ===
// ================================================================
#ifdef DEBUG_SERIAL

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

#ifdef DEBUG_LED
void Debug_Led(const byte &error_code)
{
    for (int bitPosition = 0; bitPosition < 8; ++bitPosition)
    {
        // Use bitwise AND to check the value of the bit at the given position
        if ((error_code & (1 << bitPosition)))
        {
            // Bit is 1
            Led_update(bitPosition + 1, CRGB::Blue);
        }
        else
        {
            // Bit is 0
            Led_update(bitPosition + 1, CRGB::Black);
        }
    }

    FastLED.show();
}
#endif