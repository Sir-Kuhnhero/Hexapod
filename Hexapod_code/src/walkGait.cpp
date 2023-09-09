#include <Arduino.h>
#include <header.h>

// ================================================================
// ===                         walkGait                         ===
// ================================================================

int legLiftDistance = 30;
float legLiftIncline = 2;

// move Legs to start and stand up
void LegStartup()
{
    // move up to start
    Vector3 startPosition(0.0f, 0.0f, 120.0f);

    for (int i = 0; i < 6; i++)
    {
        Leg[i].targetPosition = startPosition;
        Leg_update(i);
        delay(250);
    }

    // slowly move legs down
    Vector3 curPosition(startPosition.x, startPosition.y, startPosition.z);

    for (curPosition.z = startPosition.z; curPosition.z > 0; curPosition.z--)
    {
        for (int i = 0; i < 6; i++)
        {
            Leg[i].targetPosition = curPosition;
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

        Leg[0].targetPosition = curPosition;
        Leg[2].targetPosition = curPosition;
        Leg[4].targetPosition = curPosition;

        Leg_update(0);
        Leg_update(2);
        Leg_update(4);

        delay(timeDelta);
    }

    // move last 3 legs down
    for (int i = 0; i < steps; i++)
    {
        curPosition = Vector3::Lerp(liftPositio, downPosition, float(i) / float(steps));

        Leg[0].targetPosition = curPosition;
        Leg[2].targetPosition = curPosition;
        Leg[4].targetPosition = curPosition;

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

        Leg[1].targetPosition = curPosition;
        Leg[3].targetPosition = curPosition;
        Leg[5].targetPosition = curPosition;

        Leg_update(1);
        Leg_update(3);
        Leg_update(5);

        delay(timeDelta);
    }

    // move last 3 legs down
    for (int i = 0; i < steps; i++)
    {
        curPosition = Vector3::Lerp(liftPositio, downPosition, float(i) / float(steps));

        Leg[1].targetPosition = curPosition;
        Leg[3].targetPosition = curPosition;
        Leg[5].targetPosition = curPosition;

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

void walkCycle(Vector2 direction, const int &height, const int &rotation, const int &stepRadius)
{
    int liftedLegID;
    int pushLegID;
    // scale direction vector by passed time (mm per second)
    direction = direction * (float(loopTime) / 1000.0f);

    // ###################################
    // ### set lifted & push leg group ###
    // ###################################

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
        Vector2 curPointGroup1(Leg[liftedLegID].curPosition.x, Leg[liftedLegID].curPosition.y);
        Vector2 curPointGroup2(Leg[pushLegID].curPosition.x, Leg[pushLegID].curPosition.y);

        Vector2 targetGroup1 = projectPointToCircle(stepRadius, curPointGroup1, direction);
        Vector2 targetGroup2 = projectPointToCircle(stepRadius, curPointGroup2, direction);

        Vector2 projectedVectorGroup1(targetGroup1.x - Leg[0].curPosition.x, targetGroup1.y - Leg[0].curPosition.y);
        Vector2 projectedVectorGroup2(targetGroup2.x - Leg[1].curPosition.x, targetGroup2.y - Leg[1].curPosition.y);

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
        // temp for testing
        Leg[0].lifted = true;
        Leg[1].lifted = false;
        Leg[2].lifted = true;
        Leg[3].lifted = false;
        Leg[4].lifted = true;
        Leg[5].lifted = false;
        return;
    }
    else // !! both legs are lifted !!
    {
        Debug_Led_8bit(0b00000011);
        delay(5000);

        Leg[0].lifted = false;
        Leg[1].lifted = false;

        return;
    }
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    for (size_t i = 0; i < 6; i++)
    {
        Vector2 projectionDirection;
        Vector2 projectionOrigion;

        if (Leg[i].lifted)
        {
            projectionDirection = direction;
            projectionOrigion = Vector2::zero;
        }
        else
        {
            projectionDirection = direction.inverse();

            Vector2 newOrigion(Leg[i].curPosition.x, Leg[i].curPosition.y);
            projectionOrigion = newOrigion;
        }

        Vector3 target = projectPointToCircle(stepRadius, projectionOrigion, projectionDirection).toVector3();
        target.z += height;

        Vector3 curPointToTarget = target - Leg[i].curPosition;

        Vector3 section[3]; // path of lifted legs are seperated into three sections (lift, travel and lower). These add up for the path. For push Leg section[0,1,2] = Vector3::zero
        if (Leg[i].lifted)
        {
            // calc subTargets for lifted legs
            float missingHeight = target.z + legLiftDistance - Leg[i].curPosition.z; // height difference from CurPoint.z to LegLiftDistance
            float distanceCurPointSubTarget1_XYPlane = fabs(missingHeight) / legLiftIncline;
            float distanceTargetCircleSubTarget2_XYPlane = legLiftDistance / legLiftIncline;

            float distanceCurPointTargetCircle_XYPlane = curPointToTarget.xyPlane().magnitude();

            // if leg is already passed first subTarget or the two subTargets would interfere only one will be calculated and stepHeight might differe from desired ledLiftDistance
            int numbOfSubTargets = 0; // number of subtargets to be calculated for path of lifted leg
            float stepHeight = legLiftDistance;

            // true if two subTargets are needed
            if (distanceCurPointSubTarget1_XYPlane + distanceTargetCircleSubTarget2_XYPlane < distanceCurPointTargetCircle_XYPlane && !almostEqual(fabs(missingHeight), 0, 2))
            {
                numbOfSubTargets = 2;
            }
            else if (missingHeight < 2)
            {
                numbOfSubTargets = 1;
                stepHeight = legLiftDistance - ((distanceCurPointSubTarget1_XYPlane + distanceTargetCircleSubTarget2_XYPlane - distanceCurPointTargetCircle_XYPlane) / 2) * legLiftIncline;
            }
            else
            {
                numbOfSubTargets = 1;
                stepHeight = distanceCurPointTargetCircle_XYPlane * legLiftIncline;
            }

            // calc path to and between subTargets
            section[2] = curPointToTarget.xyPlane().normalized() * (stepHeight / legLiftIncline);
            section[2].z = stepHeight * -1;

            if (numbOfSubTargets == 2)
            {
                section[0] = curPointToTarget.xyPlane().normalized() * (fabs(missingHeight) / legLiftIncline);
                section[0].z = missingHeight;
            }

            section[1] = (target - section[2]) - (Leg[i].curPosition + section[0]);
        }

        Leg[i].pointOnPath.resize(4);

        Leg[i].pointOnPath[0] = Leg[i].curPosition;
        Leg[i].pointOnPath[1] = Leg[i].pointOnPath[0] + section[0];
        Leg[i].pointOnPath[2] = Leg[i].pointOnPath[1] + section[1];
        Leg[i].pointOnPath[3] = target;
    }

    // find leg path length and leg speed to reach target in time
    std::vector<float> pathLength;
    pathLength.resize(6);

    float substepLength[6];

    for (size_t i = 0; i < 6; i++)
    {
        pathLength[i] = calculatePathLength(Leg[i].pointOnPath);
    }

    float subSteps = pathLength[findLongestPath(pathLength)] / direction.magnitude();

    for (size_t i = 0; i < 6; i++)
    {
        substepLength[i] = pathLength[i] / subSteps;
    }

    Vector3 nextTarget[6];

    for (size_t i = 0; i < 6; i++)
    {
        nextTarget[i] = interpolatePathByLength(Leg[i].pointOnPath, substepLength[i]);
    }

    // switch lifted leg group on target reach
    /*
    bool reachedTarget[6];

    for (size_t i = 0; i < 6; i++)
    {
        reachedTarget[i] = (nextTarget[i] - Leg[i].curPosition).magnitude() < direction.magnitude() / 2;
    }

     lifted and push has reached its target and now have to switch roles
     if (reachedTarget[0] && reachedTarget[1] && reachedTarget[2] && reachedTarget[3] && reachedTarget[4] && reachedTarget[5])
    {
        Leg[0].lifted = !Leg[0].lifted;
        Leg[1].lifted = !Leg[0].lifted;
        Leg[2].lifted = !Leg[1].lifted;
        Leg[3].lifted = !Leg[0].lifted;
        Leg[4].lifted = !Leg[1].lifted;
        Leg[5].lifted = !Leg[0].lifted;

        Debug_Led_8bit(0b01010101);
    }
    */

    // #############################
    // ### set calculated values ###
    // #############################

    for (size_t i = 0; i < 6; i++)
    {
        Leg[i].targetPosition = nextTarget[i];
    }

    return;
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    /*
        // #############################################
        // ### calc new target point on outer circle ###
        // #############################################

        Vector2 curPointLiftedGroup_XYPlane(Leg[liftedLegID].curPosition.x, Leg[liftedLegID].curPosition.y);
        Vector2 curPointPushGroup_XYPlane(Leg[pushLegID].curPosition.x, Leg[pushLegID].curPosition.y);

        Vector3 curPointLiftedGroup = Leg[liftedLegID].curPosition;
        Vector3 curPointPushGroup = Leg[pushLegID].curPosition;

        Vector2 targetCircleLiftedGroup_XYPlane = projectPointToCircle(stepRadius, curPointLiftedGroup_XYPlane, direction);
        Vector2 targetCirclePushGroup_XYPlane = projectPointToCircle(stepRadius, curPointPushGroup_XYPlane, direction.inverse());

        Vector3 targetCircleLiftedGroup(targetCircleLiftedGroup_XYPlane.x, targetCircleLiftedGroup_XYPlane.y, height);
        Vector3 targetCirclePushGroup(targetCirclePushGroup_XYPlane.x, targetCirclePushGroup_XYPlane.y, height);

        // ######################################
        // ### calc subTarget for lifted legs ###
        // ######################################

        Vector3 curPointToTarget(targetCircleLiftedGroup.x - curPointLiftedGroup.x, targetCircleLiftedGroup.y - curPointLiftedGroup.y, targetCircleLiftedGroup.z - curPointLiftedGroup.z);
        Vector3 curPointCirleTargetPush(targetCirclePushGroup.x - curPointPushGroup.x, targetCirclePushGroup.y - curPointPushGroup.y, targetCirclePushGroup.z - curPointPushGroup.z);

        // calc time to target for push legs (in seconds)
        float timeToTargetPush = (curPointCirleTargetPush).magnitude() / direction.magnitude();

        // calc subTargets for lifted legs
        float missingHeight = targetCircleLiftedGroup.z + legLiftDistance - curPointLiftedGroup.z;
        float distanceCurPointSubTarget1_XYPlane = fabs(missingHeight) / legLiftIncline;
        float distanceTargetCircleSubTarget2_XYPlane = legLiftDistance / legLiftIncline;

        Vector2 curPointToTarget_XYPlane(curPointToTarget.x, curPointToTarget.y);
        float distanceCurPointTargetCircle_XYPlane = curPointToTarget_XYPlane.magnitude();

        // if leg is already passed first subTarget or the two subTargets would interfere only one will be calculated and stepHeoght might differe from desired ledLiftDistance
        int numbOfSubTargets = 0; // number of subtargets to be calculated for path of lifted leg
        float stepHeight = legLiftDistance;

        // true if two subTargets are needed
        if (distanceCurPointSubTarget1_XYPlane + distanceTargetCircleSubTarget2_XYPlane < distanceCurPointTargetCircle_XYPlane && !almostEqual(fabs(missingHeight), 0, 2))
        {
            numbOfSubTargets = 2;
        }
        else if (missingHeight < 2)
        {
            numbOfSubTargets = 1;

            float stepHeightBy = legLiftDistance - ((distanceCurPointSubTarget1_XYPlane + distanceTargetCircleSubTarget2_XYPlane - distanceCurPointTargetCircle_XYPlane) / 2) * legLiftIncline;
        }
        else
        {
            numbOfSubTargets = 1;
            stepHeight = distanceCurPointTargetCircle_XYPlane * legLiftIncline;
        }

        // calc path to and between subTargets
        Vector3 section[3]; // path of lifted legs are seperated into three sections (lift, travel and lower). These add up to the path

        Vector3 directionIn3D(direction.x, direction.y, 0);

        section[2] = directionIn3D.normalized() * (stepHeight / legLiftIncline);
        section[2].z = stepHeight * -1;

        if (numbOfSubTargets == 2)
        {
            section[0] = directionIn3D.normalized() * (missingHeight / legLiftIncline);
            section[0].z = missingHeight;
        }

        section[1] = (targetCircleLiftedGroup - section[2]) - (curPointLiftedGroup + section[0]);

        // calc speed needed to reach the target at the same time as push legs
        float speedLifted = (section[0].magnitude() + section[1].magnitude() + section[2].magnitude()) / timeToTargetPush;
        // (possibly add an offset to give the currently lifted legs (new push legs) some time on the ground before the old legs are lifted)

        // !! not implemented !! if speed is over max speed slow push legs down as needed
        float speedPush = direction.magnitude();

        // #################################################
        // ### interpolate to next target based on speed ###
        // #################################################

        // !! needs change to anticipate future movement and smooth corners
        float interpolationSection[3]; // how much each section is interpolated (0 - 1) 0 means section[i] * 0

        // if we need to interpolate more that a single section allowes overflow the rest to the next section
        interpolationSection[0] = constrain(speedLifted, 0, section[0].magnitude());
        interpolationSection[1] = constrain(speedLifted - interpolationSection[0], 0, section[1].magnitude());
        interpolationSection[2] = constrain(speedLifted - interpolationSection[0] - interpolationSection[1], 0, section[2].magnitude());

        Vector3 nextTargetLifted = section[0].normalized() * interpolationSection[0] + section[1].normalized() * interpolationSection[1] + section[2].normalized() * interpolationSection[2] + curPointLiftedGroup;

        Vector3 nextTargetPush = (curPointCirleTargetPush).normalized() * speedPush + curPointPushGroup;

        // clamp radius to make shure the leg never leaves the circle for projections
        Vector3 nextTargetLiftedClamped = Vector3::ClampMagnitude(nextTargetLifted, stepRadius * 2);
        Vector3 nextTargetPushClamped = Vector3::ClampMagnitude(nextTargetPush, stepRadius);

        // switch lifted leg group on target reach
        bool liftedReachedTarget = (nextTargetLiftedClamped - curPointLiftedGroup).magnitude() < speedLifted / 2;
        bool pushReachedTarget = (nextTargetPushClamped - curPointPushGroup).magnitude() < speedPush / 2;

        if (liftedReachedTarget, pushReachedTarget) // lifted and push has reached its target and now have to switch
        {
            Leg[0].lifted = !Leg[0].lifted;
            Leg[1].lifted = !Leg[0].lifted;
        }

        // #############################
        // ### set calculated values ###
        // #############################

        // three legs move in together
        Leg[liftedLegID].targetPosition = nextTargetLiftedClamped;
        Leg[liftedLegID + 2].targetPosition = nextTargetLiftedClamped;
        Leg[liftedLegID + 4].targetPosition = nextTargetLiftedClamped;

        Leg[pushLegID].targetPosition = nextTargetPushClamped;
        Leg[pushLegID + 2].targetPosition = nextTargetPushClamped;
        Leg[pushLegID + 4].targetPosition = nextTargetPushClamped;
        */
}
