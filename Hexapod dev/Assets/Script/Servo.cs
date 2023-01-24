using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Servo : MonoBehaviour
{
    public HingeJoint hingeJoint;
    public float targetAngle = 0;
    public float crossingPointVelocityScaler = 1;
    public float maxVelocity = 1;
    public float torque = 1;

    
    public float currentAngle { get; private set; }
    private float previousAngle;

    private float currentJointAngle;
    private float previousJointAngle;


    private JointMotor motor;

    private void Start() 
    {    
        hingeJoint.useMotor = true;
        
        motor = hingeJoint.motor;

        currentAngle = hingeJoint.angle;
        previousAngle = currentAngle;

        currentJointAngle = hingeJoint.angle;
        previousJointAngle = currentJointAngle;
    }

    private void Update()
    {
        currentJointAngle = hingeJoint.angle;
        float deltaJointAngle = currentJointAngle - previousJointAngle;

        if (deltaJointAngle > 180)
        {
            deltaJointAngle -= 360;
        } 
        else if (deltaJointAngle < -180)
        {
            deltaJointAngle += 360;
        }

        currentAngle += deltaJointAngle;


        float tempTargetAngle = targetAngle;

        float deltaAngle = tempTargetAngle - currentAngle;
        
        motor.targetVelocity = deltaAngle * Mathf.Abs(deltaAngle);
        motor.targetVelocity = motor.targetVelocity * crossingPointVelocityScaler;
        motor.targetVelocity = Mathf.Clamp(motor.targetVelocity, maxVelocity * -1, maxVelocity);

        motor.force = torque;
        hingeJoint.motor = motor;


        previousAngle = currentAngle;
        previousJointAngle = currentJointAngle;
    }
}

