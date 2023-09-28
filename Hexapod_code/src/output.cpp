#include <Arduino.h>
#include "header.h"

// ================================================================
// ===                          Servo                           ===
// ================================================================
#ifdef SERVO

Adafruit_PWMServoDriver servoDriver_0 = Adafruit_PWMServoDriver();
Adafruit_PWMServoDriver servoDriver_1 = Adafruit_PWMServoDriver(0x41);

Leg_Struct Leg[6];

void Servo_init()
{
    servoDriver_0.begin();
    servoDriver_0.setOscillatorFrequency(27000000);
    servoDriver_0.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

    servoDriver_1.begin();
    servoDriver_1.setOscillatorFrequency(27000000);
    servoDriver_1.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

#pragma region configureServos
#pragma region Leg_0
    Leg[0].Servo[0].ch = 2;
    Leg[0].Servo[1].ch = 1;
    Leg[0].Servo[2].ch = 0;

    Leg[0].Servo[2].minAngle = 20;
    Leg[0].Servo[2].maxAngle = 160;

    Leg[0].Servo[0].angleOffset = 0;
    Leg[0].Servo[1].angleOffset = -10;
    Leg[0].Servo[2].angleOffset = -5;

    Leg[0].mirrored = true;
    Leg[0].mountAngle = -30;

    Leg[0].minLED = 85;
    Leg[0].maxLED = 115;
#pragma endregion

#pragma region Leg_1
    Leg[1].Servo[0].ch = 6;
    Leg[1].Servo[1].ch = 5;
    Leg[1].Servo[2].ch = 4;

    Leg[1].Servo[2].minAngle = 20;
    Leg[1].Servo[2].maxAngle = 160;

    Leg[1].Servo[0].angleOffset = -10;
    Leg[1].Servo[1].angleOffset = 0;
    Leg[1].Servo[2].angleOffset = 12;

    Leg[1].mirrored = true;
    Leg[1].mountAngle = -90;

    Leg[1].minLED = 116;
    Leg[1].maxLED = 146;
#pragma endregion

#pragma region Leg_2
    Leg[2].Servo[0].ch = 13;
    Leg[2].Servo[1].ch = 14;
    Leg[2].Servo[2].ch = 15;

    Leg[2].Servo[2].minAngle = 20;
    Leg[2].Servo[2].maxAngle = 160;

    Leg[2].Servo[0].angleOffset = -12;
    Leg[2].Servo[1].angleOffset = -10;
    Leg[2].Servo[2].angleOffset = -16;

    Leg[2].mirrored = true;
    Leg[2].mountAngle = -150;

    Leg[2].minLED = 147;
    Leg[2].maxLED = 177;
#pragma endregion

#pragma region Leg_3
    Leg[3].Servo[0].ch = 18;
    Leg[3].Servo[1].ch = 17;
    Leg[3].Servo[2].ch = 16;

    Leg[3].Servo[2].minAngle = 20;
    Leg[3].Servo[2].maxAngle = 160;

    Leg[3].Servo[0].angleOffset = -1;
    Leg[3].Servo[1].angleOffset = 6;
    Leg[3].Servo[2].angleOffset = 44;

    Leg[3].mirrored = false;
    Leg[3].mountAngle = 150;

    Leg[3].minLED = 178;
    Leg[3].maxLED = 208;
#pragma endregion

#pragma region Leg_4
    Leg[4].Servo[0].ch = 25;
    Leg[4].Servo[1].ch = 26;
    Leg[4].Servo[2].ch = 27;

    Leg[4].Servo[2].minAngle = 20;
    Leg[4].Servo[2].maxAngle = 160;

    Leg[4].mirrored = false;
    Leg[4].mountAngle = 90;

    Leg[4].Servo[0].angleOffset = -5;
    Leg[4].Servo[1].angleOffset = -10;
    Leg[4].Servo[2].angleOffset = -8;

    Leg[4].minLED = 209;
    Leg[4].maxLED = 239;
#pragma endregion

#pragma region Leg_5
    Leg[5].Servo[0].ch = 29;
    Leg[5].Servo[1].ch = 30;
    Leg[5].Servo[2].ch = 31;

    Leg[5].Servo[2].minAngle = 20;
    Leg[5].Servo[2].maxAngle = 160;

    Leg[5].Servo[0].angleOffset = -2;
    Leg[5].Servo[1].angleOffset = 3;
    Leg[5].Servo[2].angleOffset = -20;

    Leg[5].mirrored = false;
    Leg[5].mountAngle = 30;

    Leg[5].minLED = 240;
    Leg[5].maxLED = 270;

#pragma endregion
    for (int i = 0; i < int(sizeof(Leg) / sizeof(Leg[0])); i++)
    {
        Leg[i].Servo[1].minAngle = 0;
        Leg[i].Servo[1].maxAngle = 180;
    }

#pragma endregion
}

