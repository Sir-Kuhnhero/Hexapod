#include <Arduino.h>
#include <header.h>

int lengthTrochanter = 50;
int lengthFemur = 80;
int lengthTibia = 120;

#define LEG_ZERO_COORDINATE_X 150
#define LEG_ZERO_COORDINATE_Y 0
#define LEG_ZERO_COORDINATE_Z 0

void calcLegServo(int coordinates[3], int angle, Servo_Struct &Servo_0, Servo_Struct &Servo_1, Servo_Struct &Servo_2, int &v1, int &v2, int &v3)
{

    // adjust coodinated for loacal space (rotated leg)
    int localCoordinates[3] = {0, 0, 0};

    localCoordinates[0] = cos(angle * DEG_TO_RAD) * coordinates[0] + sin(angle * DEG_TO_RAD) * coordinates[1] + LEG_ZERO_COORDINATE_X;
    localCoordinates[1] = sin(angle * DEG_TO_RAD) * coordinates[0] + cos(angle * DEG_TO_RAD) * coordinates[1] + LEG_ZERO_COORDINATE_Y;
    localCoordinates[2] += LEG_ZERO_COORDINATE_Z;

    // calc angles
    int angle_1 = int(90 - atan2(localCoordinates[0], localCoordinates[1]) * RAD_TO_DEG) + 90;

    int noTrochanter = sqrt(pow(localCoordinates[0], 2) + pow(localCoordinates[1], 2)) - lengthTrochanter; // distance between Servo 2 and the tip ignoring the y and z component
    int servo2TipDistance = sqrt(pow(noTrochanter, 2) + pow(localCoordinates[2], 2));

    int angle_2 = int(atan(localCoordinates[2] * -1 / noTrochanter) * RAD_TO_DEG + 90 + acos((pow(lengthTibia, 2) - pow(servo2TipDistance, 2) - pow(lengthFemur, 2)) / (-2 * servo2TipDistance * lengthFemur)) * RAD_TO_DEG);

    int angle_3 = 180 - int(acos((pow(servo2TipDistance, 2) - pow(lengthTibia, 2) - pow(lengthFemur, 2)) / (-2 * lengthTibia * lengthFemur)) * RAD_TO_DEG);

    v1 = constrain(angle_1, Servo_0.minAngle, Servo_0.maxAngle);
    v2 = constrain(angle_2, Servo_1.minAngle, Servo_1.maxAngle);
    v3 = constrain(angle_3, Servo_2.minAngle, Servo_2.maxAngle);
}