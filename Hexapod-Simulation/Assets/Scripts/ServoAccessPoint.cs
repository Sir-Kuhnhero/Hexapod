using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ServoAccessPoint : MonoBehaviour
{
    [SerializeField] private List<ServoWithValue> servoList;

    [SerializeField] private float crossingPointVelocityScaler = 1;
    [SerializeField] private float maxVelocity = 1;
    [SerializeField] private float torque = 1;


    // Update is called once per frame
    void Update()
    {
        foreach (var servoInList in servoList)
        {
            servoInList.servo.targetAngle = servoInList.angle;
            servoInList.servo.crossingPointVelocityScaler = crossingPointVelocityScaler;
            servoInList.servo.maxVelocity = maxVelocity;
            servoInList.servo.torque = torque;
        }
    }
}

[System.Serializable]
public class ServoWithValue {
    public Servo servo;
    public float angle;
}
