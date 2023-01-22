using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PIDPController : MonoBehaviour
{
    public float kP = 1f;
    public float kI = 0.1f;
    public float kD = 0.1f;
    public float kPreview = 0.1f;
    public GameObject targetObject;
    public Vector3 preview;

    public float previewTime = 1f;

    public Vector3 previousError { get; private set; }
    public Vector3 integral { get; private set; }
    public Vector3 derivative { get; private set; }
    public Rigidbody rb { get; private set; }

    void Start()
    {
        rb = GetComponent<Rigidbody>();
    }

    void Update()
    {
        preview = rb.position + rb.velocity * previewTime;
    }

    void FixedUpdate()
    {
        Vector3 error = targetObject.transform.position - rb.position;
        integral += error * Time.fixedDeltaTime;
        derivative = (error - previousError) / Time.fixedDeltaTime;
        Vector3 output = kP * error + kI * integral + kD * derivative + kPreview * preview;
        rb.AddForce(output);
        previousError = error;
    }
}
