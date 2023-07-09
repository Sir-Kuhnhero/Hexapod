#include <Arduino.h>
#include <Wire.h>
#include <math.h>

// ================================================================
// ===                           main                           ===
// ================================================================
extern int loopTime;

// ================================================================
// ===                           LED                            ===
// ================================================================
// #define WS2812B_LED

#ifdef WS2812B_LED
#include <FastLED.h>

#define NUM_LEDS 31
#define DATA_PIN 3
#endif

// ================================================================
// ===                          output                          ===
// ================================================================
#define SERVO

#ifdef SERVO

#include <Adafruit_PWMServoDriver.h>

// extern Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
//  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define SERVO_MIN 100
#define SERVO_MAX 450

#define SERVO_BASELINE 100

struct Servo_Struct
{
    int ch = 0;                        // channel on servo driver
    int minAngle = 60, maxAngle = 120; // max and min angle with 90° being the center position
    int targetAngle = 90;              // Servo angle from 0° to 180°
};

extern Servo_Struct Servo[3];

bool Servo_init();
bool Servo_update();

#endif

// ================================================================
// ===                          debug                           ===
// ================================================================
// #define DEBUG
#define SERIAL_out

#pragma region preventError
// if DEBUG is not enabled no data should be printed over the serial monitor
#ifndef DEBUG
#undef SERIAL_out
#endif
#ifndef SERIAL_out

#endif
// if a module is not implemented it also can not be printed over the serial monitor

#pragma endregion

#ifdef DEBUG
void Debug_Serial_out();
void Debug_WaitForSerial();
void Debug_delay();
#endif

// ================================================================
// ===                            IK                            ===
// ===============================================================
void calcLegServo(int coordinates[3], int angle, Servo_Struct &Servo_0, Servo_Struct &Servo_1, Servo_Struct &Servo_2, int &v1, int &v2, int &v3);