void Servo_update(const Servo_Struct &servo, const int &onValue)
{
    int angle = constrain(servo.targetAngle, servo.minAngle, servo.maxAngle);
    // check which servo driver to use
    if (servo.ch < 16)
        servoDriver_0.setPWM(servo.ch, onValue, SERVO_BASELINE + (angle + servo.angleOffset) * 2);
    else if (servo.ch < 32)
        servoDriver_1.setPWM(servo.ch - 16, onValue, SERVO_BASELINE + (angle + servo.angleOffset) * 2);
}

void Servo_update(const int &servoCH, const int &angle, const int &onValue)
{
    // check which servo driver to use
    if (servoCH < 16)
        servoDriver_0.setPWM(servoCH, onValue, SERVO_BASELINE + angle * 2);
    else if (servoCH < 32)
        servoDriver_1.setPWM(servoCH - 16, onValue, SERVO_BASELINE + angle * 2);
}

// turns off the output for the servo
void Servo_deactivateAll()
{
    // loop through each leg
    for (size_t i = 0; i < 6; i++)
    {
        // loop through each servo in leg
        for (size_t o = 0; o < 3; o++)
        {
            Servo_update(Leg[i].Servo[o], 4096);
        }
    }
}

// used to test wether min values are correct
void Servo_moveAllToMinValue()
{
    // loop through each leg
    for (size_t i = 0; i < 6; i++)
    {
        // loop through each servo in leg
        for (size_t o = 0; o < 3; o++)
        {
            int oldTargetAngle = Leg[i].Servo[o].targetAngle;

            Leg[i].Servo[o].targetAngle = Leg[0].Servo[o].minAngle - abs(Leg[0].Servo[o].angleOffset);

            Servo_update(Leg[i].Servo[o]);

            Leg[i].Servo[o].targetAngle = oldTargetAngle;
        }
    }
}

// used to test wether max values are correct
void Servo_moveAllToMaxValue()
{
    // loop through each leg
    for (size_t i = 0; i < 6; i++)
    {
        // loop through each servo in leg
        for (size_t o = 0; o < 3; o++)
        {
            int oldTargetAngle = Leg[i].Servo[o].targetAngle;

            Leg[i].Servo[o].targetAngle = Leg[0].Servo[o].maxAngle + abs(Leg[0].Servo[o].angleOffset);

            Servo_update(Leg[i].Servo[o]);

            Leg[i].Servo[o].targetAngle = oldTargetAngle;
        }
    }
}

void Leg_update(const int &legID)
{
    calcLegServoAngles(Leg[legID]);

    Servo_update(Leg[legID].Servo[0]);
    Servo_update(Leg[legID].Servo[1]);
    Servo_update(Leg[legID].Servo[2]);
}

// update every output as needed to reflect any changes
void Output_update()
{
    Leg_update(0);
    Leg_update(1);
    Leg_update(2);
    Leg_update(3);
    Leg_update(4);
    Leg_update(5);
}

#endif

// ================================================================
// ===                           LED                            ===
// ================================================================
#ifdef WS2812B_LED

CRGB leds[NUM_LEDS];

void Led_init()
{
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); // GRB ordering is typical

    FastLED.setBrightness(brightness);

    // turn all led off
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void Led_update(const int &ledID)
{
    //leds[ledID] = CRGB::Red;
    
    leds[ledID] = CRGB(uint8_t(colorR), uint8_t(colorG), uint8_t(colorB));
}

#ifdef SERVO
void LED_leg_animation(const int &legID, const int &aminationID, const float &level)
{
    // animation ID: specify the type of animation
    // animation level: how far the animation has progressed (0-1)

    int LEDsOn = 0;

    switch (aminationID)
    {
    case 0: // bar
        LEDsOn = int(level * 31.0f);
        for (int i = 0; i < Leg[legID].maxLED - Leg[legID].minLED + 1; i++)
        {
            if (i < LEDsOn)
                Led_update(Leg[legID].minLED + i);
            else
                Led_update(Leg[legID].minLED + i);
        }
        break;

    case 1: // bar reverse
        LEDsOn = int(level * 31.0f);
        for (int i = Leg[legID].maxLED - Leg[legID].minLED + 1; i > 0; i--)
        {
            if (i < LEDsOn)
                Led_update(Leg[legID].maxLED - i);
            else
                Led_update(Leg[legID].maxLED - i);
        }
        break;

    default:
        return;
    }
}
#endif

#endif