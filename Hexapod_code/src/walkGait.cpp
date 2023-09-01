#include <Arduino.h>
#include <header.h>

// ================================================================
// ===                         walkGait                         ===
// ================================================================

int legLiftDistance;
int legLiftIncline;

// move Legs to start and stand up
void LegStartup()
{
    // move up to start
    Vector3 startPosition(0.0f, 0.0f, 120.0f);

    for (int i = 0; i < 6; i++)
    {
        Leg[i].targetEndpoint = startPosition;
        Leg_update(i);
        delay(250);
    }

    // slowly move legs down
    Vector3 curPosition(startPosition.x, startPosition.y, startPosition.z);

    for (curPosition.z = startPosition.z; curPosition.z > 0; curPosition.z--)
    {
        for (int i = 0; i < 6; i++)
        {
            Leg[i].targetEndpoint = curPosition;
        }

        Output_update();

        delay(5);
    }

    // reposition legs to zero
    startPosition = curPosition;
    Vector3 liftPositio(curPosition.x / 2, curPosition.y, 50);
    Vector3 downPosition;

    int steps = 50;    // steps for interpolation
    int timeDelta = 2; // tiem between steps

#pragma region repositionLeg_0_2_4
    for (int i = 0; i < steps; i++)
    {
        curPosition = Vector3::Lerp(startPosition, liftPositio, float(i) / float(steps));

        Leg[0].targetEndpoint = curPosition;
        Leg[2].targetEndpoint = curPosition;
        Leg[4].targetEndpoint = curPosition;

        Leg_update(0);
        Leg_update(2);
        Leg_update(4);

        delay(timeDelta);
    }

    // move last 3 legs down
    for (int i = 0; i < steps; i++)
    {
        curPosition = Vector3::Lerp(liftPositio, downPosition, float(i) / float(steps));

        Leg[0].targetEndpoint = curPosition;
        Leg[2].targetEndpoint = curPosition;
        Leg[4].targetEndpoint = curPosition;

        Leg_update(0);
        Leg_update(2);
        Leg_update(4);

        delay(timeDelta);
    }

#pragma endregion

#pragma region repositionLeg_1_3_5
    for (int i = 0; i < steps; i++)
    {
        curPosition = Vector3::Lerp(startPosition, liftPositio, float(i) / float(steps));

        Leg[1].targetEndpoint = curPosition;
        Leg[3].targetEndpoint = curPosition;
        Leg[5].targetEndpoint = curPosition;

        Leg_update(1);
        Leg_update(3);
        Leg_update(5);

        delay(timeDelta);
    }

    // move last 3 legs down
    for (int i = 0; i < steps; i++)
    {
        curPosition = Vector3::Lerp(liftPositio, downPosition, float(i) / float(steps));

        Leg[1].targetEndpoint = curPosition;
        Leg[3].targetEndpoint = curPosition;
        Leg[5].targetEndpoint = curPosition;

        Leg_update(1);
        Leg_update(3);
        Leg_update(5);

        delay(timeDelta);
    }

#pragma endregion
}

// returns a point on a circle based on a point inside the circle that is projected by a vector
Vector2 projectPointToCircle(const float &radius, const Vector2 &point, Vector2 direction)
{
    // no direction for projection -> no calculation
    if (direction.magnitude() == 0)
        return point;
    // start bein (0, 0) or haveing the same direction as directionInput makes the calculation very simple
    if (point.magnitude() == 0 || point.normalized() == direction.normalized() || point.normalized() * -1 == direction.normalized())
        return direction.normalized() * radius;

    // if point is outside the circle reposition it to be inside
    if (direction.magnitude() > radius)
        Vector2::ClampMagnitude(direction, radius - 0.005);

    float lengthC = point.magnitude();

    float angleBeta = 180 - Vector2::Angle(direction, point); // calculate angle beta on unequal triangle

    // calculate missing Angles
    float sinGamma = (lengthC * sin(angleBeta * DEG_TO_RAD)) / radius;

    float angleGamma = asin(sinGamma) * RAD_TO_DEG;
    float angleAlpha = 180 - angleBeta - angleGamma;

    float projectionLength = (radius * sin(angleAlpha * DEG_TO_RAD)) / sin(angleBeta * DEG_TO_RAD);

    Vector2 output = point + direction.normalized() * projectionLength;

    return output;
}

