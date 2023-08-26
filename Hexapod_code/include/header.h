#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#pragma once // Include guards to prevent multiple inclusion

#define WS2812B_LED

#define SERVO

#define DEBUG
// #define DEBUG_SERIAL
#define DEBUG_LED

// ================================================================
// ===                          struct                          ===
// ================================================================
// ############
// ### math ###
// ############
struct Vector3
{
    float x;
    float y;
    float z;

    Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}
};

// ##############
// ### output ###
// ##############
struct Servo_Struct
{
    int ch = 0;                        // channel on servo driver
    int minAngle = 60, maxAngle = 120; // max and min angle with 90° being the center position
    int targetAngle = 90;              // Servo angle from 0° to 180°
    int angleOffset = 0;               // if the servos are not installed correctly ((0,0,0) position does not form right angle) an offset can be applied
};

struct Leg_Struct
{
    int minLED, maxLED;     // index of LEDs on leg from root
    Servo_Struct Servo[3];  // servos for each leg (from root to tip)
    bool mirrored;          // ik for mirrored legs is slightly different
    int mountAngle;         // angle at which the leg is mounted (from center line forwared)
    Vector3 targetEndpoint; // enpoint used for ik calculations
};

// ================================================================
// ===                           main                           ===
// ================================================================
extern int loopTime;

// ================================================================
// ===                           math                           ===
// ================================================================
#ifdef SERVO

void calcLegServo(Leg_Struct &leg);

#endif

Vector3 addVectors(const Vector3 &a, const Vector3 &b);
Vector3 scaleVector(const Vector3 &v, float scaleFactor);
float dotProduct(const Vector3 &a, const Vector3 &b);
Vector3 interpolate(const Vector3 &start, const Vector3 &end, const float &t);

// ================================================================
// ===                           LED                            ===
// ================================================================

#ifdef WS2812B_LED
#include <FastLED.h>

#define NUM_LEDS 271

#pragma region ledNotes
// 0:       Eye center
// 1-8:     Eye middle ring
// 9-24:    Eye outer ring
// 25-84:   Bottom ring

// 85-115:  Leg[0]
// 85-91
// 92-94
// 95-115

// 116-146:  Leg[1]
// 116-122
// 123-125
// 126-146

// 147-177:  Leg[2]
// 147-153
// 154-156
// 157-177

// 178-208:  Leg[3]
// 178-184
// 185-187
// 188-208

// 209-239:  Leg[4]
// 209-215
// 216-218
// 219-239

// 240-270:  Leg[5]
// 240-246
// 247-249
// 250-270
#pragma endregion

#define DATA_PIN 3
#endif

// ================================================================
// ===                          output                          ===
// ================================================================

#ifdef SERVO

#include <Adafruit_PWMServoDriver.h>

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define SERVO_MIN 100
#define SERVO_MAX 450

#define SERVO_BASELINE 100

extern Leg_Struct Leg[6];

byte Servo_init();
void Servo_update(const Servo_Struct &servo);
void Servo_update(const int &servoCH, const int &angle);

void Leg_update(const int &legID, const Vector3 &endpoint);

#endif

#ifdef WS2812B_LED

byte Led_init();
void Led_update(const int &LedID, const CRGB &color);

#endif
// ================================================================
// ===                          debug                           ===
// ================================================================

#pragma region preventError
// if DEBUG is not enabled no data should be printed over the serial monitor
#ifndef DEBUG
#undef DEBUG_SERIAL
#endif

#pragma endregion

#ifdef DEBUG_SERIAL
void Debug_Serial_out();
void Debug_WaitForSerial();
void Debug_delay();
#endif

#ifdef DEBUG_LED

void Debug_Led(const byte &error_code);

#endif
// return 0b00000000;

// byte returnVal;
//    returnVal = single_servo_update(leg.Servo[0]);
// if (returnVal != 0b00000000) Debug_Led(returnVal);
