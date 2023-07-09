#include <Arduino.h>
#include "header.h"

// ================================================================
// ===                          Servo                           ===
// ================================================================
#ifdef SERVO

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Servo_Struct Servo[3];

uint16_t curPosition = 280;

bool Servo_init()
{
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

    // configure channels
    Servo[0].ch = 13;
    Servo[1].ch = 14;
    Servo[2].ch = 15;

    Servo[1].minAngle = 0;
    Servo[2].minAngle = 45;

    Servo[1].maxAngle = 180;
    Servo[2].maxAngle = 180;

    // for (int i = 0; i < int(sizeof(Servo) / sizeof(Servo[0])); i++)
    // {
    // }

    return true;
}

bool Servo_update()
{
    int minVal = -10;
    int maxVal = 85;

    int timeDelay = 3;

    int coordinates[3] = {0, 0, 0};

    for (int i = maxVal; i > minVal; i--)
    {
        coordinates[0] = i;

        calcLegServo(coordinates, 0, Servo[0], Servo[1], Servo[1], Servo[0].targetAngle, Servo[1].targetAngle, Servo[2].targetAngle);

        pwm.setPWM(Servo[0].ch, 0, SERVO_BASELINE + Servo[0].targetAngle * 2);
        pwm.setPWM(Servo[1].ch, 0, SERVO_BASELINE + Servo[1].targetAngle * 2);
        pwm.setPWM(Servo[2].ch, 0, SERVO_BASELINE + Servo[2].targetAngle * 2);

        delay(timeDelay);
    }

    for (int i = minVal; i < maxVal; i++)
    {
        coordinates[0] = i;

        calcLegServo(coordinates, 0, Servo[0], Servo[1], Servo[1], Servo[0].targetAngle, Servo[1].targetAngle, Servo[2].targetAngle);

        pwm.setPWM(Servo[0].ch, 0, SERVO_BASELINE + Servo[0].targetAngle * 2);
        pwm.setPWM(Servo[1].ch, 0, SERVO_BASELINE + Servo[1].targetAngle * 2);
        pwm.setPWM(Servo[2].ch, 0, SERVO_BASELINE + Servo[2].targetAngle * 2);

        delay(timeDelay);
    }

    return true;
}

#endif