void walkCycle(Vector2 direction, const int &height, const int &stepRadius)
{
    int liftedLegID;
    int pushLegID;
    // scale direction vector by passed time (mm per second)
    direction = direction * (float(loopTime) / 1000.0f);

    // #########################################
    // ### set lifted leg and push leg group ###
    // #########################################
    // set ID for lifted group and push group. Three legs work in sync so Leg[0] and Leg[1] can be used for each
    if (Leg[0].lifted && !Leg[1].lifted) // group 1 is lifted | group 2 is not lifted
    {
        liftedLegID = 0;
        pushLegID = 1;
    }
    else if (!Leg[0].lifted && Leg[1].lifted) // group 1 is not lifted | group 2 is lifted
    {
        liftedLegID = 1;
        pushLegID = 0;
    }
    else if (!Leg[0].lifted && !Leg[1].lifted) // group 1 & 2 is not lifted -> start new walk cycle
    {
        // choose new lifted pair based on their position (which group makes more sense for the next step)
        Vector2 curPointGroup1(Leg[liftedLegID].targetEndpoint.x, Leg[liftedLegID].targetEndpoint.y);
        Vector2 curPointGroup2(Leg[pushLegID].targetEndpoint.x, Leg[pushLegID].targetEndpoint.y);

        Vector2 targetGroup1 = projectPointToCircle(stepRadius, curPointGroup1, direction);
        Vector2 targetGroup2 = projectPointToCircle(stepRadius, curPointGroup2, direction);

        Vector2 projectedVectorGroup1(targetGroup1.x - Leg[0].targetEndpoint.x, targetGroup1.y - Leg[0].targetEndpoint.y);
        Vector2 projectedVectorGroup2(targetGroup2.x - Leg[1].targetEndpoint.x, targetGroup2.y - Leg[1].targetEndpoint.y);

        if (almostEqual(projectedVectorGroup1.magnitude(), projectedVectorGroup2.magnitude())) // both legs can move the same distance
        {
            Leg[0].lifted = true;
            Leg[1].lifted = false;
        }
        else if (projectedVectorGroup1.magnitude() > projectedVectorGroup2.magnitude()) // group 1 can take a longer step
        {
            Leg[0].lifted = true;
            Leg[1].lifted = false;
        }
        else // group 2 can take a longer step
        {
            Leg[0].lifted = false;
            Leg[1].lifted = true;
        }

        return;
    }
    else // !! both legs are lifted !!
    {
        Debug_Led_8bit(0b01010101);
        delay(1000);

        Leg[0].lifted = false;
        Leg[1].lifted = false;

        return;
    }

    // #############################################
    // ### calc new target point on outer circle ###
    // #############################################

    Vector2 curPointLiftedGroup(Leg[liftedLegID].targetEndpoint.x, Leg[liftedLegID].targetEndpoint.y);
    Vector2 curPointPushGroup(Leg[pushLegID].targetEndpoint.x, Leg[pushLegID].targetEndpoint.y);

    Vector2 targetCircleLiftedGroup = projectPointToCircle(stepRadius, curPointLiftedGroup, direction);
    Vector2 targetCirclePushGroup = projectPointToCircle(stepRadius, curPointPushGroup, direction.inverse());

    //
    //
    //
    //
    //
    //
    //
    //
    // ###############################################
    // ### calc subtarget based on target valocity ###
    // ###############################################
    Vector3 curPointToCirleTargetLifted(targetCircleLiftedGroup.x - curPointLiftedGroup.x, targetCircleLiftedGroup.y - curPointLiftedGroup.y, 0);
    Vector3 curPointToCirleTargetPush(targetCirclePushGroup.x - curPointPushGroup.x, targetCirclePushGroup.y - curPointPushGroup.y, 0);

    // calc time to target for push legs (in seconds)
    float timeToTargetPush = (curPointToCirleTargetPush).magnitude() / direction.magnitude();

    // !! not correct as it does not add the curPoint ot point in circle !! calc subPoints for lifted legs
    Vector3 subTarget[2];
    subTarget[0] = curPointToCirleTargetLifted.normalized() * (legLiftDistance / legLiftIncline);
    subTarget[0].z += legLiftDistance;

    subTarget[1] = curPointToCirleTargetLifted.normalized().inverse() * (legLiftDistance / legLiftIncline);
    subTarget[1].z += legLiftDistance;

    // check wether curPoint to subTarget1 & subTarget2 to targetCircle intersect -> if true set this intersection point as subtarget and overwrite the other two

    // calc speed needed to reach the target at the same time as push legs
    // (possibly add an offset to give the currently lifted legs (new push legs) some time on the ground before the old legs are lifted)

    // if speed is over max speed slow push legs down as needed

    // calculate sub target

    Vector3 targetGroup1(targetCircleLiftedGroup.x, targetCircleLiftedGroup.y, 0);
    Vector3 targetGroup2(targetCirclePushGroup.x, targetCirclePushGroup.y, 0);

    // interpolate based on target speed
    Vector3 nextTargetGroup1 = (targetGroup1 - Leg[0].targetEndpoint).normalized() * direction.magnitude() + Leg[0].targetEndpoint;
    Vector3 nextTargetGroup2 = (targetGroup2 - Leg[1].targetEndpoint).normalized() * direction.magnitude() + Leg[1].targetEndpoint;

    // clamp radius to make shure the leg never leaves the circle for projections
    Vector3 nextTargetGroup1Clamped = Vector3::ClampMagnitude(nextTargetGroup1, stepRadius);
    Vector3 nextTargetGroup2Clamped = Vector3::ClampMagnitude(nextTargetGroup2, stepRadius);

    // set target height for legs groups
    nextTargetGroup1Clamped.z = height + (Leg[0].lifted ? legLiftDistance : height);
    nextTargetGroup2Clamped.z = height + (Leg[1].lifted ? legLiftDistance : height);

    // switch lifted leg group on target reach
    bool group1ReachedTarget = (nextTargetGroup1Clamped - Leg[0].targetEndpoint).magnitude() < direction.magnitude() / 2;
    bool group2ReachedTarget = (nextTargetGroup2Clamped - Leg[1].targetEndpoint).magnitude() < direction.magnitude() / 2;

    if (group1ReachedTarget, group2ReachedTarget) // group 1 & 2 has reached its target
    {
        Leg[0].lifted = !Leg[0].lifted;
        Leg[1].lifted = !Leg[1].lifted;
    }

    //
    //
    //
    //
    //
    //
    // #############################
    // ### set calculated values ###
    // #############################
    // three legs move in together
    Leg[0].targetEndpoint = nextTargetGroup1Clamped;
    Leg[2].targetEndpoint = nextTargetGroup1Clamped;
    Leg[4].targetEndpoint = nextTargetGroup1Clamped;

    Leg[1].targetEndpoint = nextTargetGroup2Clamped;
    Leg[3].targetEndpoint = nextTargetGroup2Clamped;
    Leg[5].targetEndpoint = nextTargetGroup2Clamped;
}