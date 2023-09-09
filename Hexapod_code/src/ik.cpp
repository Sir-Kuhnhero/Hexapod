#include <Arduino.h>
#include <header.h>

// ================================================================
// ===                            IK                            ===
// ================================================================
#ifdef SERVO

const Vector3 legZeroOffset(130, 0, -120); // local offset from leg origion to set the zero position somewhere more useful

void calcLegServoAngles(Leg_Struct &leg)
{

    // ###############################################################
    // ### adjust coodinated for loacal space of leg (rotated leg) ###
    // ###############################################################

    Vector3 localCoordinates;

    float sinAlpha = sin((leg.mountAngle) * DEG_TO_RAD);
    float cosAlpha = cos((leg.mountAngle) * DEG_TO_RAD);

    localCoordinates.x = cosAlpha * leg.targetPosition.x - sinAlpha * leg.targetPosition.y; // adjust for leg mounting rotation and zer0 position
    localCoordinates.x += legZeroOffset.x;                                                  // adjust for zero position (otherwise the leg origion is (0,0,0))

    localCoordinates.y = sinAlpha * leg.targetPosition.x + cosAlpha * leg.targetPosition.y;
    localCoordinates.y += legZeroOffset.y;

    localCoordinates.z = leg.targetPosition.z + legZeroOffset.z;

    // ###################
    // ### calc angles ###
    // ###################

    float noTrochanter = sqrt(pow(localCoordinates.x, 2) + pow(localCoordinates.y, 2)) - LENGTH_TROCHANTER; // distance between Servo 2 and the tip on the XY Plane
    float servo2TipDistance = sqrt(pow(noTrochanter, 2) + pow(localCoordinates.z, 2));                      // distance between servo 2 and the tip

    float angle_0 = atan2(localCoordinates.y, localCoordinates.x) * RAD_TO_DEG + 90;

    float angleRightSideTriangle = atan2(localCoordinates.z, noTrochanter) * RAD_TO_DEG;
    float angleUnequalTriangle = acos((pow(LENGTH_TIBIA, 2) - pow(servo2TipDistance, 2) - pow(LENGTH_FEMUR, 2)) / (-2 * servo2TipDistance * LENGTH_FEMUR)) * RAD_TO_DEG;

    float angleUnequalTriangle_2 = acos((pow(servo2TipDistance, 2) - pow(LENGTH_TIBIA, 2) - pow(LENGTH_FEMUR, 2)) / (-2 * LENGTH_TIBIA * LENGTH_FEMUR)) * RAD_TO_DEG; // second angle of unequal triangle

    int angle_1 = 0;
    int angle_2 = 0;

    if (leg.mirrored) // if the leg is mirrored the servos are installed slightly different
    {
        angle_1 = int(angleRightSideTriangle + angleUnequalTriangle) * -1 + 90;
        angle_2 = int(angleUnequalTriangle_2);
    }
    else
    {
        angle_1 = int(angleRightSideTriangle + angleUnequalTriangle) + 90;
        angle_2 = 180 - int(angleUnequalTriangle_2);
    }

    // ####################################
    // ### set target angles for servos ###
    // ####################################

    leg.Servo[0].targetAngle = angle_0;
    leg.Servo[1].targetAngle = angle_1;
    leg.Servo[2].targetAngle = angle_2;

    leg.curPosition = leg.targetPosition;
}

#endif