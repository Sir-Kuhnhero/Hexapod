#include <Arduino.h>
#include <header.h>

// ================================================================
// ===                         walkGait                         ===
// ================================================================

#ifdef SERVO

int legLiftDistance = 25;
float legLiftIncline = 2;

float deltaTime;
Vector2 direction;
float rotation;

void setLegStateAtWalkInit();
void setLegStateAtTargetReach(const Vector3 (&interpolatedTarget)[6]);
Vector3 calcTarget(Leg_Struct &leg);
void calcLegPath(Leg_Struct &leg, const Vector3 &target);
void calcInterpolatedTarget(Vector3 (&interpolatedTarget)[6]);

// move Legs to start and stand up
void standUp()
{
    if (HexapodState != State::SITTING)
    {
        return;
    }

    // move up to start
    Vector3 startPosition(0.0f, 0.0f, 120.0f);

    for (int i = 0; i < 6; i++)
    {
        Leg[i].targetPosition = startPosition;
        Leg_update(i);
        delay(250);
    }

    HexapodState = State::STANDING;
}

void sitDown()
{
    float initialGroundClearance = groundClearance;

    directionInput = Vector2::zero;
    rotation = 0;

    loopTime = 10; // manually set looptime. It's a bit jank but it works

    // slowly move legs upwards
    for (size_t i = initialGroundClearance; i > -40; i--)
    {
        groundClearance = i;

        walkCycle();
        Output_update();
        delay(10);
    }

    Servo_deactivateAll();

    HexapodState = State::SITTING;
}

void walkCycle()
{
    #ifdef SERVO_CALIBRATION
    for (size_t i = 0; i < 6; i++)
    {
        Vector3 newPosition;
        //set new position to 0 for each axis
        newPosition.x = 0;
        newPosition.y = 0;
        newPosition.z = 0;

        Leg[i].targetPosition = newPosition;
    }

    return;
    #endif

    // scale direction vector by passed time (mm per second)
    deltaTime = (float(loopTime) / 1000.0f);
    direction = directionInput * deltaTime;
    rotation = rotationInput * deltaTime;

    if ((direction.magnitude() > 0 || rotation != 0))
    {
        HexapodState = State::WALKING;
    }
    else
    {
        HexapodState = State::STANDING;
    }

    // ######################################
    // ### set lifted & push leg at start ###
    // ######################################

    if (!Leg[0].lifted && !Leg[1].lifted && HexapodState == State::WALKING) // group 1 & 2 is not lifted -> start new walk cycle
    {
        setLegStateAtWalkInit();
    }

    // #######################################
    // ### calculare target and subTargets ###
    // #######################################

    for (size_t i = 0; i < 6; i++)
    {
        calcLegPath(Leg[i], calcTarget(Leg[i]));
    }

    // #######################
    // ### calc nextTarget ###
    // #######################

    Vector3 interpolatedTarget[6];
    calcInterpolatedTarget(interpolatedTarget);

    // switch lifted leg group on target reach
    setLegStateAtTargetReach(interpolatedTarget);

    // #############################
    // ### set calculated values ###
    // #############################

    for (size_t i = 0; i < 6; i++)
    {
        Leg[i].targetPosition = interpolatedTarget[i];
    }
}

// choose lifted legs for furthes step (only when all legs are on the ground)
void setLegStateAtWalkInit()
{
    // choose new lifted legs based on their position (which group makes more sense for the next step)
    float possibleStepLength[6]; // how long of a step can the leg take while being in contackt with the ground

    for (size_t i = 0; i < 6; i++)
    {
        // calc distance the leg can travel as Push leg
        Leg[i].lifted = false;
        calcLegPath(Leg[i], calcTarget(Leg[i]));

        possibleStepLength[i] = calculatePathLength(Leg[i].pointOnPath);
    }

    // static array has to first be converted to dynamic array
    std::vector<float> dynamicArray;
    dynamicArray.resize(6);

    for (size_t i = 0; i < dynamicArray.size(); i++)
    {
        dynamicArray[i] = possibleStepLength[i];
    }

    // legs act in group of threes (0, 2, 4 and 1, 3, 5) -> get leg for each group that takes the smalles step
    int indexOfShortesPath = findSmallestValue(dynamicArray);

    if (indexOfShortesPath % 2 == 0)
    {
        Leg[0].lifted = false;
        Leg[1].lifted = true;
        Leg[2].lifted = false;
        Leg[3].lifted = true;
        Leg[4].lifted = false;
        Leg[5].lifted = true;
    }
    else
    {
        Leg[0].lifted = true;
        Leg[1].lifted = false;
        Leg[2].lifted = true;
        Leg[3].lifted = false;
        Leg[4].lifted = true;
        Leg[5].lifted = false;
    }
}

