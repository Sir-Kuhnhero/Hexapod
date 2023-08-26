#include <Arduino.h>
#include <header.h>

// ================================================================
// ===                            IK                            ===
// ================================================================
#ifdef SERVO

int lengthTrochanter = 50;
int lengthFemur = 80;
int lengthTibia = 120;

#define LEG_ZERO_COORDINATE_X 130
#define LEG_ZERO_COORDINATE_Y 0
#define LEG_ZERO_COORDINATE_Z -120

void calcLegServo(Leg_Struct &leg)
{

    // ########################################################
    // ### adjust coodinated for loacal space (rotated leg) ###
    // ########################################################

    Vector3 localCoordinates;

    localCoordinates.x = cos(leg.mountAngle * DEG_TO_RAD) * leg.targetEndpoint.x + sin(leg.mountAngle * DEG_TO_RAD) * leg.targetEndpoint.y; // adjust for leg mounting rotation
    localCoordinates.x += LEG_ZERO_COORDINATE_X;                                                                                            // adjust for zero position (otherwise the leg origion is (0,0,0))

    localCoordinates.y = sin(leg.mountAngle * DEG_TO_RAD) * leg.targetEndpoint.x + cos(leg.mountAngle * DEG_TO_RAD) * leg.targetEndpoint.y;
    localCoordinates.y += LEG_ZERO_COORDINATE_Y;

    localCoordinates.z = leg.targetEndpoint.z + LEG_ZERO_COORDINATE_Z;

    // ###################
    // ### calc angles ###
    // ###################

    float noTrochanter = sqrt(pow(localCoordinates.x, 2) + pow(localCoordinates.y, 2)) - lengthTrochanter; // distance between Servo 2 and the tip on the XY Plane
    float servo2TipDistance = sqrt(pow(noTrochanter, 2) + pow(localCoordinates.z, 2));                     // distance between servo 2 and the tip

    float angle_0 = atan2(localCoordinates.x, localCoordinates.y) * RAD_TO_DEG * -1 + 180;

    float angleRightSideTriangle = atan2(localCoordinates.z, noTrochanter) * RAD_TO_DEG;
    float angleUnequalTriangle = acos((pow(lengthTibia, 2) - pow(servo2TipDistance, 2) - pow(lengthFemur, 2)) / (-2 * servo2TipDistance * lengthFemur)) * RAD_TO_DEG;

    float angleUnequalTriangle_2 = acos((pow(servo2TipDistance, 2) - pow(lengthTibia, 2) - pow(lengthFemur, 2)) / (-2 * lengthTibia * lengthFemur)) * RAD_TO_DEG; // second angle of unequal triangle

    int angle_1 = 0;
    int angle_2 = 0;

    if (leg.mirrored) // if the leg id mirrored the servos are installed slightly different
    {
        angle_1 = int(angleRightSideTriangle + angleUnequalTriangle) * -1 + 90;
        angle_2 = int(angleUnequalTriangle_2);
    }
    else
    {
        angle_1 = int(angleRightSideTriangle + angleUnequalTriangle) + 90;
        angle_2 = 180 - int(angleUnequalTriangle_2);
    }

    // constrain angles to the maximum of each servo
    constrain(angle_0, leg.Servo[0].minAngle, leg.Servo[0].maxAngle);
    constrain(angle_1, leg.Servo[1].minAngle, leg.Servo[1].maxAngle);
    constrain(angle_2, leg.Servo[2].minAngle, leg.Servo[2].maxAngle);

    // ####################################
    // ### set target angles for servos ###
    // ####################################

    leg.Servo[0].targetAngle = angle_0;
    leg.Servo[1].targetAngle = angle_1;
    leg.Servo[2].targetAngle = angle_2;
}

#endif

// ================================================================
// ===                           math                           ===
// ================================================================

Vector3 addVectors(const Vector3 &a, const Vector3 &b)
{
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vector3 scaleVector(const Vector3 &v, float scaleFactor)
{
    Vector3 result;
    result.x = v.x * scaleFactor;
    result.y = v.y * scaleFactor;
    result.z = v.z * scaleFactor;
    return result;
}

float dotProduct(const Vector3 &a, const Vector3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 interpolate(const Vector3 &start, const Vector3 &end, const float &t)
{
    Vector3 result;
    result.x = (1 - t) * start.x + t * end.x;
    result.y = (1 - t) * start.y + t * end.y;
    result.z = (1 - t) * start.z + t * end.z;
    return result;
}