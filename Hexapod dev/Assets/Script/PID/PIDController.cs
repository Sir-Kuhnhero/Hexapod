using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PIDController : MonoBehaviour
{
    public float kP = 1f;
    public float kI = 0.1f;
    public float kD = 0.1f;
    public GameObject targetObject;

    public Vector3 previousError { get; private set; }
    public Vector3 integral { get; private set; }
    public Rigidbody rb { get; private set; }

    void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    void FixedUpdate()
    {
        Vector3 error = targetObject.transform.position - rb.position;
        integral += error * Time.fixedDeltaTime;
        Vector3 derivative = (error - previousError) / Time.fixedDeltaTime;
        Vector3 output = kP * error + kI * integral + kD * derivative;
        rb.AddForce(output);
        previousError = error;
    }
}