// switch lifted leg group on target reach
void setLegStateAtTargetReach(const Vector3 (&interpolatedTarget)[6])
{
    bool reachedTarget[6];

    float threshold = (direction.magnitude() + fabs(rotation)) / 2;

    if (threshold == 0)
    {
        threshold = (maxSpeed * deltaTime) / 2;
    }

    for (size_t i = 0; i < 6; i++)
    {
        reachedTarget[i] = (interpolatedTarget[i] - Leg[i].curPosition).magnitude() < threshold;
    }

    // lifted and push has reached its target and now have to switch roles
    if (reachedTarget[0] && reachedTarget[1] && reachedTarget[2] && reachedTarget[3] && reachedTarget[4] && reachedTarget[5])
    {
        Leg[0].lifted = !Leg[0].lifted;
        Leg[1].lifted = !Leg[0].lifted;
        Leg[2].lifted = !Leg[1].lifted;
        Leg[3].lifted = !Leg[0].lifted;
        Leg[4].lifted = !Leg[1].lifted;
        Leg[5].lifted = !Leg[0].lifted;
    }
}

// project target
Vector3 calcTarget(Leg_Struct &leg)
{
    // return cur point (with tip on the ground) if there is no input
    if (HexapodState == State::STANDING)
    {
        Vector3 target = leg.curPosition;
        target.z = LENGTH_TIBIA - (groundClearance + 40);

        return target;
    }

    Vector2 projectionDirection;
    Vector2 projectionOrigion;

    Vector2 rotaionProjection; // if there is no rotation input is is left as zero

    if (rotation < 0)
    {
        rotaionProjection = (Vector2::forward).rotate(leg.mountAngle - 90);
    }
    else if (rotation > 0)
    {
        rotaionProjection = (Vector2::back).rotate(leg.mountAngle - 90);
    }

    if (leg.lifted)
    {
        projectionDirection = direction + rotaionProjection;
        projectionOrigion = Vector2::zero;
    }
    else
    {
        projectionDirection = direction.inverse() + rotaionProjection.inverse();

        Vector2 newOrigion(leg.curPosition.x, leg.curPosition.y);
        projectionOrigion = newOrigion;
    }

    Vector3 target = projectPointToCircle(stepRadius, projectionOrigion, projectionDirection).toVector3();
    target.z = LENGTH_TIBIA - (groundClearance + 40);

    return target;
}

// calc path to reach target
void calcLegPath(Leg_Struct &leg, const Vector3 &target)
{

    Vector3 curPointToTarget = target - leg.curPosition;

    Vector3 section[3]; // path of lifted legs are seperated into three sections (lift, travel and lower). These add up for the path. For push Leg section[0,1,2] = Vector3::zero
    if (leg.lifted)
    {
        // calc subTargets for lifted legs
        float missingHeight = target.z + legLiftDistance - leg.curPosition.z; // height difference from CurPoint.z to LegLiftDistance
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

        section[1] = (target - section[2]) - (leg.curPosition + section[0]);
    }

    leg.pointOnPath.resize(4);

    leg.pointOnPath[0] = leg.curPosition;
    leg.pointOnPath[1] = leg.pointOnPath[0] + section[0];
    leg.pointOnPath[2] = leg.pointOnPath[1] + section[1];
    leg.pointOnPath[3] = target;
}

// calc subTarget for smooth movement based on speed
void calcInterpolatedTarget(Vector3 (&interpolatedTarget)[6])
{
    std::vector<float> pathLength;
    pathLength.resize(6);

    float substepLength[6];

    for (size_t i = 0; i < 6; i++)
    {
        pathLength[i] = calculatePathLength(Leg[i].pointOnPath);
    }

    // calc substepLength
    float movementMagnitude = direction.magnitude() + fabs(rotation);
    if (movementMagnitude == 0)
    {
        movementMagnitude = maxSpeed * deltaTime / 2; // slight bit of movement to get the legs back on the ground
    }

    float subSteps = pathLength[findLongestPath(pathLength)] / movementMagnitude;

    for (size_t i = 0; i < 6; i++)
    {
        substepLength[i] = pathLength[i] / subSteps;
    }

    // calc nextTarget by interpolating

    for (size_t i = 0; i < 6; i++)
    {
        interpolatedTarget[i] = interpolatePathByLength(Leg[i].pointOnPath, substepLength[i]);
    }
}

#endif